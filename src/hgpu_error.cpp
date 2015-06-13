/******************************************************************************
 * @file     hgpu_error.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0
 *
 * @brief    [HGPU library]
 *           Error codes of HGPU modules
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

#include "../include/hgpu_error.h"

#define ERROR_CODE(error_code,error_msg)    {case error_code: error_message = error_msg; break;}

HGPU_error_handler_t* HGPU_error_handler = NULL;

void
HGPU_error(int error_code){
    if (error_code==HGPU_ERROR_SUCCESS) return;

    const char* error_message = NULL;
    HGPU_error_message(error_code,error_message);
}

void
HGPU_error_note(int error_code,const char* error_message){
    if (error_code==HGPU_ERROR_SUCCESS) return;

    const char* err = error_message;
    if (!error_message) err = HGPU_error_code_to_str(error_code);
    if (HGPU_error_handler){
        (*HGPU_error_handler)(error_code,err);
        return;
    }
    fflush (stdout);
    fprintf(stderr,"HGPU library run-time error [%i]: %s\n\n",error_code,err);
    fflush (stderr);
}

void
HGPU_error_message(int error_code,const char* error_message){
    HGPU_error_note(error_code,error_message);
    exit(error_code);
}

const char*
HGPU_error_code_to_str(int error_code){
    const char* error_message;

    // setup error messages
    switch (error_code){
        ERROR_CODE(HGPU_ERROR_UNKNOWN_PARAMETER,                "unknown parameter");
        ERROR_CODE(HGPU_ERROR_NO_MEMORY,                        "memory allocation error");
        ERROR_CODE(HGPU_ERROR_ARRAY_OUT_OF_BOUND,               "out of bounds array index error");
        ERROR_CODE(HGPU_ERROR_NO_PLATFORM,                      "there is no desired OpenCL platform");
        ERROR_CODE(HGPU_ERROR_NO_DEVICE,                        "there is no desired OpenCL device");
        ERROR_CODE(HGPU_ERROR_DEVICE_INITIALIZATION_FAILED,     "OpenCL device initialization failed");
        ERROR_CODE(HGPU_ERROR_NO_BUFFER,                        "there is no desired OpenCL memory object");
        ERROR_CODE(HGPU_ERROR_BAD_OPENCL_VERSION,               "bad OpenCL version");
        ERROR_CODE(HGPU_ERROR_PATH_NOT_FOUND_ROOT,              "OpenCL kernels root path not found");
        ERROR_CODE(HGPU_ERROR_FILE_NOT_FOUND,                   "file not found");
        ERROR_CODE(HGPU_ERROR_FILE_NOT_CLOSED,                  "file was not closed");
        ERROR_CODE(HGPU_ERROR_WARNING_ERROR,                    "warning caused error");
        ERROR_CODE(HGPU_ERROR_BAD_KERNEL,                       "bad kernel ptr");
        ERROR_CODE(HGPU_ERROR_BAD_BUFFER,                       "bad buffer ptr");
        ERROR_CODE(HGPU_ERROR_BAD_CONTEXT,                      "bad context ptr");
        ERROR_CODE(HGPU_ERROR_BAD_PRNG,                         "bad PRNG");
        ERROR_CODE(HGPU_ERROR_BAD_PRNG_INIT,                    "bad PRNG initializtion");

        default:
            error_message = "unexpected error";
            break;
    }

    return error_message;
}
