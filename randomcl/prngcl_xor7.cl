/******************************************************************************
 * @file     prngcl_xor7.cl
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.1
 *
 * @brief    [PRNGCL library]
 *           contains OpenCL implementation of seven-XOR pseudo-random number generator
 *
 *
 * @section  CREDITS
 *
 *   Francois Panneton and Pierre L'ecuyer,
 *   "On the xorshift random number generators",
 *   ACM Transactions on Modeling and Computer Simulation (TOMACS), 15, 4 (2005), 346--361.
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

#ifndef PRNGCL_XOR7_CL
#define PRNGCL_XOR7_CL

#include "prngcl_common.cl"

#define XOR7_m_FP   (4294967296.0f)
#define XOR7_min    1
#define XOR7_max    4294967295
#define XOR7_min_FP (1.0/4294967296.0)
#define XOR7_max_FP (4294967295.0/4294967296.0)
#define XOR7_k      (2.3283064365386962890625E-10) // 1/(2^32-1)
#define XOR7_left   (XOR7_min_FP+XOR7_k*XOR7_max_FP)
#define XOR7_right  (XOR7_max_FP+XOR7_k*XOR7_min_FP)

__attribute__((always_inline)) void
xor7_step(uint4* seed1,uint4* seed2)
{
    uint t, y;
        t = (*seed2).w;     t = t ^ (t<<13);    y = t ^ (t<<9);
        t = (*seed2).x;     y^= t ^ (t<<7);
        t = (*seed1).w;     y^= t ^ (t>>3);
        t = (*seed1).y;     y^= t ^ (t>>10);
        t = (*seed1).x;     t = t ^ (t>>7);     y^= t ^ (t<<24);

        (*seed1).xyz = (*seed1).yzw;
        (*seed1).w   = (*seed2).x;
        (*seed2).xyz = (*seed2).yzw;
        (*seed2).w   = y;
}

#ifdef PRECISION_DOUBLE  // if double precision is defined
__attribute__((always_inline)) hgpu_double
xor7_step_double(uint4* seed1,uint4* seed2)
{
    hgpu_double result;
    uint rnd1, rnd2;
    uint4 sed1 = (*seed1);
    uint4 sed2 = (*seed2);
    bool flag = true;
    xor7_step(&sed1,&sed2);
    rnd1 = sed2.w;
    while (flag) {
        xor7_step(&sed1,&sed2);
        rnd2 = sed2.w;
        result = hgpu_uint_to_double(rnd1,rnd2,XOR7_min,XOR7_max,XOR7_k);
        if ((result>=XOR7_left) && (result<XOR7_right))
            flag = false;
        else
            rnd1 = rnd2;
    }
    (*seed1) = sed1;
    (*seed2) = sed2;
    return result;
}
#endif

__kernel void
xor7(__global uint4* seed_table, 
     __global hgpu_float4* randoms,
       const uint N)
{
    uint giddst = GID;
#ifdef PRECISION_DOUBLE  // if double precision is defined
    hgpu_double4 result;
#else
    float4 result;
    float4 normal = (float4) XOR7_m_FP;
#endif
    uint4 seed1 = seed_table[GID];
    uint4 seed2 = seed_table[GID + GID_SIZE];
    for (uint i = 0; i < N; i++) {
#ifdef PRECISION_DOUBLE // if double precision is defined
        result.x = xor7_step_double(&seed1,&seed2);
        result.y = xor7_step_double(&seed1,&seed2);
        result.z = xor7_step_double(&seed1,&seed2);
        result.w = xor7_step_double(&seed1,&seed2);
        randoms[giddst] = result;
#else
        xor7_step(&seed1,&seed2);
            result.x = (float) seed2.w;
        xor7_step(&seed1,&seed2);
            result.y = (float) seed2.w;
        xor7_step(&seed1,&seed2);
            result.z = (float) seed2.w;
        xor7_step(&seed1,&seed2);
            result.w = (float) seed2.w;
        randoms[giddst] = result / normal;
#endif
        giddst += GID_SIZE;
    }
    seed_table[GID] = seed1;
    seed_table[GID + GID_SIZE] = seed2;
}



#endif
