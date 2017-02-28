/******************************************************************************
 * @file     hgpucl_devices.h
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           Interface for OpenCL AMD APP & nVidia SDK environment
 *           devices submodule
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

#ifndef HGPUCL_DEVICES_H
#define HGPUCL_DEVICES_H

#include "hgpucl.h"

       HGPU_GPU_devices*  HGPU_GPU_devices_new(unsigned int number_of_devices);
       HGPU_GPU_devices*  HGPU_GPU_devices_new_empty(void);
                   void   HGPU_GPU_devices_delete(HGPU_GPU_devices** devices);
       HGPU_GPU_devices*  HGPU_GPU_devices_clone(HGPU_GPU_devices* devices);
                   void   HGPU_GPU_devices_copy(HGPU_GPU_devices** devices_destination,HGPU_GPU_devices* devices_source);
    
       HGPU_GPU_devices*  HGPU_GPU_devices_get(void);
       HGPU_GPU_devices*  HGPU_GPU_devices_get_on_platform(cl_platform_id platform);
       HGPU_GPU_devices*  HGPU_GPU_devices_get_on_platforms(HGPU_GPU_platforms* platforms);

       HGPU_GPU_devices*  HGPU_GPU_devices_get_by_vendor(HGPU_GPU_vendor vendor);
       HGPU_GPU_devices*  HGPU_GPU_devices_get_by_vendor_on_platform(cl_platform_id platform,HGPU_GPU_vendor vendor);
       HGPU_GPU_devices*  HGPU_GPU_devices_get_by_vendor_on_platforms(HGPU_GPU_platforms* platforms,HGPU_GPU_vendor vendor);

           unsigned int   HGPU_GPU_devices_get_total_number(void);
           unsigned int   HGPU_GPU_devices_get_number(HGPU_GPU_devices* devices);
           unsigned int   HGPU_GPU_devices_get_number_on_platform(cl_platform_id platform);
           unsigned int   HGPU_GPU_devices_get_number_on_platforms(HGPU_GPU_platforms* platforms);
       HGPU_GPU_version   HGPU_GPU_devices_get_version_max(HGPU_GPU_devices* devices);

                   void   HGPU_GPU_devices_select_by_vendor(HGPU_GPU_devices** devices,HGPU_GPU_vendor vendor);
                   void   HGPU_GPU_devices_select_by_version(HGPU_GPU_devices** devices,HGPU_GPU_version version);
                   void   HGPU_GPU_devices_select_by_type(HGPU_GPU_devices** devices,cl_device_type device_type);
                   void   HGPU_GPU_devices_select_by_device_and_compiler_available(HGPU_GPU_devices** devices);
                   void   HGPU_GPU_devices_select_by_global_mem_size(HGPU_GPU_devices** devices,unsigned long int memory_size_in_bytes);
                   void   HGPU_GPU_devices_select_by_max_alloc_mem(HGPU_GPU_devices** devices,unsigned long int memory_size_in_bytes);
                   void   HGPU_GPU_devices_select_by_parameter_uint(HGPU_GPU_devices** devices,cl_device_info parameter,unsigned long int min_value);
                   void   HGPU_GPU_devices_select_by_double_precision(HGPU_GPU_devices** devices);
                   void   HGPU_GPU_devices_select_by_highest_version(HGPU_GPU_devices** devices);
       HGPU_GPU_devices*  HGPU_GPU_devices_select_auto(HGPU_parameter** parameters);

                   void   HGPU_GPU_devices_exclude_device(HGPU_GPU_devices** devices,cl_device_id device);
                   void   HGPU_GPU_devices_sort(HGPU_GPU_devices** devices);

                   void   HGPU_GPU_devices_print(HGPU_GPU_devices* devices);
                   void   HGPU_GPU_devices_print_platforms(HGPU_GPU_devices* devices);


           unsigned int   HGPU_GPU_device_get_index_of(cl_device_id device);
           unsigned int   HGPU_GPU_device_get_index_of_on_platform(cl_device_id device);
         cl_platform_id   HGPU_GPU_device_get_platform(cl_device_id device);
           unsigned int   HGPU_GPU_device_get_platform_index_of(cl_device_id device);
           cl_device_id   HGPU_GPU_device_get_by_index(unsigned int device_index);
           cl_device_id   HGPU_GPU_device_get_by_index_on_platform(cl_platform_id platform,unsigned int device_index);
      unsigned long int   HGPU_GPU_device_get_info_uint(cl_device_id device,cl_device_info parameter);
                   char*  HGPU_GPU_device_get_info_str(cl_device_id device,cl_device_info parameter);
                   char*  HGPU_GPU_device_get_info_short(cl_device_id device);
   HGPU_GPU_device_info   HGPU_GPU_device_get_info(cl_device_id device);
                   char*  HGPU_GPU_device_get_name(cl_device_id device);
       HGPU_GPU_version   HGPU_GPU_device_get_version(cl_device_id device);
           unsigned int   HGPU_GPU_device_get_rating(cl_device_id device);
           unsigned int   HGPU_GPU_device_get_max_memory_width(cl_device_id device);
      unsigned long int   HGPU_GPU_device_get_max_allocation_memory(cl_device_id device);

                   void   HGPU_GPU_device_print_info(cl_device_id device);
                   void   HGPU_GPU_device_print_name(cl_device_id device);

        HGPU_GPU_vendor   HGPU_GPU_device_get_vendor(cl_device_id device);
           cl_device_id   HGPU_GPU_device_get_first(HGPU_GPU_devices* devices);
           cl_device_id   HGPU_GPU_device_get_best(HGPU_GPU_devices* devices);
           cl_device_id   HGPU_GPU_device_get_next(HGPU_GPU_devices* devices,cl_device_id device);

                   bool   HGPU_GPU_device_check_double_precision(cl_device_id device);

           cl_device_id   HGPU_GPU_device_select_auto(void);
           cl_device_id   HGPU_GPU_device_select_auto_next(cl_device_id device);
           cl_device_id   HGPU_GPU_device_select_auto_from_devices(HGPU_GPU_devices* devices);
           cl_device_id   HGPU_GPU_device_select_auto_from_devices_next(HGPU_GPU_devices* devices,cl_device_id device);

           cl_device_id   HGPU_GPU_device_select_auto_with_parameters(HGPU_parameter** parameters);
           cl_device_id   HGPU_GPU_device_select_auto_with_parameters_next(cl_device_id device,HGPU_parameter** parameters);

#endif
