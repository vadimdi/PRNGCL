/******************************************************************************
 * @file     hgpucl_program.h
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0
 *
 * @brief    [HGPU library]
 *           Interface for OpenCL AMD APP & nVidia SDK environment
 *           program submodule
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

#ifndef HGPUCL_PROGRAM_H
#define HGPUCL_PROGRAM_H

#include "platform.h"
#include "hgpucl_context.h"
#include "../include/hgpu_timer.h"
#include "../include/hgpu_md5.h"

             cl_program   HGPU_GPU_program_new(const char* source,HGPU_GPU_context* context);
             cl_program   HGPU_GPU_program_with_options_new(const char* source,const char* options,HGPU_GPU_context* context);

                   void   HGPU_program_add(HGPU_GPU_context* context,cl_program program);

// simple functions:
             cl_program   HGPU_program_build(cl_context context,cl_device_id device,const char* source,const char* options);
                   char*  HGPU_program_get_buildlog(cl_program program,cl_device_id device);
                   void   HGPU_program_get_binary(cl_program program,cl_device_id device,unsigned char** binary_code,size_t* binary_size);

#endif
