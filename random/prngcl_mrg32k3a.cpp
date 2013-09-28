/******************************************************************************
 * @file     prngcl_mrg32k3a.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0
 *
 * @brief    [PRNGCL library]
 *           contains implementation, description and initialization procedures of
 *           MRG32k3a pseudo-random number generator
 *
 *
 * @section  CREDITS
 *
 *   Pierre L'Ecuyer,
 *   "Good Parameter Sets for Combined Multiple Recursive Random Number Generators",
 *   Operations Research, 47, 1 (1999), 159--164.
 *
 *
 * @section  LICENSE
 *
 * Copyright (c) 2013, Vadim Demchik
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

#define HGPU_PRNG_MRG32K3A_m_FP   (4294967088.0)                 // 2^32-208
#define HGPU_PRNG_MRG32K3A_min    1
#define HGPU_PRNG_MRG32K3A_max    4294967087                     // 2^32-209
#define HGPU_PRNG_MRG32K3A_min_FP (1.0/4294967088.0)
#define HGPU_PRNG_MRG32K3A_max_FP (4294967087.0/4294967088.0)
#define HGPU_PRNG_MRG32K3A_k      (2.3283065492957276882397E-10) // 1/(2^32-208)
                                   
#define HGPU_PRNG_MRG32K3A_init_1  4294967087
#define HGPU_PRNG_MRG32K3A_init_2  4294944443

#define HGPU_PRNG_MRG32K3A_m          2.3283065492957276882397381481401e-10 // 1/(2^32-208) for MRG32k3a generator
#define HGPU_PRNG_MRG32K3A_m1         4294967087
#define HGPU_PRNG_MRG32K3A_m2         4294944443
#define HGPU_PRNG_MRG32K3A_a11        1403580LL
#define HGPU_PRNG_MRG32K3A_a12        810728LL
#define HGPU_PRNG_MRG32K3A_a21        527612LL
#define HGPU_PRNG_MRG32K3A_a22        1370589LL

    typedef struct {
          unsigned int  seeds[6];
    } HGPU_PRNG_MRG32K3A_state_t;

            static void   HGPU_PRNG_MRG32K3A_initialize(void* PRNG_state, unsigned int PRNG_randseries);
    static unsigned int   HGPU_PRNG_MRG32K3A_produce_one_uint_CPU(void* PRNG_state);
            static void   HGPU_PRNG_MRG32K3A_init_GPU(HGPU_GPU_context* context,void* PRNG_state,HGPU_PRNG_parameters* PRNG_parameters);
            static void   HGPU_PRNG_MRG32K3A_parameters_set(void* PRNG_state,HGPU_parameter** parameters);

inline double
trunc(double x) {
	double y, z;
	y = modf(x,&z);
	return z;
}

static void
HGPU_PRNG_MRG32K3A_initialize(void* PRNG_state, unsigned int PRNG_randseries){
    HGPU_PRNG_MRG32K3A_state_t* state = (HGPU_PRNG_MRG32K3A_state_t*) PRNG_state;
    HGPU_PRNG_srand(PRNG_randseries);

    state->seeds[0] = (HGPU_PRNG_rand32bit() % HGPU_PRNG_MRG32K3A_init_1);
    state->seeds[1] = (HGPU_PRNG_rand32bit() % HGPU_PRNG_MRG32K3A_init_1);
    state->seeds[2] = (HGPU_PRNG_rand32bit() % HGPU_PRNG_MRG32K3A_init_1);

    state->seeds[3] = (HGPU_PRNG_rand32bit() % HGPU_PRNG_MRG32K3A_init_2);
    state->seeds[4] = (HGPU_PRNG_rand32bit() % HGPU_PRNG_MRG32K3A_init_2);
    state->seeds[5] = (HGPU_PRNG_rand32bit() % HGPU_PRNG_MRG32K3A_init_2);
}

static unsigned int
HGPU_PRNG_MRG32K3A_produce_one_uint_CPU(void* PRNG_state){
    HGPU_PRNG_MRG32K3A_state_t* state = (HGPU_PRNG_MRG32K3A_state_t*) PRNG_state;

    unsigned int rnd;
    long long x1n, x2n;
    long long k;

    x1n  = HGPU_PRNG_MRG32K3A_a11 * state->seeds[1] - HGPU_PRNG_MRG32K3A_a12 * state->seeds[0];
    k = x1n / HGPU_PRNG_MRG32K3A_m1;
    x1n -= k * HGPU_PRNG_MRG32K3A_m1;
    if (x1n < 0) x1n += HGPU_PRNG_MRG32K3A_m1;
    state->seeds[0] = state->seeds[1];
    state->seeds[1] = state->seeds[2];
    state->seeds[2] = (unsigned int) x1n;

    x2n  = HGPU_PRNG_MRG32K3A_a21 * state->seeds[5] - HGPU_PRNG_MRG32K3A_a22 * state->seeds[3];
    k = x2n / HGPU_PRNG_MRG32K3A_m2;
    x2n -= k * HGPU_PRNG_MRG32K3A_m2;
    if (x2n < 0) x2n += HGPU_PRNG_MRG32K3A_m2;
    state->seeds[3] = state->seeds[4];
    state->seeds[4] = state->seeds[5];
    state->seeds[5] = (unsigned int) x2n;

    if (x1n>x2n) rnd = (unsigned int) (x1n - x2n);
    else         rnd = (unsigned int) (x1n - x2n  + HGPU_PRNG_MRG32K3A_m1);

 	return rnd;
}

static double
HGPU_PRNG_MRG32K3A_produce_one_double_CPU(void* PRNG_state){
    double result = (double) HGPU_PRNG_MRG32K3A_produce_one_uint_CPU(PRNG_state);
	return (result * HGPU_PRNG_MRG32K3A_m);
}

static void
HGPU_PRNG_MRG32K3A_init_GPU(HGPU_GPU_context* context,void* PRNG_state,HGPU_PRNG_parameters* PRNG_parameters){
    HGPU_PRNG_MRG32K3A_state_t* state = (HGPU_PRNG_MRG32K3A_state_t*) PRNG_state;

    size_t seed_table_size         = HGPU_GPU_context_buffer_size_align(context,PRNG_parameters->instances * 2);
    size_t randoms_size            = HGPU_GPU_context_buffer_size_align(context,PRNG_parameters->instances * PRNG_parameters->samples);
    cl_uint4*  PRNG_seed_table_uint4 = (cl_uint4*)  calloc(seed_table_size,sizeof(cl_uint4));
    cl_float4*  PRNG_randoms         = NULL;
    cl_double4* PRNG_randoms_double  = NULL;
    if (PRNG_parameters->precision==HGPU_precision_double)
        PRNG_randoms_double = (cl_double4*) calloc(randoms_size,sizeof(cl_double4));
    else
        PRNG_randoms        = (cl_float4*)  calloc(randoms_size,sizeof(cl_float4));
    if ((!PRNG_seed_table_uint4) || ((!PRNG_randoms_double) && (!PRNG_randoms)))
        HGPU_error_message(HGPU_ERROR_NO_MEMORY,"could not allocate memory for randoms");

    for (unsigned int i=0; i<(seed_table_size>>1); i++) {
        PRNG_seed_table_uint4[i].s[0] = (HGPU_PRNG_rand32bit() % HGPU_PRNG_MRG32K3A_init_1);
        PRNG_seed_table_uint4[i].s[1] = (HGPU_PRNG_rand32bit() % HGPU_PRNG_MRG32K3A_init_1);
        PRNG_seed_table_uint4[i].s[2] = (HGPU_PRNG_rand32bit() % HGPU_PRNG_MRG32K3A_init_1);
        PRNG_seed_table_uint4[i].s[3] = (HGPU_PRNG_rand32bit() % HGPU_PRNG_MRG32K3A_init_1);
    }

    for (unsigned int i=(seed_table_size>>1); i<seed_table_size; i++) {
        PRNG_seed_table_uint4[i].s[0] = (HGPU_PRNG_rand32bit() % HGPU_PRNG_MRG32K3A_init_2);
        PRNG_seed_table_uint4[i].s[1] = (HGPU_PRNG_rand32bit() % HGPU_PRNG_MRG32K3A_init_2);
        PRNG_seed_table_uint4[i].s[2] = (HGPU_PRNG_rand32bit() % HGPU_PRNG_MRG32K3A_init_2);
        PRNG_seed_table_uint4[i].s[3] = (HGPU_PRNG_rand32bit() % HGPU_PRNG_MRG32K3A_init_2);
    }

    PRNG_seed_table_uint4[0].s[0] = state->seeds[0];    // setup first thread as CPU
    PRNG_seed_table_uint4[0].s[1] = state->seeds[1];
    PRNG_seed_table_uint4[0].s[2] = state->seeds[2];

    PRNG_seed_table_uint4[PRNG_parameters->instances].s[0] = state->seeds[3];
    PRNG_seed_table_uint4[PRNG_parameters->instances].s[1] = state->seeds[4];
    PRNG_seed_table_uint4[PRNG_parameters->instances].s[2] = state->seeds[5];

    unsigned int seed_table_id = 0;
    unsigned int randoms_id = 0;

    seed_table_id = HGPU_GPU_context_buffer_init(context,PRNG_seed_table_uint4,HGPU_GPU_buffer_type_io,seed_table_size,sizeof(cl_uint4));
    HGPU_GPU_context_buffer_set_name(context,seed_table_id,(char*) "(MRG32K3A) PRNG_seed_table");

    if (PRNG_parameters->precision==HGPU_precision_double)
        randoms_id = HGPU_GPU_context_buffer_init(context,PRNG_randoms_double,HGPU_GPU_buffer_type_io,randoms_size,sizeof(cl_double4));
    else
        randoms_id = HGPU_GPU_context_buffer_init(context,PRNG_randoms,HGPU_GPU_buffer_type_io,randoms_size,sizeof(cl_float4));
    HGPU_GPU_context_buffer_set_name(context,randoms_id,(char*) "(MRG32K3A) PRNG_randoms");

    PRNG_parameters->id_buffer_input_seeds = HGPU_GPU_MAX_BUFFERS;
    PRNG_parameters->id_buffer_seeds       = seed_table_id;
    PRNG_parameters->id_buffer_randoms     = randoms_id;
}

static const HGPU_PRNG_description
HGPU_PRNG_MRG32K3A_description = {
        "MRG32K3A",                                   // name
        32,                                           // bitness
        HGPU_PRNG_output_type_float4,                 // PRNG GPU output type
        HGPU_PRNG_MRG32K3A_min,                       // PRNG_min_uint_value
        HGPU_PRNG_MRG32K3A_max,                       // PRNG_max_uint_value
        HGPU_PRNG_MRG32K3A_min_FP,                    // PRNG_min_double_value
        HGPU_PRNG_MRG32K3A_max_FP,                    // PRNG_max_double_value
        HGPU_PRNG_MRG32K3A_k,                         // PRNG_k_value
        sizeof(HGPU_PRNG_MRG32K3A_state_t),           // size of PRNG state
        &HGPU_PRNG_MRG32K3A_initialize,               // PRNG initialization
        NULL,                                         // PRNG additional parameters initialization
        &HGPU_PRNG_MRG32K3A_produce_one_uint_CPU,     // PRNG production one unsigned integer
        &HGPU_PRNG_MRG32K3A_produce_one_double_CPU,   // PRNG production one double
        //
        &HGPU_PRNG_MRG32K3A_init_GPU,                 // PRNG init for GPU procedure
        NULL,                                         // PRNG additional compilation options
        "random/prngcl_mrg32k3a.cl",                  // PRNG source codes
        NULL,                                         // PRNG init kernel
        "mrg32k3a"                                    // PRNG production kernel
};

const HGPU_PRNG_description*  HGPU_PRNG_MRG32K3A = &HGPU_PRNG_MRG32K3A_description;
