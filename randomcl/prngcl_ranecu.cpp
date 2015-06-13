/******************************************************************************
 * @file     prngcl_ranecu.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.1.2
 *
 * @brief    [PRNGCL library]
 *           contains implementation, description and initialization procedures of
 *           RANECU pseudo-random number generator
 *
 *
 * @section  CREDITS
 *
 *   F. James,
 *   "A review of pseudorandom number generators",
 *   Computer Physics Communications, 60 (1990), 329--344.
 *
 *   Pierre L'Ecuyer,
 *   "Efficient and Portable Combined Random Number Generators",
 *   Communications of the ACM, 31, 6 (1988), 742--751.
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

#define HGPU_PRNG_CL_FILE        PRNGCL_ROOT_PATH "prngcl_ranecu.cl"

#define HGPU_PRNG_RANECU_m       (2147483648.0)                 // 2^31
#define HGPU_PRNG_RANECU_min     1
#define HGPU_PRNG_RANECU_max     2147483647                     // 2^31-1
#define HGPU_PRNG_RANECU_min_FP  (1.0/2147483648.0)
#define HGPU_PRNG_RANECU_max_FP  (2147483647.0/2147483648.0)
#define HGPU_PRNG_RANECU_k       (4.656612873077392578125E-10)  // 1/2^31

#define HGPU_PRNG_RANECU_twom31	 (2147483648.0)

#define HGPU_PRNG_RANECU_icons1  2147483563
#define HGPU_PRNG_RANECU_icons2  2147483399
#define HGPU_PRNG_RANECU_icons3  2147483562

#define HGPU_PRNG_RANECU_init_1  2147483563
#define HGPU_PRNG_RANECU_init_2  2147483398

#define HGPU_PRNG_RANECU_seedP11 53668
#define HGPU_PRNG_RANECU_seedP12 12211
#define HGPU_PRNG_RANECU_seedP13 40014

#define HGPU_PRNG_RANECU_seedP21 52774
#define HGPU_PRNG_RANECU_seedP22 3791
#define HGPU_PRNG_RANECU_seedP23 40692

    typedef struct {
                 int  jseed1;
                 int  jseed2;
    } HGPU_PRNG_RANECU_state_t;

            static void   HGPU_PRNG_RANECU_initialize(void* PRNG_state, unsigned int PRNG_randseries);
    static unsigned int   HGPU_PRNG_RANECU_produce_one_uint_CPU(void* PRNG_state);
          static double   HGPU_PRNG_RANECU_produce_one_double_CPU(void* PRNG_state);
            static void   HGPU_PRNG_RANECU_init_GPU(HGPU_GPU_context* context,void* PRNG_state,HGPU_PRNG_parameters* PRNG_parameters);
            static void   HGPU_PRNG_RANECU_parameters_set(void* PRNG_state,HGPU_parameter** parameters);

static void
HGPU_PRNG_RANECU_initialize(void* PRNG_state, unsigned int PRNG_randseries){
    HGPU_PRNG_RANECU_state_t* state = (HGPU_PRNG_RANECU_state_t*) PRNG_state;
    HGPU_PRNG_srand(PRNG_randseries);

    state->jseed1 = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANECU_init_1;
    state->jseed2 = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANECU_init_2;
}

static unsigned int
HGPU_PRNG_RANECU_produce_one_uint_CPU(void* PRNG_state){
    HGPU_PRNG_RANECU_state_t* state = (HGPU_PRNG_RANECU_state_t*) PRNG_state;
    int k;
    //
    k = state->jseed1 / HGPU_PRNG_RANECU_seedP11;
        state->jseed1 = HGPU_PRNG_RANECU_seedP13 * (state->jseed1 - k * HGPU_PRNG_RANECU_seedP11) - k * HGPU_PRNG_RANECU_seedP12;
        if (state->jseed1 < 0) state->jseed1 += HGPU_PRNG_RANECU_icons1;

    k = state->jseed2 / HGPU_PRNG_RANECU_seedP21;
        state->jseed2 = HGPU_PRNG_RANECU_seedP23 * (state->jseed2 - k * HGPU_PRNG_RANECU_seedP21) - k * HGPU_PRNG_RANECU_seedP22;
        if (state->jseed2 < 0) state->jseed2 += HGPU_PRNG_RANECU_icons2;

    int z = state->jseed1 - state->jseed2;
    if (z < 1) z += HGPU_PRNG_RANECU_icons3;

	return z;
}

static double
HGPU_PRNG_RANECU_produce_one_double_CPU(void* PRNG_state){
    double y = (double) HGPU_PRNG_RANECU_produce_one_uint_CPU(PRNG_state);
	return (y / HGPU_PRNG_RANECU_twom31);
}

static void
HGPU_PRNG_RANECU_init_GPU(HGPU_GPU_context* context,void* PRNG_state,HGPU_PRNG_parameters* PRNG_parameters){
    HGPU_PRNG_RANECU_state_t* state = (HGPU_PRNG_RANECU_state_t*) PRNG_state;

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

    for (unsigned int i=0; i<(seed_table_size>>1); i++) {
        PRNG_seed_table_uint4[i].s[0] = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANECU_init_1;
        PRNG_seed_table_uint4[i].s[1] = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANECU_init_1;
        PRNG_seed_table_uint4[i].s[2] = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANECU_init_1;
        PRNG_seed_table_uint4[i].s[3] = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANECU_init_1;
    }

    for (unsigned int i=(seed_table_size>>1); i<seed_table_size; i++) {
        PRNG_seed_table_uint4[i].s[0] = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANECU_init_2;
        PRNG_seed_table_uint4[i].s[1] = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANECU_init_2;
        PRNG_seed_table_uint4[i].s[2] = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANECU_init_2;
        PRNG_seed_table_uint4[i].s[3] = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANECU_init_2;
    }

    PRNG_seed_table_uint4[0].s[0] = state->jseed1;    // setup first thread as CPU
    PRNG_seed_table_uint4[PRNG_parameters->instances].s[0] = state->jseed2;

    unsigned int seed_table_id = 0;
    unsigned int randoms_id = 0;

    seed_table_id = HGPU_GPU_context_buffer_init(context,PRNG_seed_table_uint4,HGPU_GPU_buffer_type_io,seed_table_size,sizeof(cl_uint4));
    HGPU_GPU_context_buffer_set_name(context,seed_table_id,(char*) "(RANECU) PRNG_seed_table");

    if (PRNG_parameters->precision==HGPU_precision_double)
        randoms_id = HGPU_GPU_context_buffer_init(context,PRNG_randoms_double,HGPU_GPU_buffer_type_io,randoms_size,sizeof(cl_double4));
    else
        randoms_id = HGPU_GPU_context_buffer_init(context,PRNG_randoms,HGPU_GPU_buffer_type_io,randoms_size,sizeof(cl_float4));
    HGPU_GPU_context_buffer_set_name(context,randoms_id,(char*) "(RANECU) PRNG_randoms");

    PRNG_parameters->id_buffer_input_seeds = HGPU_GPU_MAX_BUFFERS;
    PRNG_parameters->id_buffer_seeds       = seed_table_id;
    PRNG_parameters->id_buffer_randoms     = randoms_id;
}

static void
HGPU_PRNG_RANECU_parameters_set(void* PRNG_state,HGPU_parameter** parameters){
    if ((!parameters) || (!PRNG_state)) return;
    HGPU_PRNG_RANECU_state_t* state = (HGPU_PRNG_RANECU_state_t*) PRNG_state;
    HGPU_parameter* parameter = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PRNG_SEED1);
    if (parameter) (*state).jseed1 = parameter->value_integer;

    parameter = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PRNG_SEED2);
    if (parameter) (*state).jseed2 = parameter->value_integer;

}

static const HGPU_PRNG_description
HGPU_PRNG_RANECU_description = {
        "RANECU",                                     // name
        31,                                           // bitness
        HGPU_PRNG_output_type_uint,                   // PRNG GPU output type
        HGPU_PRNG_RANECU_min,                         // PRNG_min_uint_value
        HGPU_PRNG_RANECU_max,                         // PRNG_max_uint_value
        HGPU_PRNG_RANECU_min_FP,                      // PRNG_min_double_value
        HGPU_PRNG_RANECU_max_FP,                      // PRNG_max_double_value
        HGPU_PRNG_RANECU_k,                           // PRNG_k_value
        sizeof(HGPU_PRNG_RANECU_state_t),             // size of PRNG state
        &HGPU_PRNG_RANECU_initialize,                 // PRNG initialization
        &HGPU_PRNG_RANECU_parameters_set,             // PRNG additional parameters initialization
        &HGPU_PRNG_RANECU_produce_one_uint_CPU,       // PRNG production one unsigned integer
        &HGPU_PRNG_RANECU_produce_one_double_CPU,     // PRNG production one double
        //
        &HGPU_PRNG_RANECU_init_GPU,                   // PRNG init for GPU procedure
        NULL,                                         // PRNG additional compilation options
        HGPU_PRNG_CL_FILE,                            // PRNG source codes
        NULL,                                         // PRNG init kernel
        "ranecu"                                      // PRNG production kernel
};

const HGPU_PRNG_description*  HGPU_PRNG_RANECU = &HGPU_PRNG_RANECU_description;

#undef HGPU_PRNG_CL_FILE
