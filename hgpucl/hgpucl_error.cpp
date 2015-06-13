/******************************************************************************
 * @file     hgpucl_error.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           Interface for OpenCL AMD APP & nVidia SDK environment
 *           error submodule
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

#include "hgpucl_error.h"

#define HGPU_ERROR_CODE(error_code,error_msg)    {case error_code: error_message = error_msg; break;}

void
HGPU_GPU_error(int error_code){
    if (error_code==CL_SUCCESS) return;
    const char* error_message = HGPU_GPU_error_code_description(error_code);
    HGPU_GPU_error_message(error_code,error_message);
}

const char*
HGPU_GPU_error_code_description(int error_code){
    const char* error_message = NULL;

    switch (error_code){
        HGPU_ERROR_CODE(CL_DEVICE_NOT_FOUND,              "(-1): device not found");
        HGPU_ERROR_CODE(CL_DEVICE_NOT_AVAILABLE,          "(-2): device is not available");
        HGPU_ERROR_CODE(CL_COMPILER_NOT_AVAILABLE,        "(-3): compiler is not available");
        HGPU_ERROR_CODE(CL_MEM_OBJECT_ALLOCATION_FAILURE, "(-4): buffer allocation failure");
        HGPU_ERROR_CODE(CL_OUT_OF_RESOURCES,              "(-5): out of resources");
        HGPU_ERROR_CODE(CL_OUT_OF_HOST_MEMORY,            "(-6): out of host memory");
        HGPU_ERROR_CODE(CL_PROFILING_INFO_NOT_AVAILABLE,  "(-7): profiling info is not available");
        HGPU_ERROR_CODE(CL_BUILD_PROGRAM_FAILURE,         "(-11): build program failure");
        HGPU_ERROR_CODE(CL_MAP_FAILURE,                   "(-12): map failure");
#ifdef  CL_COMPILE_PROGRAM_FAILURE
        HGPU_ERROR_CODE(CL_COMPILE_PROGRAM_FAILURE,       "(-15): compile program failure");
#endif
#ifdef  CL_LINKER_NOT_AVAILABLE
        HGPU_ERROR_CODE(CL_LINKER_NOT_AVAILABLE,          "(-16): linker is not available");
#endif
#ifdef  CL_LINK_PROGRAM_FAILURE
        HGPU_ERROR_CODE(CL_LINK_PROGRAM_FAILURE,          "(-17): link program failure");
#endif
#ifdef  CL_KERNEL_ARG_INFO_NOT_AVAILABLE
        HGPU_ERROR_CODE(CL_KERNEL_ARG_INFO_NOT_AVAILABLE, "(-19): kernel argument info is not available");
#endif
        HGPU_ERROR_CODE(CL_INVALID_VALUE,                 "(-30): invalid value");
        HGPU_ERROR_CODE(CL_INVALID_DEVICE_TYPE,           "(-31): invalid device type");
        HGPU_ERROR_CODE(CL_INVALID_PLATFORM,              "(-32): invalid platform");
        HGPU_ERROR_CODE(CL_INVALID_DEVICE,                "(-33): invalid device");
        HGPU_ERROR_CODE(CL_INVALID_CONTEXT,               "(-34): invalid context");
        HGPU_ERROR_CODE(CL_INVALID_QUEUE_PROPERTIES,      "(-35): invalid queue properties");
        HGPU_ERROR_CODE(CL_INVALID_COMMAND_QUEUE,         "(-36): invalid command queue");
        HGPU_ERROR_CODE(CL_INVALID_HOST_PTR,              "(-37): invalid host ptr");
        HGPU_ERROR_CODE(CL_INVALID_MEM_OBJECT,            "(-38): invalid buffer");
        HGPU_ERROR_CODE(CL_INVALID_BINARY,                "(-42): invalid binary");
        HGPU_ERROR_CODE(CL_INVALID_BUILD_OPTIONS,         "(-43): invalid build option");
        HGPU_ERROR_CODE(CL_INVALID_PROGRAM,               "(-44): invalid program");
        HGPU_ERROR_CODE(CL_INVALID_KERNEL_NAME,           "(-46): invalid kernel name");
        HGPU_ERROR_CODE(CL_INVALID_KERNEL_DEFINITION,     "(-47): invalid kernel definition");
        HGPU_ERROR_CODE(CL_INVALID_KERNEL,                "(-48): invalid kernel");
        HGPU_ERROR_CODE(CL_INVALID_ARG_INDEX,             "(-49): invalid argument index");
        HGPU_ERROR_CODE(CL_INVALID_ARG_VALUE,             "(-50): invalid argument value");
        HGPU_ERROR_CODE(CL_INVALID_ARG_SIZE,              "(-51): invalid argument size");
        HGPU_ERROR_CODE(CL_INVALID_KERNEL_ARGS,           "(-52): invalid kernel arguments");
        HGPU_ERROR_CODE(CL_INVALID_WORK_DIMENSION,        "(-53): invalid kernel dimension");
        HGPU_ERROR_CODE(CL_INVALID_WORK_GROUP_SIZE,       "(-54): invalid work group size");
        HGPU_ERROR_CODE(CL_INVALID_WORK_ITEM_SIZE,        "(-55): invalid work item size");
        HGPU_ERROR_CODE(CL_INVALID_GLOBAL_OFFSET,         "(-56): invalid global offset");
        HGPU_ERROR_CODE(CL_INVALID_EVENT_WAIT_LIST,       "(-57): invalid event wait list");
        HGPU_ERROR_CODE(CL_INVALID_EVENT,                 "(-58): invalid event");
        HGPU_ERROR_CODE(CL_INVALID_BUFFER_SIZE,           "(-61): invalid buffer size");
        HGPU_ERROR_CODE(CL_INVALID_GLOBAL_WORK_SIZE,      "(-63): invalid global work size");
        HGPU_ERROR_CODE(CL_INVALID_PROPERTY,              "(-64): invalid property");
#ifdef  CL_INVALID_COMPILER_OPTIONS
        HGPU_ERROR_CODE(CL_INVALID_COMPILER_OPTIONS,      "(-66): invalid compiler options");
#endif
#ifdef  CL_INVALID_LINKER_OPTIONS
        HGPU_ERROR_CODE(CL_INVALID_LINKER_OPTIONS,        "(-67): invalid linker options");
#endif
    }
    return error_message;
}

void
HGPU_GPU_error_message(int error_code,const char* error_message){

    if (error_code==CL_SUCCESS) return;
    if (error_code>0) HGPU_error_message(error_code,error_message);
    HGPU_error_message(error_code,error_message);
}

void
HGPU_GPU_error_note(int error_code,const char* error_message){
    if (error_code==CL_SUCCESS) return;
    if (error_code>0) HGPU_error_message(error_code,error_message);
    HGPU_error_note(error_code,error_message);
}
