/******************************************************************************
 * @file     prngcl_common.cl
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0
 *
 * @brief    [PRNGCL library]
 *           common module for all PRNG implementations
 *           contains precision selection, etc.
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

#ifndef PRNGCL_COMMON_CL
#define PRNGCL_COMMON_CL

#define GID_SIZE    (get_global_size(0) * get_global_size(1) * get_global_size(2))
#define GID         (get_global_id(0) + get_global_id(1) * get_global_size(0) + get_global_id(2) * get_global_size(0) * get_global_size(1))

#if defined(cl_khr_fp64)

#define hgpu_double     double
#define hgpu_double2    double2
#define hgpu_double3    double3
#define hgpu_double4    double4

#if defined(cl_amd_fp64)    // AMD extension available?
	#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#elif defined(cl_khr_fp64)  // Khronos extension available?
	#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#endif

#ifndef PRECISION
#define PRECISION   0       // single precision by default
#endif

#if ((PRECISION==0)||(PRECISION==1))
#define PRECISION_SINGLE            // define single precision
#elif (PRECISION==2)
#define PRECISION_DOUBLE            // define double precision
#endif

#ifdef PRECISION_DOUBLE
#define hgpu_float   hgpu_double
#define hgpu_float2  hgpu_double2
#define hgpu_float3  hgpu_double3
#define hgpu_float4  hgpu_double4
#else
#define hgpu_float   float
#define hgpu_float2  float2
#define hgpu_float3  float3
#define hgpu_float4  float4
#endif

#define hgpu_single  float
#define hgpu_single2 float2
#define hgpu_single3 float3
#define hgpu_single4 float4

#define MASK24       0x0FFFFFF                             // 24 bit mask
#define NORM2to24    (16777216.0f)                         // 2^24
#define NORM2_24     5.9604644775390625E-8                 // 1/2^24
#define NORM2_30     9.31322574615478515625E-10            // 1/2^30
#define NORM2_31     4.656612873077392578125E-10           // 1/2^31
#define NORM2_31_PM  4.6566128752457969241057508271680E-10 // 1/(2^31-1) for PM generator
#define NORM2_32     2.3283064365386962890625E-10          // 1/2^32
#define NORM2_48     3.5527136788005009293556213378906E-15 // 1/2^48
#define NORM2_60     8.6736173798840354720596224069595E-19 // 1/2^60
#define NORM2_62     2.1684043449710088680149056017399E-19 // 1/2^62
#define NORM2_64     5.4210108624275221700372640043497E-20 // 1/2^64

#define NORM_I_2_24  5.9604648328104515558750364705942E-8  // 2^-24/(1-2^-24)
#define NORM_I_2_31  4.6566128752457969241057508271680E-10 // 2^-31/(1-2^-31)
#define NORM_I_2_PM  4.6566128774142012721059855745230E-10 // (2^31-1)^-1/(1-(2^31-1)^-1) = 1/(2^31-2)
#define NORM_I_2_32  2.3283064370807973754314699618685E-10 // 2^-32/(1-2^-32)

#define RANGE1_a0    2.3283065492957276882397381481401E-10 // 1/(2^32-1)
#define RANGE1_a     (1.0-RANGE1_a0)

inline uint4 hgpu_float4_to_uint4(float4* x){
    uint4 result;
    result.x = (uint) (*x).x;
    result.y = (uint) (*x).y;
    result.z = (uint) (*x).z;
    result.w = (uint) (*x).w;
    return result;
}

inline hgpu_double hgpu_uint_to_double(uint rnd1, uint rnd2, uint rnd_min, uint rnd_max, double k){
    hgpu_double result = (rnd1 + k * rnd2 - (rnd_min + k * rnd_max)) / ((rnd_max - rnd_min) * (1.0 - k));
    return result;
}

inline hgpu_double4 hgpu_uint4_to_double4(uint4 rnd1, uint4 rnd2, uint rnd_min, uint rnd_max, double k){
    hgpu_double4 result;
        double a1 = (rnd_min + k * rnd_max);
        double a2 = ((rnd_max - rnd_min) * (1.0 - k));
        result.x = (rnd1.x + k * rnd2.x - a1) / a2;
        result.y = (rnd1.y + k * rnd2.y - a1) / a2;
        result.z = (rnd1.z + k * rnd2.z - a1) / a2;
        result.w = (rnd1.w + k * rnd2.w - a1) / a2;
    return result;
}

inline hgpu_double hgpu_float_to_double(float rnd1, float rnd2, double rnd_min, double rnd_max, double k){
    hgpu_double result;
        double a1 = k * (rnd_max - rnd_min);
        double a2 = (rnd_max - rnd_min) * (1.0 - k);
        double a3 = k / (rnd_max - rnd_min);
        double a4 = -rnd_min - rnd_min * a3 - k * rnd_max;
        result = (trunc((rnd1 - rnd_min) / a1) * k) / a2 + rnd2 * a3 / a2 + a4 / a2;
        return result;
}

inline hgpu_double4 hgpu_float4_to_double4(float4 rnd1, float4 rnd2, double rnd_min, double rnd_max, double k){
    hgpu_double4 result;
        double a1 = k * (rnd_max - rnd_min);
        double a2 = (rnd_max - rnd_min) * (1.0 - k);
        double a3 = k / (rnd_max - rnd_min);
        double a4 = -rnd_min - rnd_min * a3 - k * rnd_max;
        result.x = trunc((rnd1.x - rnd_min) / a1) * k / a2 + rnd2.x * a3 / a2 + a4 / a2;
        result.y = trunc((rnd1.y - rnd_min) / a1) * k / a2 + rnd2.y * a3 / a2 + a4 / a2;
        result.z = trunc((rnd1.z - rnd_min) / a1) * k / a2 + rnd2.z * a3 / a2 + a4 / a2;
        result.w = trunc((rnd1.w - rnd_min) / a1) * k / a2 + rnd2.w * a3 / a2 + a4 / a2;
        return result;
}

inline hgpu_double4 hgpu_float4_to_double4_2(float4 rnd1, float4 rnd2, double rnd_min, double rnd_max, double k){
    hgpu_double4 result;
        double a1 = k * (rnd_max - rnd_min);
        double a2 = (rnd_max - rnd_min) * (1.0 - k);
        double a3 = k / (rnd_max - rnd_min);
        double a4 = -rnd_min - rnd_min * a3 - k * rnd_max;
        result.x = trunc((rnd1.x - rnd_min) / a1) * k / a2 + rnd1.y * a3 / a2 + a4 / a2;
        result.y = trunc((rnd1.z - rnd_min) / a1) * k / a2 + rnd1.w * a3 / a2 + a4 / a2;
        result.z = trunc((rnd2.x - rnd_min) / a1) * k / a2 + rnd2.y * a3 / a2 + a4 / a2;
        result.w = trunc((rnd2.z - rnd_min) / a1) * k / a2 + rnd2.w * a3 / a2 + a4 / a2;
        return result;
}

#endif

 inline float4 hgpu_uint4_to_float4(uint4 x){
    float4 result;
    result.x = (float) x.x;
    result.y = (float) x.y;
    result.z = (float) x.z;
    result.w = (float) x.w;

    return result;
}

 typedef union _Uint_and_Float           // Uint <---> Float converter
{
    uint uint_value;
    float float_value;
} Uint_and_Float;

#endif
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
