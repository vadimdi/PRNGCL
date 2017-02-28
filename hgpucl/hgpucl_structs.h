/******************************************************************************
 * @file     hgpucl_structs.h
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  2.0.0
 *
 * @brief    [HGPU library]
 *           Interface for OpenCL AMD APP & nVidia SDK environment
 *           structures in HGPUCL packages
 *
 *
 * @section  LICENSE
 *
 * Copyright (c) 2013-2017 Vadim Demchik
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

#ifndef HGPUCL_STRUCTS_H
#define HGPUCL_STRUCTS_H

#include "hgpucl.h"

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


    // structure for arrays of devices
    typedef struct {
        cl_device_id*  devices;
        unsigned int   number_of_devices;
    } HGPU_GPU_devices;

    // structure for device info
    typedef struct {
        cl_ulong   global_memory_size;     // CL_DEVICE_GLOBAL_MEM_SIZE
        cl_ulong   local_memory_size;      // CL_DEVICE_LOCAL_MEM_SIZE
        cl_ulong   max_constant_size;      // CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE
        cl_ulong   max_memory_size;        // CL_DEVICE_MAX_MEM_ALLOC_SIZE
        size_t   max_memory_width;       // CL_DEVICE_IMAGE2D_MAX_WIDTH
        size_t   max_memory_height;      // CL_DEVICE_IMAGE2D_MAX_HEIGHT
        unsigned int   max_compute_units;      // CL_DEVICE_MAX_COMPUTE_UNITS
        size_t   max_workgroup_size;     // max(CL_DEVICE_MAX_WORK_ITEM_SIZES)
        size_t   memory_align_factor;    // memory align factor for buffers (=CL_DEVICE_MAX_COMPUTE_UNITS)
        HGPU_GPU_vendor   platform_vendor;        // platform vendor
        HGPU_GPU_vendor   device_vendor;          // device vendor
    } HGPU_GPU_device_info;


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


    // structure for arrays of platforms
    typedef struct{
                   cl_platform_id*  platforms;
                     unsigned int   number_of_platforms;
    } HGPU_GPU_platforms;

    typedef struct{
                     unsigned int   major;
                     unsigned int   minor;
    } HGPU_GPU_version;



#endif // HGPUCL_STRUCTS_H
