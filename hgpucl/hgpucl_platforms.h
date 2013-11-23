/******************************************************************************
 * @file     hgpucl_platforms.h
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0
 *
 * @brief    [HGPU library]
 *           Interface for OpenCL AMD APP & nVidia SDK environment
 *           platforms submodule
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

#ifndef HGPUCL_PLATFORMS_H
#define HGPUCL_PLATFORMS_H

#include <CL/cl.h>
#include "platform.h"
#include "hgpucl_constants.h"
#include "../include/hgpu_string.h"
#include "../include/hgpu_enum.h"
#include "../include/hgpu_error.h"
#include "hgpucl_error.h"

    // structure for arrays of platforms
    typedef struct{
                   cl_platform_id*  platforms;
                     unsigned int   number_of_platforms;
    } HGPU_GPU_platforms;

    typedef struct{
                     unsigned int   major;
                     unsigned int   minor;
    } HGPU_GPU_version;

     HGPU_GPU_platforms*  HGPU_GPU_platforms_new(unsigned int number_of_platforms);
                   void   HGPU_GPU_platforms_delete(HGPU_GPU_platforms** platforms);

     HGPU_GPU_platforms*  HGPU_GPU_platforms_get(void);
     HGPU_GPU_platforms*  HGPU_GPU_platforms_get_by_vendor(HGPU_GPU_vendor vendor);
           unsigned int   HGPU_GPU_platforms_get_total_number(void);
           unsigned int   HGPU_GPU_platforms_get_number(HGPU_GPU_platforms* platforms);

                   void   HGPU_GPU_platforms_select_by_vendor(HGPU_GPU_platforms** platforms,HGPU_GPU_vendor vendor);
                   void   HGPU_GPU_platforms_select_by_version(HGPU_GPU_platforms** platforms,HGPU_GPU_version version);

           unsigned int   HGPU_GPU_platform_get_index_of(cl_platform_id platform);
         cl_platform_id   HGPU_GPU_platform_get_by_index(unsigned int platform_number);
        HGPU_GPU_vendor   HGPU_GPU_platform_get_vendor(cl_platform_id platform);
                   char*  HGPU_GPU_platform_get_info_str(cl_platform_id platform,cl_platform_info parameter);
                   char*  HGPU_GPU_platform_get_info_short(cl_platform_id platform);
                   char*  HGPU_GPU_platform_get_name(cl_platform_id platform);
       HGPU_GPU_version   HGPU_GPU_platform_get_version(cl_platform_id platform);
         cl_platform_id   HGPU_GPU_platform_get_first(HGPU_GPU_platforms* platforms);

                   void   HGPU_GPU_platform_print_info(cl_platform_id platform);
                   void   HGPU_GPU_platform_print_name(cl_platform_id platform);

       HGPU_GPU_version   HGPU_GPU_version_get(char* version_str);
                   bool   HGPU_GPU_version_check(HGPU_GPU_version GPU_version,HGPU_GPU_version version);
       HGPU_GPU_version   HGPU_GPU_version_max(HGPU_GPU_version version1, HGPU_GPU_version version2);
       HGPU_GPU_version   HGPU_GPU_version_min(HGPU_GPU_version version1, HGPU_GPU_version version2);


#endif
