/******************************************************************************
 * @file     hgpu_prng.h
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.1
 *
 * @brief    [PRNGCL library]
 *           Pseudo-random number generators for HGPU package
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

#ifndef HGPU_PRNG_H
#define HGPU_PRNG_H

#include "../include/hgpu_enum.h"
#include "../include/hgpu_error.h"
#include "../include/hgpu_parameters.h"
#include "../hgpucl/hgpucl_context.h"
#include "../hgpucl/hgpucl_program.h"
#include "hgpu_prng_test.h"


#define PRNGCL_VERSION_MAJOR 1
#define PRNGCL_VERSION_MINOR 1
#define PRNGCL_VERSION_MICRO 0

#define PRNGCL_ROOT_PATH    "randomcl/"     // global root path to OpenCL-files

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct{
                     unsigned int   randseries;
                   HGPU_precision   precision;
                     unsigned int   instances;
                     unsigned int   samples;
                     unsigned int   id_buffer_input_seeds;
                     unsigned int   id_buffer_seeds;
                     unsigned int   id_buffer_randoms;
                     unsigned int   id_kernel_produce;
    } HGPU_PRNG_parameters;

    typedef struct{ 
                       const char*  name;                 // PRNG name
                              int   bitness;              // PRNG bitness
            HGPU_PRNG_output_type   output_type;          // PRNG output type (uint/float/double)
                unsigned long int   min_uint_value;       // minimal unsigned int value, which may be produced by PRNG (0, if floating point PRNG)
                unsigned long int   max_uint_value;       // maximal unsigned int value, which may be produced by PRNG (0, if floating point PRNG)
                           double   min_double_value;     // minimal double value, which may be produced by PRNG
                           double   max_double_value;     // maximal double value, which may be produced by PRNG
                           double   k_value;              // factor for 2uint->double or 2float->double converting (0 if double precision PRNG)
                           size_t   state_size;           // size of PRNG state
                             void   (*init)(void* state, unsigned int randseries);      // PRNG initialization
                             void   (*parameter_init)(void* state,HGPU_parameter** parameters); // PRNG parameters initialization by (HGPU_parameter) array (NULL if absent)
                     unsigned int   (*CPU_produce_one_uint)(void* state);               // produce one unsigned integer on CPU PRNG subroutine (NULL if absent)
                           double   (*CPU_produce_one_double)(void* state);             // produce one unsigned integer on CPU PRNG subroutine
                             void   (*GPU_init)(HGPU_GPU_context* context,void* state,HGPU_PRNG_parameters* PRNG_parameters); // GPU init for initial seed table
                             char*  (*GPU_options)(HGPU_GPU_context* context,void* state,HGPU_PRNG_parameters* PRNG_parameters); // setup additional OpenCL compilation options
                       const char*  prng_src;             // PRNG src
                       const char*  init_kernel;          // PRNG init kernel name
                       const char*  production_kernel;    // PRNG production kernel name
    } HGPU_PRNG_description;

    typedef struct{
      const HGPU_PRNG_description*  prng;
                             void*  state;
             HGPU_PRNG_parameters*  parameters;
    } HGPU_PRNG;

    typedef struct {
        unsigned int x;
        unsigned int y;
        unsigned int z;
        unsigned int t;
    } HGPU_PRNG_internal_state_t;


    extern const HGPU_PRNG_description*  HGPU_PRNG_XOR128;
    extern const HGPU_PRNG_description*  HGPU_PRNG_CONSTANT;
    extern const HGPU_PRNG_description*  HGPU_PRNG_XOR7;
    extern const HGPU_PRNG_description*  HGPU_PRNG_RANECU;
    extern const HGPU_PRNG_description*  HGPU_PRNG_RANMAR;
    extern const HGPU_PRNG_description*  HGPU_PRNG_RANLUX;
    extern const HGPU_PRNG_description*  HGPU_PRNG_PM;
    extern const HGPU_PRNG_description*  HGPU_PRNG_MRG32K3A;
    //  extern const PRNG_description* PRNG_...;
    //  add here other PRNG implementations

      const HGPU_PRNG_description** HGPU_PRNG_descriptions_init(void);
extern const HGPU_PRNG_description* HGPU_PRNG_default;
              extern unsigned int   HGPU_PRNG_default_randseries;
            extern HGPU_precision   HGPU_PRNG_default_precision;
              extern unsigned int   HGPU_PRNG_default_instances;
              extern unsigned int   HGPU_PRNG_default_samples;

    // functions and procedures __________________________________________________________________________________________
                             void   HGPU_PRNG_srand(unsigned int randseries);
                     unsigned int   HGPU_PRNG_rand32bit(void);
                           double   HGPU_PRNG_double_get_k(int bitness);
                           double   HGPU_PRNG_double_from_uint(unsigned int rnd1,unsigned int rnd2,unsigned int rnd_min,unsigned int rnd_max,double k);
                           double   HGPU_PRNG_double_from_float(unsigned int rnd1,unsigned int rnd2,double rnd_min,double rnd_max,double k);

                     unsigned int   HGPU_PRNG_init(HGPU_GPU_context* context,HGPU_PRNG* prng);

      const HGPU_PRNG_description** HGPU_PRNG_descriptions_new(void);
      const HGPU_PRNG_description*  HGPU_PRNG_description_get_with_name(const char* prng_name);
      const HGPU_PRNG_description*  HGPU_PRNG_description_get_with_parameters(HGPU_parameter** parameters);

                             void   HGPU_PRNG_set_default_with_parameters(HGPU_parameter** parameters);
                             void   HGPU_PRNG_set_default_prng(const char* prng_name);
                             void   HGPU_PRNG_set_default_randseries(unsigned int randseries);
                             void   HGPU_PRNG_set_default_precision(HGPU_precision precision);
                             void   HGPU_PRNG_set_default_instances(unsigned int instances);
                             void   HGPU_PRNG_set_default_samples(unsigned int samples);

                             void   HGPU_PRNG_set_with_parameters(HGPU_PRNG* prng,HGPU_parameter** parameters);
                             void   HGPU_PRNG_set_randseries(HGPU_PRNG* prng,unsigned int randseries);
                             void   HGPU_PRNG_set_precision(HGPU_PRNG* prng,HGPU_precision precision);
                             void   HGPU_PRNG_set_instances(HGPU_PRNG* prng,unsigned int instances);
                             void   HGPU_PRNG_set_samples(HGPU_PRNG* prng,unsigned int samples);
                             void   HGPU_PRNG_change_samples(HGPU_GPU_context* context,HGPU_PRNG* prng,unsigned int samples);
                     unsigned int   HGPU_PRNG_get_output_type_values(HGPU_PRNG* prng);

                        HGPU_PRNG*  HGPU_PRNG_new(const HGPU_PRNG_description* prng_descr);
                        HGPU_PRNG*  HGPU_PRNG_new_default(void);
                        HGPU_PRNG*  HGPU_PRNG_new_with_parameters(HGPU_parameter** parameters);

                           double   HGPU_PRNG_produce_CPU_one(HGPU_PRNG* prng);
                           double   HGPU_PRNG_produce_CPU_float_one( HGPU_PRNG* prng,unsigned int prns_drop,double prng_left,double prng_right);
                           double   HGPU_PRNG_produce_CPU_double_one(HGPU_PRNG* prng,unsigned int prns_drop,double prng_left,double prng_right);
                           double   HGPU_PRNG_produce_CPU_floatN_one(HGPU_PRNG* prng,unsigned int prns_drop,double prng_left,double prng_right);
                           double   HGPU_PRNG_produce_CPU_uintN_one( HGPU_PRNG* prng,unsigned int prns_drop,double prng_left,double prng_right);

                             void   HGPU_PRNG_produce_CPU(HGPU_PRNG* prng,double** randoms_CPU,unsigned int number_of_prns);
                             void   HGPU_PRNG_produce(HGPU_GPU_context* context,unsigned int prng_kernel_id);

                             void   HGPU_PRNG_free(HGPU_PRNG* prng);
                             void   HGPU_PRNG_and_buffers_free(HGPU_GPU_context* context,HGPU_PRNG* prng);

                           double   HGPU_PRNG_GPU_get_from_buffer(HGPU_GPU_context* context,HGPU_PRNG* prng,unsigned int index);

                             char*  HGPU_PRNG_make_header(HGPU_PRNG* prng);
                             void   HGPU_PRNG_write_results(HGPU_GPU_context* context,HGPU_PRNG* prng,const char* file_path,const char* file_name);
                             void   HGPU_PRNG_write_output_text(HGPU_GPU_context* context,HGPU_PRNG* prng,const char* file_path,const char* file_name);
                             void   HGPU_PRNG_write_output_binary(HGPU_GPU_context* context,HGPU_PRNG* prng,const char* file_path,const char* file_name);

                     unsigned int   HGPU_PRNG_test(HGPU_GPU_context* context,const HGPU_PRNG_description* prng_descr,unsigned int randseries,
                                                   HGPU_precision precision,unsigned int number,double test_value);

                           double   HGPU_PRNG_benchmark(HGPU_GPU_context* context,const HGPU_PRNG_description* prng_descr,HGPU_precision precision);

#ifdef __cplusplus
}
#endif

#endif
