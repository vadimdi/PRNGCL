/******************************************************************************
 * @file     prngcl_ranmar.cl
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.1
 *
 * @brief    [PRNGCL library]
 *           contains OpenCL implementation of RANMAR pseudo-random number generator
 *
 *
 * @section  CREDITS
 *
 *   George Marsaglia, Arif Zaman and Wai Wan Tsang,
 *   "Toward a universal random number generator",
 *   Statistics & Probability Letters, 9 (1990), 35--39.
 *
 *   F. James,
 *   "A review of pseudorandom number generators",
 *   Computer Physics Communications, 60 (1990), 329--344.
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

#ifndef PRNGCL_RANMAR_CL
#define PRNGCL_RANMAR_CL

#include "prngcl_common.cl"

#define RM_min    (1.0/16777216.0)
#define RM_max    (16777215.0/16777216.0)
#define RM_k      (5.9604644775390625E-8) // 1/2^24                   

#define RM_left   (RM_min+RM_k*RM_max)
#define RM_right  (RM_max+RM_k*RM_min)

#define RM_CD (7654321.0f / 16777216.0f)
#define RM_CM (16777213.0f /16777216.0f)

//________________________________________________________________________________________________________ RANMAR PRNG
__kernel void
ranmar_init(const __global uint4* seeds, __global float4* seedtable)
{
    float4 seed;
    //
    uint4 RM_seed1 = seeds[GID];
    uint4 RM_seed2 = seeds[GID + GID_SIZE];
    //
    uint4 i = ((RM_seed1 / 177) % 177) + 2;
    uint4 j = (RM_seed1 % 177) + 2;
    uint4 k = ((RM_seed2 / 169) % 178) + 1;
    uint4 l = (RM_seed2 % 169);
    for (int n = 0; n < 97; n++)
    {
        uint4 s = (uint4) 0;
        uint  t = 8388608;
        for (int m = 0; m < 24;  m++)
        {
            uint4 u = (i * j) % 179;
            u = (u * k) % 179;
            i = j;
            j = k;
            k = u;
            l = (53 * l + 1) % 169;
            uint4 lus = (l * u) % 64;

            if ( lus.x >= 32) s.x += t;
            if ( lus.y >= 32) s.y += t;
            if ( lus.z >= 32) s.z += t;
            if ( lus.w >= 32) s.w += t;
            t = t >> 1;
        }
        seed.x = ((float) s.x);
        seed.y = ((float) s.y);
        seed.z = ((float) s.z);
        seed.w = ((float) s.w);
        seedtable[GID + GID_SIZE * n] = seed / ((float4) 16777216.0f);
	}
    Uint_and_Float indx_I97, indx_J97;
    indx_I97.uint_value = 96;
    indx_J97.uint_value = 32;
    float4 indx;
    indx.x = indx_I97.float_value;
    indx.y = indx_J97.float_value;
    indx.z = (362436.0f / 16777216.0f);
    indx.w = 0.0f;

    seedtable[GID + GID_SIZE * 97] = indx;   // (RM_I97,RM_J97,RM_C,0)
}

__attribute__((always_inline)) float4
rm_step(__global float4 * seedtable,uint* RM_I97,uint* RM_J97,float* uniz)
{
        float4 uni;
        uni = seedtable[GID + GID_SIZE * (*RM_I97)] - seedtable[GID + GID_SIZE * (*RM_J97)];
	    uni = select(uni, (uni + 1.0f), uni < (float4) 0.0f);
        seedtable[GID + GID_SIZE * (*RM_I97)] = uni;

	    if ((*RM_I97) == 0) (*RM_I97) = 97;
	    if ((*RM_J97) == 0) (*RM_J97) = 97;
	    (*RM_I97)--;
	    (*RM_J97)--;

        (*uniz) -= RM_CD;
	    if ((*uniz) < 0.0) {(*uniz) += RM_CM;}
        uni -= (*uniz);
	    uni = select(uni, (uni + 1.0f), uni < (float4) 0.0f);
        return uni;
}

#ifdef PRECISION_DOUBLE  // if double precision is defined
__attribute__((always_inline)) hgpu_double4
rm_step_double(__global float4 * seedtable,uint* RM_I97,uint* RM_J97,float* uniz)
{
    hgpu_double4 result;
    float4 rnd1, rnd2;
    bool flag = true;
    rnd1 = rm_step(seedtable,RM_I97,RM_J97,uniz);
    while (flag) {
        rnd2 = rm_step(seedtable,RM_I97,RM_J97,uniz);
        result = hgpu_float4_to_double4(rnd1,rnd2,RM_min,RM_max,RM_k);
        if ((result.x>=RM_left) && (result.x<RM_right) &&
            (result.y>=RM_left) && (result.y<RM_right) &&
            (result.z>=RM_left) && (result.z<RM_right) &&
            (result.w>=RM_left) && (result.w<RM_right))
            flag = false;
        else
            rnd1 = rnd2;
    }

    return result;
}
#endif

__kernel void
ranmar(__global float4 * seedtable,__global hgpu_float4 * prns, const uint samples)
{
    uint giddst = GID;
    float4 uni = (float4) 0.0;
    Uint_and_Float indx_I97, indx_J97;
    float4 indx = seedtable[GID + GID_SIZE * 97];
    indx_I97.float_value = indx.x;
    indx_J97.float_value = indx.y;
    uint RM_I97 = indx_I97.uint_value;
    uint RM_J97 = indx_J97.uint_value;
    float uniz = indx.z;
    for (uint i=0; i<samples; i++) {
#ifdef PRECISION_DOUBLE // if double precision is defined
        hgpu_double4 result = rm_step_double(seedtable,&RM_I97,&RM_J97,&uniz);
        prns[giddst] = result;
#else
        uni = rm_step(seedtable,&RM_I97,&RM_J97,&uniz);
        prns[giddst] = uni;
#endif
        giddst += GID_SIZE;
    }

    indx_I97.uint_value = RM_I97;
    indx_J97.uint_value = RM_J97;
    indx.z = uniz;
    indx.x = indx_I97.float_value;
    indx.y = indx_J97.float_value;
    seedtable[GID + GID_SIZE * 97] = indx;
}


#endif
  