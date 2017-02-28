/******************************************************************************
* @file     hgpu_constants.h
* @version  2.0.0
*
* @brief    HGPU library
*
* This file contains a set of constants to be used in HGPU library
*
* @section  LICENSE
*
* @copyright Copyright (c) 2013-2016 HGPU group. All rights reserved.
*
* @author   Vadim Demchik <vadimdi@yahoo.com>
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

#ifndef _HGPU_CONSTANTS_H_
#define _HGPU_CONSTANTS_H_

// permanent constants
#define _KB                                  1024    ///< size: 1 kilobyte
#define _MB                             (_KB*_KB)    ///< size: 1 megabyte
#define _GB                             (_KB*_MB)    ///< size: 1 gigabyte


// logging severity
#define HGPU_LOGGING_EVERYTHING                 0   ///< logging: output everything
#define HGPU_LOGGING_DETAILS                    1   ///< logging: output detailed information
#define HGPU_LOGGING_INFO                       2   ///< logging: output information
#define HGPU_LOGGING_WARNING                    3   ///< logging: output warnings
#define HGPU_LOGGING_ERROR                      4   ///< logging: output errors
#define HGPU_LOGGING_FATAL                      5   ///< logging: output fatal information
#define HGPU_LOGGING_MAX                        6   ///< logging: maximal possible level of logging

// path symbols and vars
#define HGPU_CHAR_PATH_LINUX                  '/'   ///< char: path separator for Linux platform
#define HGPU_CHAR_PATH_WINDOWS               '\\'   ///< char: path separator for Windows platform
#define HGPU_FILENAME_MAX            FILENAME_MAX   ///< the maximum allowed length of string with filename or path

#define HGPU_PATH_SEPARATOR_LINUX             "/"
#define HGPU_PATH_SEPARATOR_WINDOWS          "\\"


// char constants
#define HGPU_CHAR_EMPTY                         0   ///< char: empty character (end of string)
#define HGPU_CHAR_NEWLINE                    '\n'   ///< char: new line (end of line)
#define HGPU_CHAR_CR                         '\r'   ///< char: carriage return
#define HGPU_CHAR_TAB                        '\t'
#define HGPU_CHAR_EQ                         '='
#define HGPU_CHAR_MINUS                      '-'
#define HGPU_CHAR_SLASH                      '/'
#define HGPU_CHAR_SPACE                      ' '
#define HGPU_CHAR_DOT                        '.'

// precision constants
#define HGPU_PRECISION_SINGLE           "SINGLE"
#define HGPU_PRECISION_DOUBLE           "DOUBLE"
#define HGPU_PRECISION_MIXED             "MIXED"

// string constants
#define HGPU_MAX_STR_INFO_LENGHT            4096    ///<  maximal length of the temporary buffers
#define HGPU_MAX_FILE_READ_BUFFER           8192    ///<  maximal length for reading buffer
#define HGPU_MAX_ENVIRONMENT_LENGTH         8192    ///<  maximal length for environment variable

// GPU vendors
#define HGPU_VENDOR_AMD                        "Advanced Micro Devices"
#define HGPU_VENDOR_APPLE                      "APPLE"
#define HGPU_VENDOR_INTEL                      "Intel"
#define HGPU_VENDOR_NVIDIA                     "NVIDIA"

// constants for parameters
#define HGPU_MAX_PARAMETERS                 1024
#define HGPU_PARAMETERS_STEP                  64

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

#define HGPU_GPU_ROOT_FILE                     "hgpucl.txt"

#define HGPU_SYMBOL_NEWLINE                    "\n"
#define HGPU_SYMBOL_REMARK                     "#"
#define HGPU_SYMBOL_EQ                         "="


#ifndef FILENAME_MAX
#define FILENAME_MAX    4096
#endif

#define HGPU_FILENAME_MAX   FILENAME_MAX // MIN(FILENAME_MAX,4096)

// SAFE_LINUX_TIMER = turn on safe timer for linux (timer counts only CPU time, not real time)
#ifndef _WIN32
#define SAFE_LINUX_TIMER
#endif


#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define CEIL(a)  ((a - (int)a)==0 ? (int)a : (int)a+1)


#endif // _HGPU_CONSTANTS_H_
