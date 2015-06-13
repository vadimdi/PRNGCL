/******************************************************************************
 * @file     hgpucl_context.h
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           Interface for OpenCL AMD APP & nVidia SDK environment
 *           context submodule
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

#ifndef HGPUCL_CONTEXT_H
#define HGPUCL_CONTEXT_H

#include <CL/cl.h>
#include "platform.h"
#include "hgpucl_error.h"
#include "hgpucl_devices.h"
#include "hgpucl_kernel.h"
#include "hgpucl_buffer.h"
#include "../include/hgpu_io.h"
#include "../include/hgpu_convert.h"
#include "../include/hgpu_timer.h"

    // structure for debug flags
    typedef struct{
                           size_t   max_workgroup_size; // maximum workgroup size for kernels
                             bool   wait_for_keypress;
                             bool   profiling;
                             bool   brief_report;
                             bool   show_stage;
                             bool   local_run;
                             bool   rebuild_binaries;
                             bool   warning_error;
                             bool   no_cache;
    } HGPU_GPU_debug;

    const HGPU_GPU_debug HGPU_GPU_debug_default = {0, false, false, false, false, false, false, false, false};


    // structure for context
    typedef struct{
                     cl_device_id   device;
             HGPU_GPU_device_info   device_info;
                       cl_context   context;
                 cl_command_queue   queue;
                   HGPU_GPU_debug   debug_flags;
                     unsigned int   number_of_programs;
                       cl_program*  program;
                  HGPU_GPU_kernel** kernel;
                  HGPU_GPU_buffer** buffer;
    } HGPU_GPU_context;


       HGPU_GPU_context*  HGPU_GPU_context_new(cl_device_id device,HGPU_GPU_debug debug_flags);
                   void   HGPU_GPU_context_delete(HGPU_GPU_context** context);
             cl_context   HGPU_GPU_context_get(HGPU_GPU_context* context);

       HGPU_GPU_context*  HGPU_GPU_context_autoselect(void);
       HGPU_GPU_context*  HGPU_GPU_context_autoselect_next(HGPU_GPU_context* context);
       HGPU_GPU_context*  HGPU_GPU_context_select_auto(HGPU_parameter** parameters);
       HGPU_GPU_context*  HGPU_GPU_context_select_auto_next(HGPU_GPU_context* context,HGPU_parameter** parameters);

         HGPU_GPU_debug   HGPU_GPU_debug_get_flags(HGPU_parameter** parameters);
                   void   HGPU_GPU_context_check_flags(HGPU_GPU_context* context);

           unsigned int   HGPU_GPU_context_kernel_init(HGPU_GPU_context* context,cl_program program,const char* kernel_name,
                               unsigned int work_dimensions,const size_t* global_size,const size_t* local_size);

           unsigned int   HGPU_GPU_context_buffer_init(HGPU_GPU_context* context,void* host_ptr_to_buffer,
                                        HGPU_GPU_buffer_types buffer_type,size_t buffer_elements,size_t sizeof_element);
                   void   HGPU_GPU_context_buffer_release(HGPU_GPU_context* context,unsigned int buffer_index);
                 size_t   HGPU_GPU_context_buffer_size_align(HGPU_GPU_context* context,size_t buffer_size);
                   void   HGPU_GPU_context_buffer_set_name(HGPU_GPU_context* context,unsigned int buffer_index,const char* buffer_name);
                   void*  HGPU_GPU_context_buffer_map(HGPU_GPU_context* context,unsigned int buffer_id);
                   void   HGPU_GPU_context_buffer_unmap(HGPU_GPU_context* context,unsigned int buffer_id,void* data_ptr);
                   void*  HGPU_GPU_context_buffer_map_async(HGPU_GPU_context* context,unsigned int buffer_id);
                   void   HGPU_GPU_context_buffer_unmap_async(HGPU_GPU_context* context,unsigned int buffer_id,void* data_ptr);
                 cl_mem   HGPU_GPU_context_buffer_get(HGPU_GPU_context* context,unsigned int buffer_id);
                   void*  HGPU_GPU_context_buffer_get_mapped(HGPU_GPU_context* context,unsigned int buffer_id);

           unsigned int   HGPU_GPU_context_kernel_bind_buffer(HGPU_GPU_context* context,unsigned int kernel_id,unsigned int buffer_id);
           unsigned int   HGPU_GPU_context_kernel_bind_buffer_by_argument_id(HGPU_GPU_context* context,unsigned int kernel_id,
                                        unsigned int buffer_id,unsigned int argument_id);
           unsigned int   HGPU_GPU_context_kernel_bind_constant(HGPU_GPU_context* context,unsigned int kernel_id,void* constant_ptr,size_t sizeof_constant);
           unsigned int   HGPU_GPU_context_kernel_bind_constant_by_argument_id(HGPU_GPU_context* context,unsigned int kernel_id,void* constant_ptr,
                                        size_t sizeof_constant,unsigned int argument_id);
                   void   HGPU_GPU_context_kernel_run(HGPU_GPU_context* context,unsigned int kernel_id);
                   void   HGPU_GPU_context_kernel_run_async(HGPU_GPU_context* context,unsigned int kernel_id);
                   void   HGPU_GPU_context_kernel_limit_max_workgroup_size(HGPU_GPU_context* context,unsigned int kernel_id);
                   char*  HGPU_GPU_context_kernel_get_name(HGPU_GPU_context* context,unsigned int kernel_id);
           unsigned int   HGPU_GPU_context_kernel_get_args_number(HGPU_GPU_context* context,unsigned int kernel_id);
           unsigned int   HGPU_GPU_context_kernel_get_id(HGPU_GPU_context* context,const char* kernel_name);
        HGPU_GPU_kernel*  HGPU_GPU_context_kernel_get_by_id(HGPU_GPU_context* context,unsigned int kernel_id);

                   void   HGPU_GPU_context_print_memory_utilized(HGPU_GPU_context* context);
                   void   HGPU_GPU_context_print_detailed_report(HGPU_GPU_context* context);
                   void   HGPU_GPU_context_print_stage(HGPU_GPU_context* context,const char* stage_message);
                   void   HGPU_GPU_context_print_used_hardware(HGPU_GPU_context* context);

                   void   HGPU_GPU_context_queue_wait_for_finish(HGPU_GPU_context* context);
                   void   HGPU_GPU_context_event_wait_for(cl_event* context_event);

#endif
