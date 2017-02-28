/******************************************************************************
 * @file     hgpu_enum.h
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           Types used in HGPU packages
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

#ifndef HGPU_ENUM_H
#define HGPU_ENUM_H

#include "hgpu.h"

    typedef enum enum_HGPU_storage_type {
            HGPU_storage_none,                  // default unknown storage type
            HGPU_storage_joint,                 // joint data based on two other data arrays
            HGPU_storage_double2high,           // high dword in double2
            HGPU_storage_double2low,            // low  dword in double2
            HGPU_storage_double                 // double
    } HGPU_storage_type;

    typedef enum enum_HGPU_PRNG_output_type {
            HGPU_PRNG_output_type_none,         // dummy PRNG
            HGPU_PRNG_output_type_uint,         // PRNG returns (unsigned integer)s only
            HGPU_PRNG_output_type_uint4,        // PRNG returns (unsigned integer4)s only
            HGPU_PRNG_output_type_uint4by1,     // PRNG returns (unsigned integer4)s only - for double precision checking after each double
            HGPU_PRNG_output_type_float,        // PRNG returns (float)s only   (single precision)
            HGPU_PRNG_output_type_float4,       // PRNG returns (float4)s only  (single precision)
            HGPU_PRNG_output_type_float4by1,    // PRNG returns (float4)s only  (single precision) - for double precision checking after each double
            HGPU_PRNG_output_type_double,       // PRNG returns (double)s only  (double precision)
            HGPU_PRNG_output_type_double4       // PRNG returns (double4)s only (double precision)
    } HGPU_PRNG_output_type;

    typedef enum enum_HGPU_GPU_vendors{
        HGPU_GPU_vendor_none   = 0,             //
        HGPU_GPU_vendor_amd    = 1,             // Advanced Micro Devices, Inc.
        HGPU_GPU_vendor_apple  = 2,             // Apple
        HGPU_GPU_vendor_nvidia = 3,             // NVIDIA Corporation
        HGPU_GPU_vendor_intel  = 4,             // Intel(R) Corporation
        HGPU_GPU_vendor_any    = 16             // any vendor
    } HGPU_GPU_vendor;

    typedef enum enum_HGPU_GPU_buffer_types{
        HGPU_GPU_buffer_type_none     = 0,      // NULL buffer type
        HGPU_GPU_buffer_type_global   = 1,      // Global buffer type
        HGPU_GPU_buffer_type_input    = 2,      // Input buffer type
        HGPU_GPU_buffer_type_io       = 3,      // like Global buffer, but with initialization
        HGPU_GPU_buffer_type_output   = 4,      // Output buffer type
        HGPU_GPU_buffer_type_constant = 5,      // Constant buffer type
        HGPU_GPU_buffer_type_local    = 6,      // Local buffer type
        HGPU_GPU_buffer_type_uav      = 7       // UAV buffer type
    } HGPU_GPU_buffer_types;

    HGPU_precision   HGPU_convert_precision_from_uint(unsigned int precision_code);
    HGPU_precision   HGPU_convert_precision_from_str(const char* precision);
        const char*  HGPU_convert_precision_to_str(HGPU_precision precision);
      unsigned int   HGPU_convert_precision_to_uint(HGPU_precision precision);

#ifdef HGPU_data_type_none
            size_t   HGPU_convert_data_type_to_size_t(HGPU_data_type data_type);
#endif

    HGPU_GPU_vendor  HGPU_convert_vendor_from_str(const char* vendor_str);
    HGPU_GPU_vendor  HGPU_convert_vendor_from_uint(unsigned int vendor_code);

#endif
