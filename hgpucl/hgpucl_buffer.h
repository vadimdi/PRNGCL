/******************************************************************************
 * @file     hgpucl_buffer.h
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           Interface for OpenCL AMD APP & nVidia SDK environment
 *           buffer submodule
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

#ifndef HGPUCL_BUFFER_H
#define HGPUCL_BUFFER_H

#include <CL/cl.h>
#include "platform.h"
#include "hgpucl_error.h"
#include "hgpucl_constants.h"
#include "../include/hgpu_enum.h"
#include "../include/hgpu_string.h"
#include "../include/hgpu_timer.h"


    // structure for buffer
    typedef struct{
                           cl_mem   buffer;
                             char*  name;                              // buffer's name
                             void*  host_ptr;
                     unsigned int*  mapped_ptr;                        // ptr to corresponding host memory after mapping
                           size_t   size;
                           size_t   sizeof_element;                    // sizeof buffer element ( sizeof(...) )
            HGPU_GPU_buffer_types   buffer_type;
                // profiling data __________________________
                         cl_event   buffer_write_event;                // event for buffer write
                         cl_event   buffer_read_event;                 // event for buffer read
                         cl_ulong   buffer_write_start;                // buffer start write time
                         cl_ulong   buffer_write_finish;               // buffer finish write time
                           double   buffer_write_elapsed_time;         // total buffer write time (in nanoseconds)
                           double   buffer_write_elapsed_time_squared; // total buffer write time squared (in nanoseconds) - for deviation calculation
                              int   buffer_write_number_of;            // total number of writes - for deviation calculation
                         cl_ulong   buffer_read_start;                 // buffer start read time
                         cl_ulong   buffer_read_finish;                // buffer finish read time
                           double   buffer_read_elapsed_time;          // total buffer read time (in nanoseconds)
                           double   buffer_read_elapsed_time_squared;  // total buffer read time squared (in nanoseconds) - for deviation calculation
                              int   buffer_read_number_of;             // total number of reads - for deviation calculation
    } HGPU_GPU_buffer;

                   void   HGPU_GPU_buffers_add(HGPU_GPU_buffer*** buffers,HGPU_GPU_buffer* buffer);
                   void   HGPU_GPU_buffers_delete(HGPU_GPU_buffer*** buffers);
           unsigned int   HGPU_GPU_buffers_get_number(HGPU_GPU_buffer** buffers);

        HGPU_GPU_buffer*  HGPU_GPU_buffer_new_empty(void);
                   void   HGPU_GPU_buffer_delete(HGPU_GPU_buffer** buffer);
                   void   HGPU_GPU_buffer_release(HGPU_GPU_buffer* buffer);

           unsigned int   HGPU_GPU_buffer_get_index(HGPU_GPU_buffer** buffers,HGPU_GPU_buffer* buffer);
        HGPU_GPU_buffer*  HGPU_GPU_buffer_get_by_index(HGPU_GPU_buffer** buffers,unsigned int index);
   HGPU_timer_deviation   HGPU_GPU_buffer_get_time_read(HGPU_GPU_buffer* buffer);
   HGPU_timer_deviation   HGPU_GPU_buffer_get_time_write(HGPU_GPU_buffer* buffer);


           unsigned int   HGPU_GPU_buffer_init(HGPU_GPU_buffer*** buffers,cl_context context,void* host_ptr_to_buffer,
                                               HGPU_GPU_buffer_types buffer_type,size_t buffer_elements,size_t sizeof_element);

                   void   HGPU_GPU_buffer_set_name(HGPU_GPU_buffer* buffer,const char* buffer_name);
                   void*  HGPU_GPU_buffer_map(HGPU_GPU_buffer* buffer,cl_command_queue queue);
                   void*  HGPU_GPU_buffer_map_with_profiling(HGPU_GPU_buffer* buffer,cl_command_queue queue);
                   void*  HGPU_GPU_buffer_map_async(HGPU_GPU_buffer* buffer,cl_command_queue queue);
                   void   HGPU_GPU_buffer_unmap(HGPU_GPU_buffer* buffer,cl_command_queue queue,void* data_ptr);
                   void   HGPU_GPU_buffer_unmap_with_profiling(HGPU_GPU_buffer* buffer,cl_command_queue queue,void* data_ptr);
                   void   HGPU_GPU_buffer_unmap_async(HGPU_GPU_buffer* buffer,cl_command_queue queue,void* data_ptr);
                   void   HGPU_GPU_buffer_profile_map(HGPU_GPU_buffer* buffer);
                   void   HGPU_GPU_buffer_profile_unmap(HGPU_GPU_buffer* buffer);

                 cl_mem   HGPU_GPU_buffer_create(cl_context context,cl_mem_flags flags,size_t buffer_size_in_bytes,void* host_ptr_to_buffer);
                   void   HGPU_GPU_buffer_mem_release(cl_mem buffer);
                 size_t   HGPU_GPU_buffer_size_align_step(size_t buffer_size,size_t step);
                   void   HGPU_GPU_buffer_wait_for_event(cl_event* buffer_event);

#endif
