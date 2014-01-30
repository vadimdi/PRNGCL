/******************************************************************************
 * @file     hgpucl.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           Interface for OpenCL AMD APP & nVidia SDK environment
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

#include "hgpucl.h"

void
HGPU_GPU_test(int argc, char** argv){

    HGPU_parameter** parameters_all = HGPU_parameters_get_all(argc,argv);
    HGPU_GPU_context* context = HGPU_GPU_context_select_auto(parameters_all);

    printf("********************************************************\n");
    printf(" PRNGCL library v.%u.%u.%u\n",PRNGCL_VERSION_MAJOR,PRNGCL_VERSION_MINOR,PRNGCL_VERSION_MICRO);
    printf(" HGPU core v.%u.%u.%u\n",HGPUCL_VERSION_MAJOR,HGPUCL_VERSION_MINOR,HGPUCL_VERSION_MICRO);
    printf(" Copyright (c) 2013, 2014 Vadim Demchik\n");
    printf("********************************************************\n\n");

    HGPU_GPU_context_print_used_hardware(context);

//    HGPU_PRNG_tests(context);
    HGPU_PRNG_benchmarks(context);


    HGPU_GPU_context_delete(&context);
}
