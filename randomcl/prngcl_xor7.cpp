/******************************************************************************
 * @file     prngcl_xor7.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.1.2
 *
 * @brief    [PRNGCL library]
 *           contains implementation, description and initialization procedures of
 *           seven-XOR pseudo-random number generator
 *
 *
 * @section  CREDITS
 *
 *   Francois Panneton and Pierre L'ecuyer,
 *   "On the xorshift random number generators",
 *   ACM Transactions on Modeling and Computer Simulation (TOMACS), 15, 4 (2005), 346--361.
 *
 *
 * @section  LICENSE
 *
 * Copyright (c) 2013-2015 Vadim Demchik
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *    Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *****************************************************************************/

#include "hgpu_prng.h"

#define HGPU_PRNG_CL_FILE     PRNGCL_ROOT_PATH "prngcl_xor7.cl"

#define HGPU_PRNG_XOR7_m      (4294967296.0)                 // 2^32
#define HGPU_PRNG_XOR7_min    1
#define HGPU_PRNG_XOR7_max    4294967295                     // 2^32-1
#define HGPU_PRNG_XOR7_min_FP (1.0/4294967296.0)
#define HGPU_PRNG_XOR7_max_FP (4294967295.0/4294967296.0)
#define HGPU_PRNG_XOR7_k      (2.3283064365386962890625E-10) // 1/2^32

    typedef struct {
        unsigned int state[8];
        unsigned int index;
    } HGPU_PRNG_XOR7_state_t;

            static void   HGPU_PRNG_XOR7_initialize(void* PRNG_state, unsigned int PRNG_randseries);
    static unsigned int   HGPU_PRNG_XOR7_produce_one_uint_CPU(void* PRNG_state);
          static double   HGPU_PRNG_XOR7_produce_one_double_CPU(void* PRNG_state);
            static void   HGPU_PRNG_XOR7_init_GPU(HGPU_GPU_context* context,void* PRNG_state,HGPU_PRNG_parameters* PRNG_parameters);

static void
HGPU_PRNG_XOR7_initialize(void* PRNG_state, unsigned int PRNG_randseries){
    HGPU_PRNG_XOR7_state_t* state = (HGPU_PRNG_XOR7_state_t*) PRNG_state;
    HGPU_PRNG_srand(PRNG_randseries);

    state->state[0] = HGPU_PRNG_rand32bit();
    state->state[1] = HGPU_PRNG_rand32bit();
    state->state[2] = HGPU_PRNG_rand32bit();
    state->state[3] = HGPU_PRNG_rand32bit();
    state->state[4] = HGPU_PRNG_rand32bit();
    state->state[5] = HGPU_PRNG_rand32bit();
    state->state[6] = HGPU_PRNG_rand32bit();
    state->state[7] = HGPU_PRNG_rand32bit();

    state->index = 0;
}

static unsigned int
HGPU_PRNG_XOR7_produce_one_uint_CPU(void* PRNG_state){
    HGPU_PRNG_XOR7_state_t* state = (HGPU_PRNG_XOR7_state_t*) PRNG_state;
    unsigned int y, t;
        t = state->state[(state->index+7) & 7];     t = t ^ (t<<13);    y = t ^ (t<<9);
        t = state->state[(state->index+4) & 7];     y^= t ^ (t<<7);
        t = state->state[(state->index+3) & 7];     y^= t ^ (t>>3);
        t = state->state[(state->index+1) & 7];     y^= t ^ (t>>10);
        t = state->state[state->index];             t = t ^ (t>>7);     y^= t ^ (t<<24);
        state->state[state->index] = y;             state->index = (state->index+1) & 7;
    return y;
}

static double
HGPU_PRNG_XOR7_produce_one_double_CPU(void* PRNG_state){
    double y = (double) HGPU_PRNG_XOR7_produce_one_uint_CPU(PRNG_state);
	return (y / HGPU_PRNG_XOR7_m);
}

static void
HGPU_PRNG_XOR7_init_GPU(HGPU_GPU_context* context,void* PRNG_state,HGPU_PRNG_parameters* PRNG_parameters){
    HGPU_PRNG_XOR7_state_t* state = (HGPU_PRNG_XOR7_state_t*) PRNG_state;

    size_t seed_table_size         = HGPU_GPU_context_buffer_size_align(context,PRNG_parameters->instances * 2);
    size_t randoms_size            = HGPU_GPU_context_buffer_size_align(context,PRNG_parameters->instances * PRNG_parameters->samples);
    cl_uint4* PRNG_seed_table_uint4= (cl_uint4*)  calloc(seed_table_size,sizeof(cl_uint4));
    cl_float4*  PRNG_randoms        = NULL;
    cl_double4* PRNG_randoms_double = NULL;
    if (PRNG_parameters->precision==HGPU_precision_double)
        PRNG_randoms_double = (cl_double4*) calloc(randoms_size,sizeof(cl_double4));
    else
        PRNG_randoms        = (cl_float4*)  calloc(randoms_size,sizeof(cl_float4));
    if ((!PRNG_seed_table_uint4) || ((!PRNG_randoms_double) && (!PRNG_randoms)))
        HGPU_error_message(HGPU_ERROR_NO_MEMORY,"could not allocate memory for randoms");

    for (unsigned int i=1; i<seed_table_size; i++) {
        PRNG_seed_table_uint4[i].s[0] = HGPU_PRNG_rand32bit();
        PRNG_seed_table_uint4[i].s[1] = HGPU_PRNG_rand32bit();
        PRNG_seed_table_uint4[i].s[2] = HGPU_PRNG_rand32bit();
        PRNG_seed_table_uint4[i].s[3] = HGPU_PRNG_rand32bit();
    }

    PRNG_seed_table_uint4[0].s[0] = state->state[0];    // setup first thread as CPU
    PRNG_seed_table_uint4[0].s[1] = state->state[1];
    PRNG_seed_table_uint4[0].s[2] = state->state[2];
    PRNG_seed_table_uint4[0].s[3] = state->state[3];

    PRNG_seed_table_uint4[PRNG_parameters->instances].s[0] = state->state[4];
    PRNG_seed_table_uint4[PRNG_parameters->instances].s[1] = state->state[5];
    PRNG_seed_table_uint4[PRNG_parameters->instances].s[2] = state->state[6];
    PRNG_seed_table_uint4[PRNG_parameters->instances].s[3] = state->state[7];

    unsigned int seed_table_id = 0;
    unsigned int randoms_id = 0;

    seed_table_id = HGPU_GPU_context_buffer_init(context,PRNG_seed_table_uint4,HGPU_GPU_buffer_type_io,seed_table_size,sizeof(cl_uint4));
    HGPU_GPU_context_buffer_set_name(context,seed_table_id,(char*) "(XOR7) PRNG_seed_table");

    if (PRNG_parameters->precision==HGPU_precision_double)
        randoms_id = HGPU_GPU_context_buffer_init(context,PRNG_randoms_double,HGPU_GPU_buffer_type_io,randoms_size,sizeof(cl_double4));
    else
        randoms_id = HGPU_GPU_context_buffer_init(context,PRNG_randoms,HGPU_GPU_buffer_type_io,randoms_size,sizeof(cl_float4));
    HGPU_GPU_context_buffer_set_name(context,randoms_id,(char*) "(XOR7) PRNG_randoms");

    PRNG_parameters->id_buffer_input_seeds = HGPU_GPU_MAX_BUFFERS;
    PRNG_parameters->id_buffer_seeds       = seed_table_id;
    PRNG_parameters->id_buffer_randoms     = randoms_id;
}

static const HGPU_PRNG_description
HGPU_PRNG_XOR7_description = {
        "XOR7",                                       // name
        32,                                           // bitness
        HGPU_PRNG_output_type_uint4by1,               // PRNG GPU output type
        HGPU_PRNG_XOR7_min,                           // PRNG_min_uint_value
        HGPU_PRNG_XOR7_max,                           // PRNG_max_uint_value
        HGPU_PRNG_XOR7_min_FP,                        // PRNG_min_double_value
        HGPU_PRNG_XOR7_max_FP,                        // PRNG_max_double_value
        HGPU_PRNG_XOR7_k,                             // PRNG_k_value
        sizeof(HGPU_PRNG_XOR7_state_t),               // size of PRNG state
        &HGPU_PRNG_XOR7_initialize,                   // PRNG initialization
        NULL,                                         // PRNG additional parameters initialization
        &HGPU_PRNG_XOR7_produce_one_uint_CPU,         // PRNG production one unsigned integer
        &HGPU_PRNG_XOR7_produce_one_double_CPU,       // PRNG production one double
        //
        &HGPU_PRNG_XOR7_init_GPU,                     // PRNG init for GPU procedure
        NULL,                                         // PRNG additional compilation options
        HGPU_PRNG_CL_FILE,                            // PRNG source codes
        NULL,                                         // PRNG init kernel
        "xor7"                                        // PRNG production kernel
};

const HGPU_PRNG_description*  HGPU_PRNG_XOR7  = &HGPU_PRNG_XOR7_description;

#undef HGPU_PRNG_CL_FILE
