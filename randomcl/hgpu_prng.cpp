/******************************************************************************
 * @file     hgpu_prng.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.1.1
 *
 * @brief    [PRNGCL library]
 *           Pseudo-random number generators for HGPU package
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

// #define HGPU_PRNG_SKIP_CHECK    // skip uniformity checking in double precision

#define HGPU_PRNG_MAX_descriptions   32
#define HGPU_PRNG_MAX_name_length   256
#define HGPU_PRNG_INCLUDE(prng) {                                                   \
            if (i>=HGPU_PRNG_MAX_descriptions) exit(HGPU_ERROR_ARRAY_OUT_OF_BOUND); \
            HGPU_PRNG_descriptions[i++] = prng;                                     \
        };

const HGPU_PRNG_description*  HGPU_PRNG_descriptions[HGPU_PRNG_MAX_descriptions];
                        int   HGPU_PRNG_descriptions_number = 0;

 HGPU_PRNG_internal_state_t   HGPU_PRNG_internal_state = {1, 12345, 31415926, 3};

void
HGPU_PRNG_srand(unsigned int randseries){
    srand(randseries);
    HGPU_PRNG_internal_state.x = randseries;
    HGPU_PRNG_internal_state.y = 12345;
    HGPU_PRNG_internal_state.z = 31415926;
    HGPU_PRNG_internal_state.t = 3;
    unsigned int z = 1;
    for (int i=0; i<200; i++) z |= HGPU_PRNG_rand32bit();
}

unsigned int
HGPU_PRNG_rand32bit(void){
    unsigned long t=(HGPU_PRNG_internal_state.x^(HGPU_PRNG_internal_state.x<<11));

	HGPU_PRNG_internal_state.x = HGPU_PRNG_internal_state.y;
	HGPU_PRNG_internal_state.y = HGPU_PRNG_internal_state.z;
	HGPU_PRNG_internal_state.z = HGPU_PRNG_internal_state.t;
	HGPU_PRNG_internal_state.t = (HGPU_PRNG_internal_state.t^(HGPU_PRNG_internal_state.t>>19))^(t^(t>>8));

    return HGPU_PRNG_internal_state.t;
}

inline double
trunc(double x){
	double z;
	modf(x,&z);
	return z;
}

double
HGPU_PRNG_double_from_uint(unsigned int rnd1,unsigned int rnd2,unsigned int rnd_min,unsigned int rnd_max,double k){
    return (rnd1 + k * rnd2 - (rnd_min + k * rnd_max)) / ((rnd_max - rnd_min) * (1.0 - k));
}

double
HGPU_PRNG_double_from_float(float rnd1,float rnd2,double rnd_min,double rnd_max,double k){
    double a1 = (rnd_max - rnd_min);
    double a2 = trunc((a1-2.0*k) / k) + a1;
    return (trunc((rnd1-rnd_min-k) / k) + rnd2 - rnd_min) * (1.0-k*k) / a2;
//    double a1 = (rnd_max - rnd_min);
//    double a2 = k * a1;
//    double a3 = a1 * (1.0 - k);
//    double a4 = -(rnd_min / a1 + rnd_max) * k - rnd_min;
//    return (trunc((rnd1 - rnd_min) / a2) + rnd2 / a1) * k / a3 + a4 / a3;
}

double
HGPU_PRNG_double_get_k(int bitness){
    double result = 1.0/pow(2.0,bitness);
    return result;
}

const HGPU_PRNG_description**
HGPU_PRNG_descriptions_new(void){ 
    int i = 0;
    HGPU_PRNG_INCLUDE(HGPU_PRNG_XOR128);
    HGPU_PRNG_INCLUDE(HGPU_PRNG_CONSTANT);
    HGPU_PRNG_INCLUDE(HGPU_PRNG_XOR7);
    HGPU_PRNG_INCLUDE(HGPU_PRNG_RANECU);
    HGPU_PRNG_INCLUDE(HGPU_PRNG_RANMAR);
    HGPU_PRNG_INCLUDE(HGPU_PRNG_RANLUX);
    HGPU_PRNG_INCLUDE(HGPU_PRNG_PM);
    HGPU_PRNG_INCLUDE(HGPU_PRNG_MRG32K3A);
    HGPU_PRNG_INCLUDE(NULL);
    HGPU_PRNG_descriptions_number = (i-1);
    return HGPU_PRNG_descriptions;
}

const HGPU_PRNG_description*
HGPU_PRNG_description_get_with_parameters(HGPU_parameter** parameters){
    const HGPU_PRNG_description* description = NULL;
    HGPU_PRNG_default_randseries = 0;
    HGPU_parameter* parameter_prng = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PRNG);
    if (parameter_prng && (parameter_prng->value_text))
        description = HGPU_PRNG_description_get_with_name(parameter_prng->value_text);
    else
        description = HGPU_PRNG_default;
    HGPU_PRNG_set_default_with_parameters(parameters);

    return description;
}

const HGPU_PRNG_description*
HGPU_PRNG_description_get_with_name(const char* prng_name){
    const HGPU_PRNG_description* description = NULL;
    if (prng_name) {
        const HGPU_PRNG_description** prng_all = HGPU_PRNG_descriptions_new();

        int i=0;
        while ((i<HGPU_PRNG_descriptions_number)&&(!description)){
            if(!strcmp(prng_all[i]->name,prng_name)) description = prng_all[i];
            i++;
        }

        if (!description){
            fprintf(stderr,"Unknown PRNG: %s\n",prng_name);
            fprintf(stderr,"Please, use one of the following PRNGs:\n");
            for (int j=0; j<i; j++) fprintf(stderr,"%s\n",prng_all[j]->name);
            fprintf(stderr,"\n");
            HGPU_error_message(HGPU_ERROR_UNKNOWN_PARAMETER,"unknown PRNG");
        }
    } else {
        description = HGPU_PRNG_description_get_with_parameters(NULL);
    }

    HGPU_PRNG_default = description;

    return description;
}

HGPU_PRNG*
HGPU_PRNG_new(const HGPU_PRNG_description* prng_descr){
    HGPU_PRNG* prng = (HGPU_PRNG*) calloc(1,sizeof(HGPU_PRNG));
    if (!prng)
        HGPU_error_message(HGPU_ERROR_NO_MEMORY,"could not allocate memory for PRNG_description");

    prng->state = calloc(1,prng_descr->state_size);
    if (!prng->state) {
        free(prng);
        HGPU_error_message(HGPU_ERROR_NO_MEMORY,"could not allocate memory for PRNG_state");
    }
    prng->prng = prng_descr;
    prng->parameters = (HGPU_PRNG_parameters*) calloc(1,sizeof(HGPU_PRNG_parameters));
    prng->parameters->id_buffer_input_seeds = HGPU_GPU_MAX_BUFFERS;
    prng->parameters->id_buffer_seeds       = HGPU_GPU_MAX_BUFFERS;
    prng->parameters->id_buffer_randoms     = HGPU_GPU_MAX_BUFFERS;
    prng->parameters->id_kernel_produce     = HGPU_GPU_MAX_KERNELS;

    HGPU_PRNG_set_samples(   prng,HGPU_PRNG_default_samples);
    HGPU_PRNG_set_instances( prng,HGPU_PRNG_default_instances);
    HGPU_PRNG_set_precision( prng,HGPU_PRNG_default_precision);
    HGPU_PRNG_set_randseries(prng,HGPU_PRNG_default_randseries);

    return prng;
}

HGPU_PRNG*
HGPU_PRNG_new_default(void){
    HGPU_PRNG* prng = HGPU_PRNG_new(HGPU_PRNG_default);
        HGPU_PRNG_set_precision(prng,HGPU_PRNG_default_precision);
        HGPU_PRNG_set_instances(prng,HGPU_PRNG_default_instances);
        HGPU_PRNG_set_samples(prng,HGPU_PRNG_default_samples);
        HGPU_PRNG_set_randseries(prng,HGPU_PRNG_default_randseries);

    return prng;
}

HGPU_PRNG*
HGPU_PRNG_new_with_parameters(HGPU_parameter** parameters){
    HGPU_PRNG* prng = HGPU_PRNG_new(HGPU_PRNG_description_get_with_parameters(parameters));
    if (prng->prng->parameter_init) (prng->prng->parameter_init)(prng->state,parameters);
    return prng;
}

void
HGPU_PRNG_free(HGPU_PRNG* prng){
    if(!prng) return;
    free(prng->state);
    free(prng->parameters);
    free(prng);
}

void
HGPU_PRNG_and_buffers_free(HGPU_GPU_context* context,HGPU_PRNG* prng){
    // release all buffers
    HGPU_GPU_context_buffer_release(context,prng->parameters->id_buffer_input_seeds);
    HGPU_GPU_context_buffer_release(context,prng->parameters->id_buffer_seeds);
    HGPU_GPU_context_buffer_release(context,prng->parameters->id_buffer_randoms);

    HGPU_PRNG_free(prng);
}


void
HGPU_PRNG_set_default_with_parameters(HGPU_parameter** parameters){
    if (!parameters) return;
    HGPU_parameter* parameter_randseries = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PRNG_RANDSERIES);
    if (parameter_randseries && (parameter_randseries->value_text)) HGPU_PRNG_set_default_randseries(parameter_randseries->value_integer);

    HGPU_precision precision = HGPU_parameters_get_precision(parameters,(char*) HGPU_PARAMETER_PRNG_PRECISION);
    if (precision) HGPU_PRNG_set_default_precision(precision);

    HGPU_parameter* parameter_samples    = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PRNG_SAMPLES);
    if (parameter_samples && (parameter_samples->value_text)) HGPU_PRNG_set_default_samples(parameter_samples->value_integer);

    HGPU_parameter* parameter_instances  = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PRNG_INSTANCES);
    if (parameter_instances && (parameter_instances->value_text)) HGPU_PRNG_set_default_instances(parameter_instances->value_integer);
}

void
HGPU_PRNG_set_default_precision(HGPU_precision precision){
    HGPU_PRNG_default_precision = precision;
}

void
HGPU_PRNG_set_default_randseries(unsigned int randseries){
    HGPU_PRNG_default_randseries = randseries;
}

void
HGPU_PRNG_set_default_instances(unsigned int instances){
    HGPU_PRNG_default_instances = instances;
}

void
HGPU_PRNG_set_default_samples(unsigned int samples){
    HGPU_PRNG_default_samples = samples;
}

void
HGPU_PRNG_set_default_prng(const char* prng_name){
    HGPU_PRNG_default = NULL;
    HGPU_PRNG_default_randseries = 0;
    if (prng_name) {
        const HGPU_PRNG_description** prng_all = HGPU_PRNG_descriptions_new();

        int i=0;
        while ((i<HGPU_PRNG_descriptions_number)&&(!HGPU_PRNG_default)){
            if(!strcmp(prng_all[i]->name,prng_name)) HGPU_PRNG_default = prng_all[i];
            i++;
        }

        if (!HGPU_PRNG_default){
            fprintf(stderr,"Unknown PRNG: %s\n",prng_name);
            fprintf(stderr,"Please, use one of the following PRNGs:\n");
            for (int j=0; j<i; j++) fprintf(stderr,"%s\n",prng_all[j]->name);
            fprintf(stderr,"\n");
            HGPU_error_message(HGPU_ERROR_UNKNOWN_PARAMETER,"unknown PRNG");
        }
    } else {
        HGPU_PRNG_default = HGPU_PRNG_description_get_with_parameters(NULL);
    }
}

void
HGPU_PRNG_set_precision(HGPU_PRNG* prng,HGPU_precision precision){
    prng->parameters->precision = precision;
}

void
HGPU_PRNG_set_randseries(HGPU_PRNG* prng,unsigned int randseries){
    prng->parameters->randseries = randseries;

    // reinit PRNG
    unsigned int srandtime = (unsigned int) time(NULL);
    if (!prng->parameters->randseries) prng->parameters->randseries = srandtime; // set time-dependent RANDOM seed (if randseries=0)
    (prng->prng->init)(prng->state,prng->parameters->randseries);                // CPU seed table initialization
}

void
HGPU_PRNG_set_instances(HGPU_PRNG* prng,unsigned int instances){
    prng->parameters->instances = instances;
}

void
HGPU_PRNG_set_samples(HGPU_PRNG* prng,unsigned int samples){
    prng->parameters->samples = samples;
}

void
HGPU_PRNG_change_samples(HGPU_GPU_context* context,HGPU_PRNG* prng,unsigned int samples){
    unsigned int kernel_id = prng->parameters->id_kernel_produce;
    if (kernel_id<HGPU_GPU_MAX_KERNELS) {
        HGPU_PRNG_set_samples(prng,samples);
        unsigned int number_of_args = HGPU_GPU_context_kernel_get_args_number(context,kernel_id);
        if (number_of_args)
            HGPU_GPU_context_kernel_bind_constant_by_argument_id(context,kernel_id,&prng->parameters->samples,sizeof(prng->parameters->samples),(number_of_args-1));
    }
}   

void
HGPU_PRNG_set_with_parameters(HGPU_PRNG* prng,HGPU_parameter** parameters){
    if ((!prng) || (!parameters)) return;

    HGPU_precision precision = HGPU_parameters_get_precision(parameters,(char*) HGPU_PARAMETER_PRNG_PRECISION);
    if (precision) HGPU_PRNG_set_precision(prng,precision);

    HGPU_parameter* parameter_samples    = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PRNG_SAMPLES);
    if (parameter_samples && (parameter_samples->value_text)) HGPU_PRNG_set_samples(prng,parameter_samples->value_integer);

    HGPU_parameter* parameter_instances  = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PRNG_INSTANCES);
    if (parameter_instances && (parameter_instances->value_text)) HGPU_PRNG_set_instances(prng,parameter_instances->value_integer);

    HGPU_parameter* parameter_randseries = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PRNG_RANDSERIES);
    if (parameter_randseries && (parameter_randseries->value_text)) HGPU_PRNG_set_randseries(prng,parameter_randseries->value_integer);

    if (prng->prng->parameter_init) (prng->prng->parameter_init)(prng->state,parameters);
}

unsigned int
HGPU_PRNG_init(HGPU_GPU_context* context,HGPU_PRNG* prng){
    unsigned int kernel_id           = HGPU_GPU_MAX_KERNELS;
    unsigned int PRNG_input_seeds_id = HGPU_GPU_MAX_BUFFERS;
    unsigned int PRNG_seeds_id       = HGPU_GPU_MAX_BUFFERS;
    unsigned int PRNG_randoms_id     = HGPU_GPU_MAX_BUFFERS;
    if (!context) {
        HGPU_GPU_error_note(HGPU_ERROR_BAD_CONTEXT,"context is not initialized");
        // or try to initialize context
        return kernel_id;
    }
    if (!prng) {
        HGPU_GPU_error_note(HGPU_ERROR_BAD_PRNG,"PRNG is not initialized");
        // or try to initialize PRNG
        return kernel_id;
    }
    if (!prng->prng->prng_src) {
        HGPU_GPU_error_note(HGPU_ERROR_BAD_PRNG_INIT,"bad PRNG initialization");
        // or try to reinitialize PRNG
        return kernel_id;
    }

    // calulate instances
    if (!prng->parameters->instances) {
        prng->parameters->instances = HGPU_GPU_device_get_max_memory_width(context->device);
        prng->parameters->samples   = 1 + (prng->parameters->samples - 1) / prng->parameters->instances;
    }

    char* prng_src = HGPU_io_file_read_with_path(HGPU_io_path_root,prng->prng->prng_src);
    char options[HGPU_GPU_MAX_OPTIONS_LENGTH];
    int  j2 = sprintf_s(options,HGPU_GPU_MAX_OPTIONS_LENGTH,"-I %s%s",HGPU_io_path_root,PRNGCL_ROOT_PATH);
    if (prng->parameters->precision==HGPU_precision_double) j2 += sprintf_s(options+j2,HGPU_GPU_MAX_OPTIONS_LENGTH-j2," -D PRNG_PRECISION=2");
#ifdef HGPU_PRNG_SKIP_CHECK
    j2 += sprintf_s(options+j2,HGPU_GPU_MAX_OPTIONS_LENGTH-j2," -D PRNG_SKIP_CHECK");
#endif

    // additional particular options of PRNG
    // include additional options for opencl
    if (prng->prng->GPU_options) {
        char* add_options = (*prng->prng->GPU_options)(context,prng->state,prng->parameters);
        if ((add_options) && (strlen(add_options)>0)) j2 += sprintf_s(options+j2,HGPU_GPU_MAX_OPTIONS_LENGTH-j2," %s",add_options);
        free(add_options);
    }

    // compile opencl program
    cl_program prg = HGPU_GPU_program_with_options_new(prng_src,options,context);

    // make memory objects: prepare seed tables (input and working)
    // perform input seed table by PRNG subroutine
    (*prng->prng->GPU_init)(context,prng->state,prng->parameters);
    PRNG_input_seeds_id = prng->parameters->id_buffer_input_seeds;
    PRNG_seeds_id       = prng->parameters->id_buffer_seeds;
    PRNG_randoms_id     = prng->parameters->id_buffer_randoms;

    const size_t global_size[]  = {prng->parameters->instances}; // global_size

    if ((PRNG_input_seeds_id<HGPU_GPU_MAX_BUFFERS) && (PRNG_seeds_id<HGPU_GPU_MAX_BUFFERS) && (prng->prng->init_kernel)) {
        // make seed preparation kernel
        unsigned int kernel_seed_id = HGPU_GPU_context_kernel_init(context,prg,prng->prng->init_kernel,1,global_size,NULL);
                     HGPU_GPU_context_kernel_bind_buffer(context,kernel_seed_id,PRNG_input_seeds_id);
                     HGPU_GPU_context_kernel_bind_buffer(context,kernel_seed_id,PRNG_seeds_id);

        // run seed preparation kernel
        HGPU_GPU_context_kernel_run(context,kernel_seed_id);
    }

    // release input seed table
    if (PRNG_input_seeds_id<HGPU_GPU_MAX_BUFFERS)
        HGPU_GPU_context_buffer_release(context,PRNG_input_seeds_id);

    // make working kernel
    kernel_id   = HGPU_GPU_context_kernel_init(context,prg,prng->prng->production_kernel, 1,global_size,NULL);
    if (PRNG_seeds_id<HGPU_GPU_MAX_BUFFERS)   HGPU_GPU_context_kernel_bind_buffer(context,kernel_id,PRNG_seeds_id);
    if (PRNG_randoms_id<HGPU_GPU_MAX_BUFFERS) HGPU_GPU_context_kernel_bind_buffer(context,kernel_id,PRNG_randoms_id);
    HGPU_GPU_context_kernel_bind_constant(context,kernel_id,&prng->parameters->samples,sizeof(prng->parameters->samples));

    prng->parameters->id_buffer_randoms = PRNG_randoms_id;
    prng->parameters->id_kernel_produce = kernel_id;

    free(prng_src);
    return kernel_id;
}

void
HGPU_PRNG_produce(HGPU_GPU_context* context,unsigned int prng_kernel_id){
    HGPU_GPU_context_kernel_run(context,prng_kernel_id);
}

double
HGPU_PRNG_produce_CPU_float_one(HGPU_PRNG* prng,unsigned int){
    return (double) ((float) prng->prng->CPU_produce_one_double(prng->state));
}

double
HGPU_PRNG_produce_CPU_double_one(HGPU_PRNG* prng,unsigned int){
    return prng->prng->CPU_produce_one_double(prng->state);
}

double
HGPU_PRNG_produce_CPU_floatN_one(HGPU_PRNG* prng,unsigned int prns_drop){
    double result = 0.0;
    double prng_k = prng->prng->k_value;
    float rnd1, rnd2;
    rnd1 = (float) (prng->prng->CPU_produce_one_double)(prng->state);
#ifndef HGPU_PRNG_SKIP_CHECK
    bool flag = true;
    while (flag) {
#endif
        rnd2 = (float) (prng->prng->CPU_produce_one_double)(prng->state);

#ifndef HGPU_PRNG_SKIP_CHECK
        if ((rnd1 > prng->prng->min_double_value) && (rnd1 < prng->prng->max_double_value))
            flag = false;
        else {
            // pass prns_drop prns
            for (unsigned int i=0;i<prns_drop;i++) rnd1 = (float) (prng->prng->CPU_produce_one_double)(prng->state);
            rnd1 = rnd2;
        }
    }
#endif
    result = HGPU_PRNG_double_from_float(rnd1,rnd2,prng->prng->min_double_value,prng->prng->max_double_value,prng_k);
    return result;
}

double
HGPU_PRNG_produce_CPU_uintN_one(HGPU_PRNG* prng,unsigned int prns_drop){
    double result = 0.0;
    double prng_k = prng->prng->k_value;
    unsigned int rnd1, rnd2;
    rnd1 = (prng->prng->CPU_produce_one_uint)(prng->state);
#ifndef HGPU_PRNG_SKIP_CHECK
    bool flag = true;
    while (flag) {
#endif
        rnd2 = (prng->prng->CPU_produce_one_uint)(prng->state);

#ifndef HGPU_PRNG_SKIP_CHECK
        if ((rnd1 > prng->prng->min_uint_value) && (rnd1 < prng->prng->max_uint_value))
            flag = false;
        else {
            // pass prns_drop prns
            for (unsigned int i=0;i<prns_drop;i++) rnd1 = (prng->prng->CPU_produce_one_uint)(prng->state);
            rnd1 = rnd2;
        }
    }
#endif
    result = HGPU_PRNG_double_from_uint(rnd1,rnd2,prng->prng->min_uint_value,prng->prng->max_uint_value,prng_k);
    return result;
}


double
HGPU_PRNG_produce_CPU_one(HGPU_PRNG* prng){
    double result    = 0.0;
    if (!prng){
        HGPU_GPU_error_note(HGPU_ERROR_BAD_PRNG,"PRNG is not initialized");
        return result;
    }
    double (*prng_produce_one)(HGPU_PRNG*,unsigned int);
    double prng_k     = prng->prng->k_value;
    unsigned int prng_drop = 0;
    prng_produce_one = NULL;

    if (prng->parameters->precision==HGPU_precision_double) {
        if ((prng->prng->output_type == HGPU_PRNG_output_type_double) || (!prng_k)) {
            prng_produce_one = &HGPU_PRNG_produce_CPU_double_one;
        } else {
            if (((prng->prng->output_type == HGPU_PRNG_output_type_uint)  ||
                 (prng->prng->output_type == HGPU_PRNG_output_type_uint4) ||
                 (prng->prng->output_type == HGPU_PRNG_output_type_uint4by1)) && (prng->prng->CPU_produce_one_uint)) {
                 prng_drop  = HGPU_PRNG_get_output_type_values(prng) - 1;
                 if (prng->prng->output_type == HGPU_PRNG_output_type_uint4by1) prng_drop = 0;
                 prng_produce_one = &HGPU_PRNG_produce_CPU_uintN_one;
            } else {
                 prng_drop  = HGPU_PRNG_get_output_type_values(prng) - 1;
                 if (prng->prng->output_type == HGPU_PRNG_output_type_float4by1) prng_drop = 0;
                 prng_produce_one = &HGPU_PRNG_produce_CPU_floatN_one;
            }
        }
    } else {
        prng_produce_one = &HGPU_PRNG_produce_CPU_float_one;
    }

    if (prng_produce_one)
        result = (*prng_produce_one)(prng,prng_drop);

    return result;
}

void
HGPU_PRNG_produce_CPU(HGPU_PRNG* prng,double** randoms_CPU,unsigned int number_of_prns){
    if ((!prng) || (!randoms_CPU) || (!number_of_prns)) return;
    double* result = (*randoms_CPU);
    if (!result) {
        result = (double*) calloc(number_of_prns,sizeof(double));
        if (!result)
            HGPU_error_message(HGPU_ERROR_NO_MEMORY,"could not allocate memory for PRNG_CPU_output");
    }

    double (*prng_produce_one)(HGPU_PRNG*,unsigned int);
    double prng_k    = prng->prng->k_value;
    unsigned int prng_drop = 0;
    prng_produce_one = NULL;

    if (prng->parameters->precision==HGPU_precision_double) {
        if ((prng->prng->output_type == HGPU_PRNG_output_type_double) || (!prng_k)) {
            prng_produce_one = &HGPU_PRNG_produce_CPU_double_one;
        } else {
            if (((prng->prng->output_type == HGPU_PRNG_output_type_uint)  ||
                 (prng->prng->output_type == HGPU_PRNG_output_type_uint4) ||
                 (prng->prng->output_type == HGPU_PRNG_output_type_uint4by1)) && (prng->prng->CPU_produce_one_uint)) {
                 prng_drop  = HGPU_PRNG_get_output_type_values(prng) - 1;
                 if (prng->prng->output_type == HGPU_PRNG_output_type_uint4by1) prng_drop = 0;
                 prng_produce_one = &HGPU_PRNG_produce_CPU_uintN_one;
            } else {
                 prng_drop  = HGPU_PRNG_get_output_type_values(prng) - 1;
                 if (prng->prng->output_type == HGPU_PRNG_output_type_float4by1) prng_drop = 0;
                 prng_produce_one = &HGPU_PRNG_produce_CPU_floatN_one;
            }
        }
    } else {
        prng_produce_one = &HGPU_PRNG_produce_CPU_float_one;
    }

    if (prng_produce_one)
        for (unsigned int i=0; i<number_of_prns; i++)
            result[i] = (*prng_produce_one)(prng,prng_drop);

    (*randoms_CPU) = result;
}

char*
HGPU_PRNG_make_header(HGPU_PRNG* prng){
    char* result = NULL;
    if (!prng) return result;

    int j = 0;
    char buffer[HGPU_GPU_MAX_OPTIONS_LENGTH];
	j += sprintf_s(buffer+j,HGPU_GPU_MAX_OPTIONS_LENGTH-j," PRNGCL library ver.%u.%u.%u\n",PRNGCL_VERSION_MAJOR,PRNGCL_VERSION_MINOR,PRNGCL_VERSION_MICRO);
	j += sprintf_s(buffer+j,HGPU_GPU_MAX_OPTIONS_LENGTH-j," ***************************************************\n");
    j += sprintf_s(buffer+j,HGPU_GPU_MAX_OPTIONS_LENGTH-j," PRNG                        : %s\n",prng->prng->name);
    j += sprintf_s(buffer+j,HGPU_GPU_MAX_OPTIONS_LENGTH-j," PRNG precision              : %s\n",HGPU_convert_precision_to_str(prng->parameters->precision));
    j += sprintf_s(buffer+j,HGPU_GPU_MAX_OPTIONS_LENGTH-j," PRNG randseries             : %u\n",prng->parameters->randseries);
    j += sprintf_s(buffer+j,HGPU_GPU_MAX_OPTIONS_LENGTH-j," PRNG instances              : %u\n",prng->parameters->instances);
    j += sprintf_s(buffer+j,HGPU_GPU_MAX_OPTIONS_LENGTH-j," PRNG samples                : %u\n",prng->parameters->samples);
	j += sprintf_s(buffer+j,HGPU_GPU_MAX_OPTIONS_LENGTH-j," ***************************************************\n");

    size_t result_length = strlen(buffer) + 1;
    result = (char*) calloc(result_length,sizeof(char));
    j = sprintf_s(result,result_length,"%s",buffer);
    return result;
}

double
HGPU_PRNG_GPU_get_from_buffer(HGPU_GPU_context* context,HGPU_PRNG* prng,unsigned int index){
    double result = 0.0;
    if ((!context) || (!prng)) return result;
    void* results_ptr = HGPU_GPU_context_buffer_get_mapped(context,prng->parameters->id_buffer_randoms);
    cl_float4*  pointer_to_randoms = NULL;
    cl_double4* pointer_to_randoms_double = NULL;
    unsigned int output_type_vals = HGPU_PRNG_get_output_type_values(prng);
    unsigned int index_s = 0;
    unsigned int index_h = 0;
    unsigned int index_o = 0;
    if (prng->parameters->precision==HGPU_precision_double)
        pointer_to_randoms_double = (cl_double4*) results_ptr;
    else
        pointer_to_randoms        = (cl_float4*) results_ptr;
    if ((pointer_to_randoms) || (pointer_to_randoms_double)) {
        unsigned int offset = prng->parameters->instances;
        if (output_type_vals == 1) {
            index_s = (index / offset) & 3;
            index_h = index % offset;
            index_o = (index / (4 * offset));
        }
        if (output_type_vals == 4) {
            index_s = (index & 3);
            index_h = ((index >> 2) % offset) ;
            index_o = ((index >> 2) / offset);
        }
        if (prng->parameters->precision==HGPU_precision_double)
            result = pointer_to_randoms_double[index_o + offset*index_h].s[index_s];
        else
            result = pointer_to_randoms[index_o + offset*index_h].s[index_s];
    }
    return result;
}

unsigned int
HGPU_PRNG_get_output_type_values(HGPU_PRNG* prng){
    unsigned int result = 1;
    if ((prng->prng->output_type == HGPU_PRNG_output_type_uint   ) ||
        (prng->prng->output_type == HGPU_PRNG_output_type_float  ) ||
        (prng->prng->output_type == HGPU_PRNG_output_type_double))
        result = 1;
    if ((prng->prng->output_type == HGPU_PRNG_output_type_uint4    ) ||
        (prng->prng->output_type == HGPU_PRNG_output_type_uint4by1 ) ||
        (prng->prng->output_type == HGPU_PRNG_output_type_float4   ) ||
        (prng->prng->output_type == HGPU_PRNG_output_type_float4by1) ||
        (prng->prng->output_type == HGPU_PRNG_output_type_double4) )
        result = 4;
    return result;
}

void
HGPU_PRNG_write_results(HGPU_GPU_context* context,HGPU_PRNG* prng,const char* file_path,const char* file_name){
    FILE *stream;

    char* buffer = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    if (!buffer) HGPU_error(HGPU_ERROR_NO_MEMORY);
    HGPU_io_path_join_filename(&buffer,HGPU_FILENAME_MAX,file_path,file_name);

	fopen_s(&stream,buffer,"w+");
	if(stream){
        char* header = HGPU_PRNG_make_header(prng);
        fprintf(stream,"%s",header);
        unsigned int output_type_vals = HGPU_PRNG_get_output_type_values(prng);
        unsigned int number_of_prns = prng->parameters->samples * output_type_vals;
        for (unsigned int i=0; i<number_of_prns; i++)
            fprintf(stream,"[%5u]   % .20e\n",i,HGPU_PRNG_GPU_get_from_buffer(context,prng,i));
        if (fclose(stream)) HGPU_error_note(HGPU_ERROR_FILE_NOT_CLOSED,"the file was not closed");
        free(header);
    }
    free(buffer);
}

void
HGPU_PRNG_write_output_text(HGPU_GPU_context* context,HGPU_PRNG* prng,const char* file_path,const char* file_name){
    FILE *stream;

    char* buffer = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    if (!buffer) HGPU_error(HGPU_ERROR_NO_MEMORY);
    HGPU_io_path_join_filename(&buffer,HGPU_FILENAME_MAX,file_path,file_name);

	fopen_s(&stream,buffer,"w+");
	if(stream){
        unsigned int output_type_vals = HGPU_PRNG_get_output_type_values(prng);
        unsigned int number_of_prns = prng->parameters->instances * prng->parameters->samples * output_type_vals;
        for (unsigned int i=0; i<(number_of_prns>>2); i++){
            for (unsigned int j=0; j<MIN(4,(number_of_prns-i*4)); j++)
                fprintf(stream,"% .20e ",HGPU_PRNG_GPU_get_from_buffer(context,prng,i*4+j));
            fprintf(stream,"\n");
        }
        if (fclose(stream)) HGPU_error_note(HGPU_ERROR_FILE_NOT_CLOSED,"the file was not closed");
    }
    free(buffer);
}

unsigned int
HGPU_PRNG_test(HGPU_GPU_context* context,HGPU_parameter** parameters,const HGPU_PRNG_description* prng_descr,unsigned int randseries,
               HGPU_precision precision,unsigned int number,double test_value){
    unsigned int result = 0;

    HGPU_PRNG_set_default_precision(precision);
    HGPU_PRNG_set_default_randseries(randseries);
    HGPU_PRNG_set_default_samples(number);

    HGPU_parameter* parameter_instances  = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PRNG_INSTANCES);
    if (parameter_instances && (parameter_instances->value_text)) HGPU_PRNG_set_default_instances(parameter_instances->value_integer);

    HGPU_PRNG* prng = HGPU_PRNG_new(prng_descr);
    if (randseries) HGPU_PRNG_set_randseries(prng,randseries);
    double* CPU_results = (double*) calloc(number+1,sizeof(double));
    if (!CPU_results)
        HGPU_error_message(HGPU_ERROR_NO_MEMORY,"could not allocate memory for results");

    unsigned int prng_id = HGPU_PRNG_init(context,prng);
    HGPU_PRNG_produce_CPU(prng,&CPU_results,number);

    double CPU_value = CPU_results[number-1];

    HGPU_PRNG_produce(context,prng_id);
    unsigned int output_type_vals = HGPU_PRNG_get_output_type_values(prng);
    unsigned int number_of_prns = prng->parameters->samples * output_type_vals;
    double GPU_prn, CPU_prn;
    for (size_t i=0;i<number_of_prns;i++) {
        if (precision==HGPU_precision_single) {
            GPU_prn = ((float) HGPU_PRNG_GPU_get_from_buffer(context,prng,i));
            CPU_prn = ((float) CPU_results[i]);
        } else {
            GPU_prn = HGPU_PRNG_GPU_get_from_buffer(context,prng,i);
            CPU_prn = CPU_results[i];
        }
        
        if (GPU_prn!=CPU_prn) {
            printf("[%u]:\t  GPU: % .20e      CPU: % .20e     difference: % e\n",(unsigned int) i,GPU_prn,CPU_prn,(GPU_prn-CPU_prn));
            result++;
        }
    }

    double test_value_norm = test_value;
    if (prng->parameters->precision==HGPU_precision_single) test_value_norm = (double) ((float) test_value);

    if (test_value_norm!=CPU_value) {
        printf("Expected: % .20e    Generated: % .20e    Difference: % e\n",test_value_norm,CPU_value,(test_value_norm-CPU_value));
        result++;
    }

    printf("%s (%s): ",prng->prng->name,HGPU_convert_precision_to_str(prng->parameters->precision));

    if (result)
        printf("%u test(s) failed!!!\n",result);
    else
        printf("all tests passed\n");

    free(CPU_results);
    HGPU_PRNG_and_buffers_free(context,prng);
    return result;
}


double
HGPU_PRNG_benchmark(HGPU_GPU_context* context,HGPU_parameter** parameters,const HGPU_PRNG_description* prng_descr,HGPU_precision precision){
    double result = 0.0;
    unsigned int max_cycles = HGPU_PRNG_TEST_MAX_PASSES; // HGPU_PRNG_TEST_MAX_PASSES=test maximum number of passes
    double duration = HGPU_PRNG_TEST_MAX_DURATION;       // HGPU_PRNG_TEST_DURATION  =test duration in seconds

    HGPU_PRNG_set_default_precision(precision);
    HGPU_PRNG_set_default_randseries(1);

    HGPU_parameter* parameter_samples    = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PRNG_SAMPLES);
    HGPU_parameter* parameter_instances  = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PRNG_INSTANCES);
    HGPU_parameter* parameter_cycles     = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PRNG_TEST_MAX_PASSES);
    HGPU_parameter* parameter_duration   = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PRNG_TEST_MAX_DURATION);

    if (parameter_cycles   && (parameter_cycles->value_text))   max_cycles = parameter_cycles->value_integer;
    if (parameter_duration && (parameter_duration->value_text)) duration   = parameter_duration->value_double;

    HGPU_PRNG_set_default_with_parameters(parameters);
    HGPU_PRNG* prng = HGPU_PRNG_new(prng_descr);

        unsigned int instances = HGPU_GPU_device_get_max_memory_width(context->device);
        if (parameter_instances && (parameter_instances->value_text)) instances = parameter_instances->value_integer;
        unsigned int output_type_vals = 4;
        unsigned long int alloc_memory = HGPU_GPU_device_get_max_allocation_memory(context->device);
        unsigned int elem_size = (precision==HGPU_precision_single) ? sizeof(cl_float) : sizeof(cl_double);

        unsigned int samples = HGPU_convert_round_to_power_2(((alloc_memory >> 1) - prng->prng->state_size * instances) / 
                               (elem_size * output_type_vals * instances));
        unsigned int samples_best = samples;

        if (parameter_samples && (parameter_samples->value_text))     samples   = parameter_samples->value_integer;

        HGPU_PRNG_set_instances(prng,instances);
        HGPU_PRNG_set_samples(prng,samples);

        unsigned int prng_id = HGPU_PRNG_init(context,prng);
// context->debug_flags.max_workgroup_size = 256;
// HGPU_GPU_context_kernel_limit_max_workgroup_size(context,prng_id);


        double last_start_et = 0.0;
        while (samples) {
            HGPU_PRNG_change_samples(context,prng,samples);

            unsigned int cycles = 0;
            double time_elapsed = 0.0;
            HGPU_timer time_start = HGPU_timer_start();
            while((time_elapsed<duration) && (cycles<max_cycles)) {  
                HGPU_PRNG_produce(context,prng_id);
                cycles++;
                time_elapsed = HGPU_timer_get(time_start);
            }

            double prns = ((double) cycles) * ((double) prng->parameters->samples) * ((double) prng->parameters->instances) * ((double) output_type_vals);
            double productivity = 0.0;

            HGPU_timer_deviation elapsed_time = HGPU_timer_deviation_default;
            HGPU_GPU_kernel* kernel = HGPU_GPU_kernel_get_by_index(context->kernel,prng_id);
            if (kernel) {
                double e_time = 0.0;
                if (context->debug_flags.profiling) {
                    elapsed_time = HGPU_GPU_kernel_get_time_execution(kernel);
                    e_time = elapsed_time.mean - last_start_et;
                } else
                    e_time = HGPU_convert_s_to_ns(time_elapsed);
                productivity = HGPU_convert_B_to_GBS(prns,e_time);
                if (result<productivity) {
                    result = productivity;
                    samples_best = samples;
                }
                last_start_et = elapsed_time.mean;
            }
            printf("Perf: %1.2e (Gsamples/sec) - %e PRNs (%u samples - %u cycles - %u instances) per %e seconds\n",productivity,prns,samples,cycles,prng->parameters->instances,time_elapsed);

            samples = samples >> 1;
        }

    printf("\nPRNG: %s\n",prng->prng->name);
    printf("Precision: %s\n",HGPU_convert_precision_to_str(prng->parameters->precision));
    printf("Instances: %u\n",prng->parameters->instances);
    printf("Best productivity (Gsamples/sec): %e\n",result);
    printf("Best samples: %u\n",samples_best);
    HGPU_PRNG_and_buffers_free(context,prng);
    return result;
}
