/******************************************************************************
 * @file     hgpu_error.h
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           Error codes of HGPU modules
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

#ifndef HGPU_ERROR_H
#define HGPU_ERROR_H

#include <stdio.h>
#include <stdlib.h>

    typedef void HGPU_error_handler_t(
               int  error_code,
        const char* error_message
    );

// error codes_________________________________________________________________________________
#define HGPU_ERROR_SUCCESS                                0 // OK
#define HGPU_ERROR_UNKNOWN_PARAMETER                   2048 // unknown parameter
#define HGPU_ERROR_NO_MEMORY                           2049 // memory allocation error
#define HGPU_ERROR_ARRAY_OUT_OF_BOUND                  2050 // out of bounds array
#define HGPU_ERROR_NO_PLATFORM                         2101 // no OpenCL platform
#define HGPU_ERROR_NO_DEVICE                           2102 // no OpenCL device
#define HGPU_ERROR_DEVICE_INITIALIZATION_FAILED        2103 // error of device initialization
#define HGPU_ERROR_NO_BUFFER                           2104 // no buffer
#define HGPU_ERROR_BAD_OPENCL_VERSION                  2105 // bad OpenCL version
#define HGPU_ERROR_PATH_NOT_FOUND_ROOT                 2106 // could not determine cl root path
#define HGPU_ERROR_FILE_NOT_FOUND                      2107 // file not found
#define HGPU_ERROR_FILE_NOT_CLOSED                     2108 // file not closed
#define HGPU_ERROR_WARNING_ERROR                       2109 // warning caused error (setup flags)
#define HGPU_ERROR_BAD_KERNEL                          2110 // bad kernel
#define HGPU_ERROR_BAD_BUFFER                          2111 // bad buffer
#define HGPU_ERROR_BAD_CONTEXT                         2112 // bad context
#define HGPU_ERROR_BAD_PRNG                            2113 // bad PRNG
#define HGPU_ERROR_BAD_PRNG_INIT                       2114 // bad PRNG initialization
// negative codes are for OpenCL errors

            void  HGPU_error(int error_code);
            void  HGPU_error_note(int error_code,const char* error_message);
            void  HGPU_error_message(int error_code,const char* error_message);
      const char* HGPU_error_code_to_str(int error_code);

#endif
