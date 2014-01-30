/******************************************************************************
 * @file     prngcl_ranmar.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.1.1
 *
 * @brief    [PRNGCL library]
 *           contains implementation, description and initialization procedures of
 *           RANMAR pseudo-random number generator
 *
 *
 * @section  CREDITS
 *
 *   George Marsaglia, Arif Zaman and Wai Wan Tsang,
 *   "Toward a universal random number generator",
 *   Statistics & Probability Letters, 9 (1990), 35--39.
 *
 *   F. James,
 *   "A review of pseudorandom number generators",
 *   Computer Physics Communications, 60 (1990), 329--344.
 *
 *
 * @section  LICENSE
 *
 * Copyright (c) 2013, 2014 Vadim Demchik
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

#define HGPU_PRNG_CL_FILE       PRNGCL_ROOT_PATH "prngcl_ranmar.cl"

#define HGPU_PRNG_RANMAR_m      (16777216.0)                   // 2^24
#define HGPU_PRNG_RANMAR_min    0                              // implementation could not generate integer values
#define HGPU_PRNG_RANMAR_max    0                              // implementation could not generate integer values
#define HGPU_PRNG_RANMAR_min_FP (1.0/16777216.0)
#define HGPU_PRNG_RANMAR_max_FP (16777215.0/16777216.0)
#define HGPU_PRNG_RANMAR_k      (5.9604644775390625E-8)        // 1/2^24

#define HGPU_PRNG_RANMAR_CD     (7654321.0f / 16777216.0f)
#define HGPU_PRNG_RANMAR_CM     (16777213.0f /16777216.0f)

#define HGPU_PRNG_RANMAR_init_1 31328
#define HGPU_PRNG_RANMAR_init_2 30081


    typedef struct {
               float  seeds[97];
                 int  I97;
                 int  J97;
               float  C;
                 int  seed1;
                 int  seed2;
    } HGPU_PRNG_RANMAR_state_t;


            static void   HGPU_PRNG_RANMAR_initialize(void* PRNG_state, unsigned int PRNG_randseries);
                   void   HGPU_PRNG_RANMAR_initialize_seedtable_CPU(void* PRNG_state);
          static double   HGPU_PRNG_RANMAR_produce_one_double_CPU(void* PRNG_state);
            static void   HGPU_PRNG_RANMAR_init_GPU(HGPU_GPU_context* context,void* PRNG_state,HGPU_PRNG_parameters* PRNG_parameters);
            static void   HGPU_PRNG_RANMAR_parameters_set(void* PRNG_state,HGPU_parameter** parameters);

static void
HGPU_PRNG_RANMAR_initialize(void* PRNG_state, unsigned int PRNG_randseries){
    HGPU_PRNG_RANMAR_state_t* state = (HGPU_PRNG_RANMAR_state_t*) PRNG_state;
    HGPU_PRNG_srand(PRNG_randseries);

    state->seed1 = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANMAR_init_1;
    state->seed2 = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANMAR_init_2;
    HGPU_PRNG_RANMAR_initialize_seedtable_CPU(PRNG_state);
}

void
HGPU_PRNG_RANMAR_initialize_seedtable_CPU(void* PRNG_state){
    HGPU_PRNG_RANMAR_state_t* state = (HGPU_PRNG_RANMAR_state_t*) PRNG_state;

    state->I97 = 96;
    state->J97 = 32;
    state->C = (362436.0 / 16777216.0);

	int i = ((state->seed1 / 177) % 177) + 2;
	int j = (state->seed1 % 177) + 2;
	int k = ((state->seed2 / 169) % 178) + 1;
	int l = (state->seed2 % 169);
	for (int n = 0; n < 97; n++)
	{
		int s = 0;
		int t = 8388608;
		for (int m = 0; m < 24; m++)
		{
			int u = (i * j) % 179;
			u = (u * k) % 179;
			i = j;
			j = k;
			k = u;
			l = (53 * l + 1) % 169;
			if ((l * u) % 64 >= 32) s = s + t; 
			t = t >> 1;
		}
		state->seeds[n] = ((float) s) / 16777216.0f;
	}
}

static double
HGPU_PRNG_RANMAR_produce_one_double_CPU(void* PRNG_state){
    HGPU_PRNG_RANMAR_state_t* state = (HGPU_PRNG_RANMAR_state_t*) PRNG_state;

	float uni = state->seeds[state->I97] - state->seeds[state->J97];
	if (uni < 0.0) {uni = uni + 1.0f;}
	state->seeds[state->I97] = uni;
	if (!state->I97) state->I97=97;
	if (!state->J97) state->J97=97;
	state->I97--;
	state->J97--;
	state->C = (float)  state->C - (float) HGPU_PRNG_RANMAR_CD;
	if (state->C < 0.0) state->C = (float) state->C + (float) HGPU_PRNG_RANMAR_CM;
	uni = uni - state->C;
	if (uni < 0.0) uni = uni + 1.0f;

	return uni;
}

static void
HGPU_PRNG_RANMAR_init_GPU(HGPU_GPU_context* context,void* PRNG_state,HGPU_PRNG_parameters* PRNG_parameters){
    HGPU_PRNG_RANMAR_state_t* state = (HGPU_PRNG_RANMAR_state_t*) PRNG_state;

    size_t seeds_size              = HGPU_GPU_context_buffer_size_align(context,PRNG_parameters->instances * 2);
    size_t seed_table_size         = HGPU_GPU_context_buffer_size_align(context,PRNG_parameters->instances * 2 * (97+1)); // 97 - size of seed table, 1 - size of indices
    size_t randoms_size            = HGPU_GPU_context_buffer_size_align(context,PRNG_parameters->instances * PRNG_parameters->samples);
    cl_uint4*  PRNG_seeds           = (cl_uint4*)  calloc(seeds_size,sizeof(cl_uint4));
    cl_float4* PRNG_seed_table_float4= (cl_float4*) calloc(seed_table_size,sizeof(cl_float4));
    cl_float4*  PRNG_randoms        = NULL;
    cl_double4* PRNG_randoms_double = NULL;
    if (PRNG_parameters->precision==HGPU_precision_double)
        PRNG_randoms_double = (cl_double4*) calloc(randoms_size,sizeof(cl_double4));
    else
        PRNG_randoms        = (cl_float4*)  calloc(randoms_size,sizeof(cl_float4));
    if ((!PRNG_seeds) || (!PRNG_seed_table_float4) || ((!PRNG_randoms_double) && (!PRNG_randoms)))
        HGPU_error_message(HGPU_ERROR_NO_MEMORY,"could not allocate memory for randoms");

    for (unsigned int i=0; i<(seeds_size>>1); i++) {
        PRNG_seeds[i].s[0] = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANMAR_init_1;
        PRNG_seeds[i].s[1] = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANMAR_init_1;
        PRNG_seeds[i].s[2] = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANMAR_init_1;
        PRNG_seeds[i].s[3] = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANMAR_init_1;
    }

    for (unsigned int i=(seeds_size>>1); i<seeds_size; i++) {
        PRNG_seeds[i].s[0] = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANMAR_init_2;
        PRNG_seeds[i].s[1] = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANMAR_init_2;
        PRNG_seeds[i].s[2] = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANMAR_init_2;
        PRNG_seeds[i].s[3] = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANMAR_init_2;
    }

    PRNG_seeds[0].s[0] = state->seed1; // setup first thread as CPU
    PRNG_seeds[PRNG_parameters->instances].s[0] = state->seed2; // setup first thread as CPU

    unsigned int seeds_id      = 0;
    unsigned int seed_table_id = 0;
    unsigned int randoms_id    = 0;

    seeds_id = HGPU_GPU_context_buffer_init(context,PRNG_seeds,HGPU_GPU_buffer_type_io,seeds_size,sizeof(cl_uint4));
    HGPU_GPU_context_buffer_set_name(context,seeds_id,(char*) "(RANMAR) PRNG_input_seeds");

    seed_table_id = HGPU_GPU_context_buffer_init(context,PRNG_seed_table_float4,HGPU_GPU_buffer_type_io,seed_table_size,sizeof(cl_float4));
    HGPU_GPU_context_buffer_set_name(context,seed_table_id,(char*) "(RANMAR) PRNG_seed_table");

    if (PRNG_parameters->precision==HGPU_precision_double)
        randoms_id = HGPU_GPU_context_buffer_init(context,PRNG_randoms_double,HGPU_GPU_buffer_type_io,randoms_size,sizeof(cl_double4));
    else
        randoms_id = HGPU_GPU_context_buffer_init(context,PRNG_randoms,HGPU_GPU_buffer_type_io,randoms_size,sizeof(cl_float4));
    HGPU_GPU_context_buffer_set_name(context,randoms_id,(char*) "(RANMAR) PRNG_randoms");

    PRNG_parameters->id_buffer_input_seeds = seeds_id;
    PRNG_parameters->id_buffer_seeds       = seed_table_id;
    PRNG_parameters->id_buffer_randoms     = randoms_id;
}

static void
HGPU_PRNG_RANMAR_parameters_set(void* PRNG_state,HGPU_parameter** parameters){
    if ((!parameters) || (!PRNG_state)) return;
    HGPU_PRNG_RANMAR_state_t* state = (HGPU_PRNG_RANMAR_state_t*) PRNG_state;
    HGPU_parameter* parameter = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PRNG_SEED1);
    if (parameter) (*state).seed1 = parameter->value_integer;

    parameter = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PRNG_SEED2);
    if (parameter) (*state).seed2 = parameter->value_integer;

}

static const HGPU_PRNG_description
HGPU_PRNG_RANMAR_description = {
        "RANMAR",                                     // name
        24,                                           // bitness
        HGPU_PRNG_output_type_float,                  // PRNG GPU output type
        HGPU_PRNG_RANMAR_min,                         // PRNG_min_uint_value
        HGPU_PRNG_RANMAR_max,                         // PRNG_max_uint_value
        HGPU_PRNG_RANMAR_min_FP,                      // PRNG_min_double_value
        HGPU_PRNG_RANMAR_max_FP,                      // PRNG_max_double_value
        HGPU_PRNG_RANMAR_k,                           // PRNG_k_value
        sizeof(HGPU_PRNG_RANMAR_state_t),             // size of PRNG state
        &HGPU_PRNG_RANMAR_initialize,                 // PRNG initialization
        &HGPU_PRNG_RANMAR_parameters_set,             // PRNG additional parameters initialization
        NULL,                                         // PRNG production one unsigned integer
        &HGPU_PRNG_RANMAR_produce_one_double_CPU,     // PRNG production one double
        //
        &HGPU_PRNG_RANMAR_init_GPU,                   // PRNG init for GPU procedure
        NULL,                                         // PRNG additional compilation options
        HGPU_PRNG_CL_FILE,                            // PRNG source codes
        "ranmar_init",                                // PRNG init kernel
        "ranmar"                                      // PRNG production kernel
};

const HGPU_PRNG_description*  HGPU_PRNG_RANMAR = &HGPU_PRNG_RANMAR_description;

#undef HGPU_PRNG_CL_FILE
