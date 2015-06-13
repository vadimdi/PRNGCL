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

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define CEIL(a)  ((a - (int)a)==0 ? (int)a : (int)a+1)

#define HGPU_GPU_MAX_PLATFORMS                32
#define HGPU_GPU_MAX_DEVICES                  64
#define HGPU_GPU_MAX_KERNELS                1024
#define HGPU_GPU_MAX_BUFFERS                1024
#define HGPU_GPU_MAX_ARGUMENTS                16
#define HGPU_GPU_MAX_TIMERS                   64
#define HGPU_GPU_MAX_STR_INFO_LENGHT        4096
#define HGPU_GPU_MAX_FILE_READ_BUFFER       8192
#define HGPU_GPU_MAX_OPTIONS_LENGTH         8192
#define HGPU_GPU_MAX_ENVIRONMENT_LENGTH     8192
#define HGPU_GPU_MAX_MEMORY_WIDTH           8192
#define HGPU_GPU_MAX_MEMORY_HEIGHT          8192
#define HGPU_GPU_MAX_INTEL_WORKGROUP_SIZE     64
#define HGPU_GPU_MAX_WORK_DIMENSIONS           4

#define HGPU_MAX_PARAMETERS                 1024
#define HGPU_PARAMETERS_STEP                  64

#define HGPU_PRNG_TEST_MAX_DURATION         (3.0) // particular PRNG test duration (in seconds)
#define HGPU_PRNG_TEST_MAX_PASSES         100000  // maximal numeber of passes in particular PRNG test
#define HGPU_PRNG_DEFAULT_SAMPLES         204800  // default number of samples for PRNG

// WARNING!: new parameters should be also included in HGPU_parameters_get_from_environment() procedure
#define HGPU_PARAMETER_DEVICE                  "DEVICE"
#define HGPU_PARAMETER_PLATFORM                "PLATFORM"
#define HGPU_PARAMETER_DEVICE_TYPE             "DEVICE_TYPE"
#define HGPU_PARAMETER_WAIT_FOR_KEYPRESS       "WAIT_FOR_KEYPRESS"
#define HGPU_PARAMETER_SHOW_STAGE              "SHOW_STAGE"
#define HGPU_PARAMETER_PROFILING               "PROFILING"
#define HGPU_PARAMETER_BRIEF_REPORT            "BRIEF_REPORT"
#define HGPU_PARAMETER_REBUILD_BINARIES        "REBUILD_BINARIES"
#define HGPU_PARAMETER_PATH_CL_ROOT            "PATH_CL_ROOT"
#define HGPU_PARAMETER_PATH_INF                "PATH_INF"
#define HGPU_PARAMETER_PATH_WORKING            "PATH_WORKING"
#define HGPU_PARAMETER_WARNING_ERROR           "WARNING_ERROR"
#define HGPU_PARAMETER_NO_CACHE                "NO_CACHE"
#define HGPU_PARAMETER_MAX_WORKGROUP_SIZE      "MAX_WORKGROUP_SIZE"
#define HGPU_PARAMETER_DEVICES_NUMBER          "DEVICES_NUMBER"
#define HGPU_PARAMETER_PRNG                    "PRNG"
#define HGPU_PARAMETER_PRNG_RANDSERIES         "PRNG_RANDSERIES"
#define HGPU_PARAMETER_PRNG_PRECISION          "PRNG_PRECISION"
#define HGPU_PARAMETER_PRNG_SAMPLES            "PRNG_SAMPLES"
#define HGPU_PARAMETER_PRNG_INSTANCES          "PRNG_INSTANCES"
#define HGPU_PARAMETER_PRNG_SEED1              "PRNG_SEED1"
#define HGPU_PARAMETER_PRNG_SEED2              "PRNG_SEED2"
#define HGPU_PARAMETER_PRNG_SEED3              "PRNG_SEED3"
#define HGPU_PARAMETER_PRNG_SEED4              "PRNG_SEED4"
#define HGPU_PARAMETER_PRNG_RANLUX_NSKIP       "PRNG_RANLUX_NSKIP"
#define HGPU_PARAMETER_PRNG_TEST_MAX_PASSES    "PRNG_TEST_MAX_DURATION"
#define HGPU_PARAMETER_PRNG_TEST_MAX_DURATION  "PRNG_TEST_MAX_DURATION"

#define HGPU_PARAMETER_INF_PLATFORM            "PLATFORM"
#define HGPU_PARAMETER_INF_DEVICE              "DEVICE"
#define HGPU_PARAMETER_INF_MD5                 "MD5"
#define HGPU_PARAMETER_INF_NUMBER              "NUMBER"
#define HGPU_PARAMETER_INF_OPTIONS             "OPTIONS"
#define HGPU_PARAMETER_INF_DATE                "DATE"

#define HGPU_ENVIRONMENT_PREFIX                "HGPU_"

#define HGPU_OPENCL_OPTION_ERROR               "-Werror"
#define HGPU_OPENCL_OPTION_ERROR_SP            " -Werror"

#define HGPU_PRECISION_SINGLE                  "SINGLE"
#define HGPU_PRECISION_DOUBLE                  "DOUBLE"
#define HGPU_PRECISION_MIXED                   "MIXED"

#define HGPU_VENDOR_AMD                        "Advanced Micro Devices"
#define HGPU_VENDOR_APPLE                      "APPLE"
#define HGPU_VENDOR_INTEL                      "Intel"
#define HGPU_VENDOR_NVIDIA                     "NVIDIA"


#ifndef FILENAME_MAX
#define FILENAME_MAX    4096
#endif

#define HGPU_FILENAME_MAX   FILENAME_MAX // MIN(FILENAME_MAX,4096)

// SAFE_LINUX_TIMER = turn on safe timer for linux (timer counts only CPU time, not real time)
#ifndef _WIN32
#define SAFE_LINUX_TIMER
#endif

// autoselect device from list by memory, not by number of compute units
#define HGPU_GPU_DEVICE_SELECT_BY_MEMORY

#define HGPU_GPU_PATH_SEPARATOR                "/"
#define HGPU_GPU_PATH_SEPARATOR2               "\\"
#define HGPU_GPU_ROOT_FILE                     "hgpucl.txt"

#define HGPU_CHAR_NEWLINE                      '\n'
#define HGPU_CHAR_CR                           '\r'
#define HGPU_CHAR_TAB                          '\t'
#define HGPU_CHAR_EQ                           '='
#define HGPU_CHAR_MINUS                        '-'
#define HGPU_CHAR_SLASH                        '/'
#define HGPU_CHAR_SPACE                        ' '
#define HGPU_CHAR_DOT                          '.'

#define HGPU_SYMBOL_NEWLINE                    "\n"
#define HGPU_SYMBOL_REMARK                     "#"
#define HGPU_SYMBOL_EQ                         "="

#endif
