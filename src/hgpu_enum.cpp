/******************************************************************************
 * @file     hgpu_enum.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           Types used in HGPU packages
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

#include "../include/hgpu_enum.h"

#define CASE_CODE(code,value)    {case code: return value; break;}

unsigned int
HGPU_convert_precision_to_uint(HGPU_precision precision){
    switch(precision){
        CASE_CODE(HGPU_precision_none,   0);
        CASE_CODE(HGPU_precision_single, 1);
        CASE_CODE(HGPU_precision_double, 2);
        CASE_CODE(HGPU_precision_mixed,  3);
        default:
            return 0;
            break;
    }
}

HGPU_precision
HGPU_convert_precision_from_uint(unsigned int precision_code){
    switch(precision_code){
        CASE_CODE(0, HGPU_precision_none);
        CASE_CODE(1, HGPU_precision_single);
        CASE_CODE(2, HGPU_precision_double);
        CASE_CODE(3, HGPU_precision_mixed );
        default:
            return HGPU_precision_single;
            break;
    }
}

HGPU_precision
HGPU_convert_precision_from_str(const char* precision){
    if  (HGPU_string_cmp(precision,HGPU_PRECISION_SINGLE)) return HGPU_precision_single;
    if  (HGPU_string_cmp(precision,HGPU_PRECISION_DOUBLE)) return HGPU_precision_double;
    if  (HGPU_string_cmp(precision,HGPU_PRECISION_MIXED )) return HGPU_precision_mixed;
    return HGPU_precision_none;
}

const char*
HGPU_convert_precision_to_str(HGPU_precision precision){
    const static char* precision_single = HGPU_PRECISION_SINGLE;
    const static char* precision_double = HGPU_PRECISION_DOUBLE;
    const static char* precision_mixed  = HGPU_PRECISION_MIXED;
    if  (precision == HGPU_precision_single) return precision_single;
    if  (precision == HGPU_precision_double) return precision_double;
    if  (precision == HGPU_precision_mixed)  return precision_mixed;
    return NULL;
}

unsigned int
HGPU_convert_vendor_to_uint(HGPU_GPU_vendor vendor_code){
    switch(vendor_code){
        CASE_CODE(HGPU_GPU_vendor_none,   0);
        CASE_CODE(HGPU_GPU_vendor_amd,    1);
        CASE_CODE(HGPU_GPU_vendor_apple,  2);
        CASE_CODE(HGPU_GPU_vendor_nvidia, 3);
        CASE_CODE(HGPU_GPU_vendor_intel,  4);
        CASE_CODE(HGPU_GPU_vendor_any,   16);
        default:
            return 0;
            break;
    }
}

HGPU_GPU_vendor
HGPU_convert_vendor_from_str(const char* vendor_str){
    if  (strstr(vendor_str,HGPU_VENDOR_AMD))    return HGPU_GPU_vendor_amd;
    if  (strstr(vendor_str,HGPU_VENDOR_APPLE))  return HGPU_GPU_vendor_apple;
    if  (strstr(vendor_str,HGPU_VENDOR_INTEL))  return HGPU_GPU_vendor_intel;
    if  (strstr(vendor_str,HGPU_VENDOR_NVIDIA)) return HGPU_GPU_vendor_nvidia;
    return HGPU_GPU_vendor_none;
}

HGPU_GPU_vendor
HGPU_convert_vendor_from_uint(unsigned int vendor_code){
    switch(vendor_code){
        CASE_CODE(0, HGPU_GPU_vendor_none);
        CASE_CODE(1, HGPU_GPU_vendor_amd);
        CASE_CODE(2, HGPU_GPU_vendor_apple);
        CASE_CODE(3, HGPU_GPU_vendor_nvidia);
        CASE_CODE(4, HGPU_GPU_vendor_intel);
        CASE_CODE(16,HGPU_GPU_vendor_any);
        default:
            return HGPU_GPU_vendor_none;
            break;
    }
}

#ifdef HGPU_data_type_none
size_t
HGPU_convert_data_type_to_size_t(HGPU_data_type data_type){
    switch(data_type){
        CASE_CODE(HGPU_data_type_none,   0);
        CASE_CODE(HGPU_data_type_float,  sizeof(float));
        CASE_CODE(HGPU_data_type_double, sizeof(double));
        CASE_CODE(HGPU_data_type_int,    sizeof(int));
        CASE_CODE(HGPU_data_type_uint,   sizeof(unsigned int));
        default:
            return 0;
            break;
    }
}
#endif
