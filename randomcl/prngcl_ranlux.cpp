/******************************************************************************
 * @file     prngcl_ranlux.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.1.1
 *
 * @brief    [PRNGCL library]
 *           contains implementation, description and initialization procedures of
 *           RANLUX pseudo-random number generator
 *
 *
 * @section  CREDITS
 *
 *   Martin Luscher,
 *   "A portable high-quality random number generator for lattice field theory simulations",
 *   Computer Physics Communications, 79 (1994), 100--110.
 *
 *   F. James,
 *   "RANLUX: A Fortran implementation of the high-quality pseudorandom number generator of Luscher",
 *   Computer Physics Communications, 79 (1994), 111--114.
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

#define HGPU_PRNG_CL_FILE        PRNGCL_ROOT_PATH "prngcl_ranlux.cl"

#define HGPU_PRNG_RANLUX_m       (16777216.0)                   // 2^24
#define HGPU_PRNG_RANLUX_min     0                              // implementation could not generate integer values
#define HGPU_PRNG_RANLUX_max     0                              // implementation could not generate integer values
#define HGPU_PRNG_RANLUX_min_FP  (1.0/16777216.0)
#define HGPU_PRNG_RANLUX_max_FP  (16777215.0/16777216.0)
#define HGPU_PRNG_RANLUX_k       (5.9604644775390625E-8)        // 1/2^24

#define HGPU_PRNG_RANLUX_icons   2147483563
#define HGPU_PRNG_RANLUX_itwo24	 16777216	// 1<<24
#define HGPU_PRNG_RANLUX_twom24  0.000000059604644775390625f
#define HGPU_PRNG_RANLUX_twom12  0.000244140625f

#define HGPU_PRNG_RANLUX_init    2147483647

    typedef struct {
               float  seeds[24];
                 int  i24;
                 int  j24;
                 int  in24;
               float  carry;
                 int  nskip;
                 int  CPU_seed; // MAX = 2147483647
                 int  seed;
    } HGPU_PRNG_RANLUX_state_t;


            static void   HGPU_PRNG_RANLUX_initialize(void* PRNG_state, unsigned int PRNG_randseries);
                   void   HGPU_PRNG_RANLUX_initialize_seedtable_CPU(void* PRNG_state);
          static double   HGPU_PRNG_RANLUX_produce_one_double_CPU(void* PRNG_state);
            static void   HGPU_PRNG_RANLUX_init_GPU(HGPU_GPU_context* context,void* PRNG_state,HGPU_PRNG_parameters* PRNG_parameters);
            static char*  HGPU_PRNG_RANLUX_options_GPU(HGPU_GPU_context* context,void* PRNG_state,HGPU_PRNG_parameters* PRNG_parameters);
            static void   HGPU_PRNG_RANLUX_parameters_set(void* PRNG_state,HGPU_parameter** parameters);

static void
HGPU_PRNG_RANLUX_initialize(void* PRNG_state, unsigned int PRNG_randseries){
    HGPU_PRNG_RANLUX_state_t* state = (HGPU_PRNG_RANLUX_state_t*) PRNG_state;
    HGPU_PRNG_srand(PRNG_randseries);

    state->CPU_seed = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANLUX_init;
    state->seed = state->CPU_seed;
    state->nskip = 223;
    HGPU_PRNG_RANLUX_initialize_seedtable_CPU(PRNG_state);
}

void
HGPU_PRNG_RANLUX_initialize_seedtable_CPU(void* PRNG_state){
    HGPU_PRNG_RANLUX_state_t* state = (HGPU_PRNG_RANLUX_state_t*) PRNG_state;

    state->i24   = 23;
    state->j24   = 9;
    state->in24  = 0;
    state->carry = 0.0;

	int k;
	//
	for (int i=0; i<24; i++){
	    k = state->CPU_seed / 53668;
        state->CPU_seed = 40014 * (state->CPU_seed - k * 53668) - k * 12211;
	    if (state->CPU_seed < 0) state->CPU_seed += HGPU_PRNG_RANLUX_icons;
	    state->seeds[i] = ((float) (state->CPU_seed % HGPU_PRNG_RANLUX_itwo24)) * HGPU_PRNG_RANLUX_twom24;
	}
	if (state->seeds[23] == 0) state->carry = HGPU_PRNG_RANLUX_twom24;
}

static double
HGPU_PRNG_RANLUX_produce_one_double_CPU(void* PRNG_state){
    HGPU_PRNG_RANLUX_state_t* state = (HGPU_PRNG_RANLUX_state_t*) PRNG_state;

    float uni;

    if (!state->in24) {
        state->in24 = 24;
        for (int isk = 0; isk < state->nskip; isk++){
            uni = state->seeds[state->j24] - state->seeds[state->i24] - state->carry;
            if (uni < 0.0f){
                uni += 1.0f;
                state->carry = HGPU_PRNG_RANLUX_twom24;
            } else
                state->carry = 0.0f;
            state->seeds[state->i24] = uni;
            if (!state->i24) state->i24=24;
            if (!state->j24) state->j24=24;
            state->i24--;
            state->j24--;
        }
    }

    uni = state->seeds[state->j24] - state->seeds[state->i24] - state->carry;
    if (uni < 0.0f) {
        uni += 1.0f;
        state->carry = HGPU_PRNG_RANLUX_twom24;
    } else
        state->carry = 0.0f;
    state->seeds[state->i24] = uni;
    if (!state->i24) state->i24=24;
    if (!state->j24) state->j24=24;
    state->i24--;
    state->j24--;

    if (uni < HGPU_PRNG_RANLUX_twom12) {
        uni += HGPU_PRNG_RANLUX_twom24 * state->seeds[state->j24];
        if (!uni) uni = HGPU_PRNG_RANLUX_twom24 * HGPU_PRNG_RANLUX_twom24;
    }
    state->in24--;

    return uni;
}

static void
HGPU_PRNG_RANLUX_init_GPU(HGPU_GPU_context* context,void* PRNG_state,HGPU_PRNG_parameters* PRNG_parameters){
    HGPU_PRNG_RANLUX_state_t* state = (HGPU_PRNG_RANLUX_state_t*) PRNG_state;

    size_t seeds_size      = HGPU_GPU_context_buffer_size_align(context,PRNG_parameters->instances);
    size_t seed_table_size = HGPU_GPU_context_buffer_size_align(context,PRNG_parameters->instances * 7); // 7 = size of Ranlux seed table for each PRNG (in quads)
    size_t randoms_size    = HGPU_GPU_context_buffer_size_align(context,PRNG_parameters->instances * PRNG_parameters->samples);
    cl_uint*    PRNG_seeds             = (cl_uint*)   calloc(seeds_size,sizeof(cl_uint));
    cl_float4*  PRNG_seed_table_float4 = (cl_float4*) calloc(seed_table_size,sizeof(cl_float4));
    cl_float4*  PRNG_randoms        = NULL;
    cl_double4* PRNG_randoms_double = NULL;
    if (PRNG_parameters->precision==HGPU_precision_double)
        PRNG_randoms_double = (cl_double4*) calloc(randoms_size,sizeof(cl_double4));
    else
        PRNG_randoms        = (cl_float4*)  calloc(randoms_size,sizeof(cl_float4));
    if ((!PRNG_seeds) || (!PRNG_seed_table_float4) || ((!PRNG_randoms_double) && (!PRNG_randoms)))
        HGPU_error_message(HGPU_ERROR_NO_MEMORY,"could not allocate memory for randoms");

    for (unsigned int i=1; i<seeds_size; i++)
        PRNG_seeds[i] = HGPU_PRNG_rand32bit() % HGPU_PRNG_RANLUX_init;

    PRNG_seeds[0] = state->seed; // setup first thread as CPU

    unsigned int seeds_id      = 0;
    unsigned int seed_table_id = 0;
    unsigned int randoms_id    = 0;

    seeds_id = HGPU_GPU_context_buffer_init(context,PRNG_seeds,HGPU_GPU_buffer_type_io,seeds_size,sizeof(cl_uint));
    HGPU_GPU_context_buffer_set_name(context,seeds_id,(char*) "(RANLUX) PRNG_input_seeds");

    seed_table_id = HGPU_GPU_context_buffer_init(context,PRNG_seed_table_float4,HGPU_GPU_buffer_type_io,seed_table_size,sizeof(cl_float4));
    HGPU_GPU_context_buffer_set_name(context,seed_table_id,(char*) "(RANLUX) PRNG_seed_table");

    if (PRNG_parameters->precision==HGPU_precision_double)
        randoms_id = HGPU_GPU_context_buffer_init(context,PRNG_randoms_double,HGPU_GPU_buffer_type_io,randoms_size,sizeof(cl_double4));
    else
        randoms_id = HGPU_GPU_context_buffer_init(context,PRNG_randoms,HGPU_GPU_buffer_type_io,randoms_size,sizeof(cl_float4));
    HGPU_GPU_context_buffer_set_name(context,randoms_id,(char*) "(RANLUX) PRNG_randoms");

    PRNG_parameters->id_buffer_input_seeds = seeds_id;
    PRNG_parameters->id_buffer_seeds       = seed_table_id;
    PRNG_parameters->id_buffer_randoms     = randoms_id;
}

static char*
HGPU_PRNG_RANLUX_options_GPU(HGPU_GPU_context* context,void* PRNG_state,HGPU_PRNG_parameters*){
    char* result = NULL;
    if ((!context) || (!PRNG_state)) return result;
    result = (char*) calloc(HGPU_GPU_MAX_OPTIONS_LENGTH,sizeof(char));
    HGPU_PRNG_RANLUX_state_t* state = (HGPU_PRNG_RANLUX_state_t*) PRNG_state;
    sprintf_s(result,HGPU_GPU_MAX_OPTIONS_LENGTH,"-D RL_skip=%i",state->nskip);
    return result;
}

static void
HGPU_PRNG_RANLUX_parameters_set(void* PRNG_state,HGPU_parameter** parameters){
    if ((!parameters) || (!PRNG_state)) return;
    HGPU_PRNG_RANLUX_state_t* state = (HGPU_PRNG_RANLUX_state_t*) PRNG_state;
    HGPU_parameter* parameter = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PRNG_SEED1);
    if (parameter) {
        (*state).CPU_seed = parameter->value_integer;
        (*state).seed = parameter->value_integer;
    }

    parameter = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PRNG_RANLUX_NSKIP);
    if (parameter) (*state).nskip = parameter->value_integer;

}

static const HGPU_PRNG_description
HGPU_PRNG_RANLUX_description = {
        "RANLUX",                                     // name
        24,                                           // bitness
        HGPU_PRNG_output_type_float4,                 // PRNG GPU output type
        HGPU_PRNG_RANLUX_min,                         // PRNG_min_uint_value
        HGPU_PRNG_RANLUX_max,                         // PRNG_max_uint_value
        HGPU_PRNG_RANLUX_min_FP,                      // PRNG_min_double_value
        HGPU_PRNG_RANLUX_max_FP,                      // PRNG_max_double_value
        HGPU_PRNG_RANLUX_k,                           // PRNG_k_value
        sizeof(HGPU_PRNG_RANLUX_state_t),             // size of PRNG state
        &HGPU_PRNG_RANLUX_initialize,                 // PRNG initialization
        &HGPU_PRNG_RANLUX_parameters_set,             // PRNG additional parameters initialization
        NULL,                                         // PRNG production one unsigned integer
        &HGPU_PRNG_RANLUX_produce_one_double_CPU,     // PRNG production one double
        //
        &HGPU_PRNG_RANLUX_init_GPU,                   // PRNG init for GPU procedure
        &HGPU_PRNG_RANLUX_options_GPU,                // PRNG additional compilation options
        HGPU_PRNG_CL_FILE,                            // PRNG source codes
        "ranlux_init",                                // PRNG init kernel
        "ranlux"                                      // PRNG production kernel
};

const HGPU_PRNG_description*  HGPU_PRNG_RANLUX = &HGPU_PRNG_RANLUX_description;

// setup default PRNG and parameters
    const HGPU_PRNG_description*  HGPU_PRNG_default            = &HGPU_PRNG_RANLUX_description;
                 HGPU_precision   HGPU_PRNG_default_precision  = HGPU_precision_single;
                   unsigned int   HGPU_PRNG_default_randseries = 0;
                   unsigned int   HGPU_PRNG_default_instances  = 0;
                   unsigned int   HGPU_PRNG_default_samples    = HGPU_PRNG_DEFAULT_SAMPLES; /* in quads */

#undef HGPU_PRNG_CL_FILE
