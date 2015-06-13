/******************************************************************************
 * @file     hgpucl_kernel.h
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0
 *
 * @brief    [HGPU library]
 *           Interface for OpenCL AMD APP & nVidia SDK environment
 *           kernel submodule
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

#ifndef HGPUCL_KERNEL_H
#define HGPUCL_KERNEL_H

#include <CL/cl.h>
#include "platform.h"
#include "hgpucl_constants.h"
#include "hgpucl_error.h"
#include "../include/hgpu_string.h"
#include "../include/hgpu_timer.h"


    // structure for kernel
    typedef struct{
                        cl_kernel   kernel;
                              int   argument_id;                   // argument iterator
                     unsigned int   work_dimensions;               // work dimensions
                           size_t*  global_size;                   // global size
                           size_t*  local_size;                    // local size
                // profiling data __________________________
                         cl_event   kernel_event;                  // event for kernel
                         cl_ulong   kernel_start;                  // kernel last start time
                         cl_ulong   kernel_finish;                 // kernel last finish time
                           double   kernel_elapsed_time;           // total kernel execution time (in nanoseconds)
                           double   kernel_elapsed_time_squared;   // total kernel execution time squared (in nanoseconds) - for deviation calculation
                        long long   kernel_number_of_starts;       // total number of kernel starts - for deviation calculation
    } HGPU_GPU_kernel;

                   void   HGPU_GPU_kernels_add(HGPU_GPU_kernel*** kernels,HGPU_GPU_kernel* kernel);
                   void   HGPU_GPU_kernels_delete(HGPU_GPU_kernel*** kernels);
           unsigned int   HGPU_GPU_kernels_get_number(HGPU_GPU_kernel** kernels);

        HGPU_GPU_kernel*  HGPU_GPU_kernel_new_empty(void);
                   void   HGPU_GPU_kernel_delete(HGPU_GPU_kernel** kernel);
           unsigned int   HGPU_GPU_kernel_get_index(HGPU_GPU_kernel** kernels,HGPU_GPU_kernel* kernel);
        HGPU_GPU_kernel*  HGPU_GPU_kernel_get_by_index(HGPU_GPU_kernel** kernels,unsigned int index);
                   char*  HGPU_GPU_kernel_get_name(HGPU_GPU_kernel* kernel);
           unsigned int   HGPU_GPU_kernel_get_args_number(HGPU_GPU_kernel* kernel);
   HGPU_timer_deviation   HGPU_GPU_kernel_get_time_execution(HGPU_GPU_kernel* kernel);
                 size_t*  HGPU_GPU_kernel_get_local_worksize(HGPU_GPU_kernel* kernel,cl_device_id device);

           unsigned int   HGPU_GPU_kernel_init(HGPU_GPU_kernel*** kernels,cl_program program,const char* kernel_name,
                                               unsigned int work_dimensions,const size_t* global_size,const size_t* local_size);

                   void   HGPU_GPU_kernel_limit_local_workgroup_size(HGPU_GPU_kernel* kernel,cl_device_id device,size_t max_local_workgroup_size);
                   void   HGPU_GPU_kernel_run(HGPU_GPU_kernel* kernel,cl_command_queue queue);
                   void   HGPU_GPU_kernel_run_with_profiling(HGPU_GPU_kernel* kernel,cl_command_queue queue);
                   void   HGPU_GPU_kernel_run_without_profiling(HGPU_GPU_kernel* kernel,cl_command_queue queue);
                   void   HGPU_GPU_kernel_run_async(HGPU_GPU_kernel* kernel,cl_command_queue queue);
                   void   HGPU_GPU_kernel_run_profile(HGPU_GPU_kernel* kernel);


              cl_kernel   HGPU_GPU_kernel_create(cl_program program,const char* kernel_name);
                   void   HGPU_GPU_kernel_bind_argument(cl_kernel kernel,unsigned int argument_id,size_t argument_size,const void* argument_ptr);
               cl_event   HGPU_GPU_kernel_start(cl_kernel kernel,cl_command_queue queue,unsigned int work_dimensions,
                                                const size_t* global_size,const size_t* local_size);
                   void   HGPU_GPU_kernel_run_wait_for_finish(cl_event* kernel_event);
                   void   HGPU_GPU_kernel_run_wait_for_queue_finish(cl_command_queue queue);

#endif
