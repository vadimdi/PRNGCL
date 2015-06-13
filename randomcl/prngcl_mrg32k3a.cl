/******************************************************************************
 * @file     prngcl_mrg32k3a.cl
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.1
 *
 * @brief    [PRNGCL library]
 *           contains OpenCL implementation of MRG32k3a pseudo-random number generator
 *
 *
 * @section  CREDITS
 *
 *   Pierre L'Ecuyer,
 *   "Good Parameter Sets for Combined Multiple Recursive Random Number Generators",
 *   Operations Research, 47, 1 (1999), 159--164.
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

#ifndef PRNGCL_MRG32K3A_CL
#define PRNGCL_MRG32K3A_CL

#include "prngcl_common.cl"

#define MRG32k3a_min    (1.0/4294967088.0)
#define MRG32k3a_max    (4294967087.0/4294967088.0)
#define MRG32k3a_k      (2.3283065492957279e-010) // 1/(2^32-208)
#define MRG32k3a_left   (MRG32k3a_min+MRG32k3a_k*MRG32k3a_max)
#define MRG32k3a_right  (MRG32k3a_max+MRG32k3a_k*MRG32k3a_min)

#define MRG32k3a_m     (2.3283065492957276882397381481401e-10) // 1/(2^32-208) for MRG32k3a generator
#define MRG32k3a_m1    4294967087
#define MRG32k3a_m2    4294944443
#define MRG32k3a_a11   1403580L
#define MRG32k3a_a12   810728L
#define MRG32k3a_a21   527612L
#define MRG32k3a_a22   1370589L

//________________________________________________________________________________________________________ MRG32k3a PRNG
__attribute__((always_inline)) void
mrg32k3a_step(uint4* seed1,uint4* seed2,float* rnd)
{
    long x1n, x2n;
    long k;

    x1n = MRG32k3a_a11 * (*seed1).y - MRG32k3a_a12 * (*seed1).x;
    k = x1n / MRG32k3a_m1;
    x1n -= k * MRG32k3a_m1;
    if (x1n < 0) x1n += MRG32k3a_m1;
    (*seed1).x = (*seed1).y;
    (*seed1).y = (*seed1).z;
    (*seed1).z = x1n;

    x2n = MRG32k3a_a21 * (*seed2).z - MRG32k3a_a22 * (*seed2).x;
    k = x2n / MRG32k3a_m2;
    x2n -= k * MRG32k3a_m2;
    if (x2n < 0) x2n += MRG32k3a_m2;
    (*seed2).x = (*seed2).y;
    (*seed2).y = (*seed2).z;
    (*seed2).z = x2n;

    if (x1n>x2n) (*rnd) = (float) ((x1n - x2n) * MRG32k3a_m);
    else         (*rnd) = (float) ((x1n - x2n + MRG32k3a_m1) * MRG32k3a_m);
}

#ifdef PRECISION_DOUBLE  // if double precision is defined
__attribute__((always_inline)) hgpu_double
mrg32k3a_step_double(uint4* seed1,uint4* seed2)
{
    hgpu_double rnd_double;
    float rnd1, rnd2;
    bool flag = true;
    mrg32k3a_step(seed1,seed2,&rnd1);
    while (flag) {
        mrg32k3a_step(seed1,seed2,&rnd2);
        rnd_double = hgpu_float_to_double(rnd1,rnd2,MRG32k3a_min,MRG32k3a_max,MRG32k3a_k);
        if ((rnd_double>=MRG32k3a_left) && (rnd_double<MRG32k3a_right))
            flag = false;
        else
            rnd1 = rnd2;
    }
    return rnd_double;
}
#endif

__kernel void
mrg32k3a(__global uint4* seed_table, 
         __global hgpu_float4* randoms,
            const uint N)
{
    uint giddst = GID;
#ifdef PRECISION_DOUBLE  // if double precision is defined
    float rnd;
    hgpu_double4 result;
#else
    float rnd;
    float4 result;
#endif

    uint4 seed1 = seed_table[GID];
    uint4 seed2 = seed_table[GID + GID_SIZE];

    for (uint i = 0; i < N; i++) {
#ifdef PRECISION_DOUBLE // if double precision is defined
        // MRG32k3a PRNG returns values [1;4294967087]/4294967088, or (0.0;1.0)
        result.x = mrg32k3a_step_double(&seed1,&seed2);
        result.y = mrg32k3a_step_double(&seed1,&seed2);
        result.z = mrg32k3a_step_double(&seed1,&seed2);
        result.w = mrg32k3a_step_double(&seed1,&seed2);
        randoms[giddst] = result;
#else
        mrg32k3a_step(&seed1,&seed2,&rnd);
            result.x = rnd;
        mrg32k3a_step(&seed1,&seed2,&rnd);
            result.y = rnd;
        mrg32k3a_step(&seed1,&seed2,&rnd);
            result.z = rnd;
        mrg32k3a_step(&seed1,&seed2,&rnd);
            result.w = rnd;
        randoms[giddst] = result;
#endif
        giddst += GID_SIZE;
    }
    seed_table[GID] = seed1;
    seed_table[GID + GID_SIZE] = seed2;
}

#endif
