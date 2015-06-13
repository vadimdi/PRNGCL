/******************************************************************************
 * @file     prngcl_xor128.cl
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.1
 *
 * @brief    [PRNGCL library]
 *           contains OpenCL implementation of XOR128 pseudo-random number generator
 *
 *
 * @section  CREDITS
 *
 *   George Marsaglia,
 *   "Xorshift RNGs",
 *   Journal of Statistical Software, 8, 14 (2003), 1--6.
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

#ifndef PRNGCL_XOR128_CL
#define PRNGCL_XOR128_CL

#include "prngcl_common.cl"

#define XOR128_m_FP   (4294967296.0f)
#define XOR128_min    1
#define XOR128_max    4294967295
#define XOR128_min_FP (1.0/4294967296.0)
#define XOR128_max_FP (4294967295.0/4294967296.0)
#define XOR128_k      (2.3283064365386962890625E-10) // 1/2^32
#define XOR128_left   (XOR128_min_FP+XOR128_k*XOR128_max_FP)
#define XOR128_right  (XOR128_max_FP+XOR128_k*XOR128_min_FP)


//________________________________________________________________________________________________________ XOR128 PRNG
__attribute__((always_inline)) uint4
xor128_step(uint4 seed)
{
    uint4 result;
    uint t = (seed.x^(seed.x<<11));

	result.x = seed.y;
	result.y = seed.z;
	result.z = seed.w;
	result.w = (seed.w^(seed.w>>19))^(t^(t>>8));

    return result;
}

#ifdef PRECISION_DOUBLE  // if double precision is defined
__attribute__((always_inline)) hgpu_double
xor128_step_double(uint4* seed)
{
    hgpu_double result;
    uint rnd1, rnd2;
    uint4 sed = (*seed);
    bool flag = true;
    sed = xor128_step(sed);
    rnd1 = sed.w;
    while (flag) {
        sed = xor128_step(sed);
        rnd2 = sed.w;
        result = hgpu_uint_to_double(rnd1,rnd2,XOR128_min,XOR128_max,XOR128_k);
        if ((result>=XOR128_left) && (result<XOR128_right))
            flag = false;
        else
            rnd1 = rnd2;
    }
    (*seed) = sed;
    return result;
}
#endif

__kernel void
xor128(__global uint4* seed_table, 
                     __global hgpu_float4* randoms,
                     const uint N)
{
    uint giddst = GID;
#ifdef PRECISION_DOUBLE  // if double precision is defined
    hgpu_double4 result;
#else
    float4 result;
    float4 normal = (float4) XOR128_m_FP;
#endif
    uint4 seed = seed_table[GID];
    for (uint i = 0; i < N; i++) {
#ifdef PRECISION_DOUBLE // if double precision is defined
        result.x = xor128_step_double(&seed);
        result.y = xor128_step_double(&seed);
        result.z = xor128_step_double(&seed);
        result.w = xor128_step_double(&seed);
        randoms[giddst] = result;
#else
        seed = xor128_step(seed);
        result.x = (float) seed.w;
        seed = xor128_step(seed);
        result.y = (float) seed.w;
        seed = xor128_step(seed);
        result.z = (float) seed.w;
        seed = xor128_step(seed);
        result.w = (float) seed.w;
        randoms[giddst] = result / normal;
#endif
        giddst += GID_SIZE;
    }
    seed_table[GID] = seed;
}


#endif
