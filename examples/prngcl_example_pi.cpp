/******************************************************************************
 * @file     prngcl_example_pi.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.1
 *
 * @brief    [PRNGCL library]
 *           Pseudo-random number generators for HGPU package
 *           example PI calculation
 *
 *
 * @section  LICENSE
 *
 * Copyright (c) 2014 Vadim Demchik
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

#include "prngcl_example_pi.h"
#include "../hgpucl/hgpucl.h"

void
HGPU_GPU_example_pi(int argc, char** argv){
    double result = 0.0;

    HGPU_parameter** parameters_all = HGPU_parameters_get_all(argc,argv);     // get all parameters from commandline, .ini-file and environment (may be empty)
    HGPU_GPU_context* context = HGPU_GPU_context_select_auto(parameters_all); // create new context according to parameters

    printf("********************************************************\n");
    printf(" PRNGCL library v.%u.%u.%u\n",PRNGCL_VERSION_MAJOR,PRNGCL_VERSION_MINOR,PRNGCL_VERSION_MICRO);
    printf(" HGPU core v.%u.%u.%u\n",HGPUCL_VERSION_MAJOR,HGPUCL_VERSION_MINOR,HGPUCL_VERSION_MICRO);
    printf(" Copyright (c) 2014, Vadim Demchik\n\n");
    printf(" [EXAMPLE]:\n");
    printf(" Pi calculation with PRNGCL library\n\n");
    printf("********************************************************\n\n");

    HGPU_GPU_context_print_used_hardware(context); // print using hardware

    const HGPU_PRNG_description* prng_descr = HGPU_PRNG_MRG32K3A;    // PRNG to be used
          HGPU_precision         precision  = HGPU_precision_single; // precision to be used
          unsigned int           randseries = 0;                     // serial number of sequence (0=random, other=determined)
          unsigned int           instances  = HGPU_GPU_device_get_max_memory_width(context->device); // number of PRNG instances
          unsigned int           samples_per_stream = 1024;          // number of PRNs per stream
          unsigned int           passes     = 1000;                  // number of passes for pi calculation

    HGPU_PRNG* prng = HGPU_PRNG_new(prng_descr); // create new PRNG instance
    HGPU_PRNG_set_precision(prng,precision);     // set precision
    HGPU_PRNG_set_randseries(prng,randseries);   // set randseries
    HGPU_PRNG_set_instances(prng,instances);        // set instances
    HGPU_PRNG_set_samples(prng,samples_per_stream); // set samples (if instances=0 then [autoselection] and samples should be whole number of desired PRNs)

    unsigned int prng_id = HGPU_PRNG_init(context,prng); // PRNG initialization

    char* example_src = HGPU_io_file_read_with_path(HGPU_io_path_root,EXAMPLE_SRC); // get .cl-source of example
    char options[HGPU_GPU_MAX_OPTIONS_LENGTH];
    int  j2 = sprintf_s(options,HGPU_GPU_MAX_OPTIONS_LENGTH,"-I %s%s",HGPU_io_path_root,PRNGCL_ROOT_PATH);
    if (prng->parameters->precision==HGPU_precision_double) j2 += sprintf_s(options+j2,HGPU_GPU_MAX_OPTIONS_LENGTH-j2," -D PRNG_PRECISION=2");

    cl_program prg = HGPU_GPU_program_with_options_new(example_src,options,context); // compile opencl program

    // allocated memory objects
    size_t pi_acceptance_size = HGPU_GPU_context_buffer_size_align(context,prng->parameters->instances); // align buffer size
    cl_float* pi_acceptance = (cl_float*) calloc(pi_acceptance_size,sizeof(cl_float));
    if (!pi_acceptance)
        HGPU_error_message(HGPU_ERROR_NO_MEMORY,"could not allocate memory for randoms"); // chech if buffer is allocated

    unsigned int PRNG_randoms_id = prng->parameters->id_buffer_randoms; // get randoms buffer id

    // buffer OpenCL initialization
    unsigned int pi_acceptance_id = HGPU_GPU_context_buffer_init(context,pi_acceptance,HGPU_GPU_buffer_type_io,pi_acceptance_size,sizeof(cl_float));
    HGPU_GPU_context_buffer_set_name(context,pi_acceptance_id,(char*) "PI_Acceptance"); // [optional]: set name to buffer for profiling/debugging

    unsigned int kernel_pi_calculate_id = HGPU_GPU_MAX_KERNELS;
    const size_t global_size[]  = {prng->parameters->instances}; // execution domain
    if ((pi_acceptance_id<HGPU_GPU_MAX_BUFFERS) && (PRNG_randoms_id<HGPU_GPU_MAX_BUFFERS) && (prng->prng->production_kernel)) {
        // bind buffers and number of samples (PRNs) per stream to the kernel
        kernel_pi_calculate_id = HGPU_GPU_context_kernel_init(context,prg,"calculate_pi",1,global_size,NULL); // prepare kernel
                     HGPU_GPU_context_kernel_bind_buffer(context,kernel_pi_calculate_id,PRNG_randoms_id);     // bind buffer with randoms
                     HGPU_GPU_context_kernel_bind_buffer(context,kernel_pi_calculate_id,pi_acceptance_id);    // bind buffer for results
                     HGPU_GPU_context_kernel_bind_constant(context,kernel_pi_calculate_id,&samples_per_stream,sizeof(unsigned int)); // bind samples_per_stream
    }

    for (unsigned int i=0; i<passes; i++) {
        HGPU_PRNG_produce(context,prng_id);  // produce PRNs into randoms array
        HGPU_GPU_context_kernel_run(context,kernel_pi_calculate_id); // run pi calculation kernel
    }

    cl_float* results_ptr = (cl_float*) HGPU_GPU_context_buffer_get_mapped(context,pi_acceptance_id);

    for (unsigned int i=0; i<prng->parameters->instances; i++)
        result += results_ptr[i];

    double prns_used = (4.0*((double) passes)*((double) prng->parameters->instances)*((double) prng->parameters->samples));
        printf("\nPRNG instances: %u\n",prng->parameters->instances);
        printf("PRNG samples:   %u\n",prng->parameters->samples);
        printf("passes:         %u\n",passes);
        printf("PRNs used:      %e\n\n",prns_used);

        printf("PI = %f\n\n",4.0*result/(prns_used/2.0));

    // release pi_acceptance buffer for example
    if (pi_acceptance_id<HGPU_GPU_MAX_BUFFERS)
        HGPU_GPU_context_buffer_release(context,pi_acceptance_id);

    HGPU_PRNG_and_buffers_free(context,prng);    // release PRNG instance

    free(example_src);
    HGPU_GPU_context_delete(&context);           // release context
}
