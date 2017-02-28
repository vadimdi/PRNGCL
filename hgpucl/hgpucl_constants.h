/******************************************************************************
 * @file     hgpucl_constants.h
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           Interface for OpenCL AMD APP & nVidia SDK environment
 *           global constants
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

#ifndef HGPUCL_CONSTANTS_H
#define HGPUCL_CONSTANTS_H

#define HGPUCL_VERSION_MAJOR 1
#define HGPUCL_VERSION_MINOR 0
#define HGPUCL_VERSION_MICRO 2

#define HGPU_GPU_MAX_PLATFORMS                32
#define HGPU_GPU_MAX_DEVICES                  64
#define HGPU_GPU_MAX_KERNELS                1024
#define HGPU_GPU_MAX_BUFFERS                1024
#define HGPU_GPU_MAX_ARGUMENTS                16
#define HGPU_GPU_MAX_TIMERS                   64
//#define HGPU_GPU_MAX_STR_INFO_LENGHT        HGPU_MAX_STR_INFO_LENGHT
//#define HGPU_GPU_MAX_FILE_READ_BUFFER       HGPU_MAX_FILE_READ_BUFFER
#define HGPU_GPU_MAX_OPTIONS_LENGTH         8192
//#define HGPU_GPU_MAX_ENVIRONMENT_LENGTH     HGPU_MAX_ENVIRONMENT_LENGTH
#define HGPU_GPU_MAX_MEMORY_WIDTH           8192
#define HGPU_GPU_MAX_MEMORY_HEIGHT          8192
#define HGPU_GPU_MAX_INTEL_WORKGROUP_SIZE     64
#define HGPU_GPU_MAX_WORK_DIMENSIONS           4

#define HGPU_PRNG_TEST_MAX_DURATION         (3.0) // particular PRNG test duration (in seconds)
#define HGPU_PRNG_TEST_MAX_PASSES         100000  // maximal numeber of passes in particular PRNG test
#define HGPU_PRNG_DEFAULT_SAMPLES         204800  // default number of samples for PRNG

#define HGPU_OPENCL_OPTION_ERROR               "-Werror"
#define HGPU_OPENCL_OPTION_ERROR_SP            " -Werror"

// autoselect device from list by memory, not by number of compute units
#define HGPU_GPU_DEVICE_SELECT_BY_MEMORY

#endif
