/******************************************************************************
 * @file     prngcl_ranecu.cl
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.1
 *
 * @brief    [PRNGCL library]
 *           contains OpenCL implementation of RANECU pseudo-random number generator
 *
 *
 * @section  CREDITS
 *
 *   F. James,
 *   "A review of pseudorandom number generators",
 *   Computer Physics Communications, 60 (1990), 329--344.
 *
 *   Pierre L'Ecuyer,
 *   "Efficient and Portable Combined Random Number Generators",
 *   Communications of the ACM, 31, 6 (1988), 742--751.
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

#ifndef PRNGCL_RANECU_CL
#define PRNGCL_RANECU_CL

#include "prngcl_common.cl"

#define RANECU_m_FP   (2147483648.0f)
#define RANECU_min    1
#define RANECU_max    2147483647
#define RANECU_min_FP (1.0/2147483648.0)
#define RANECU_max_FP (2147483647.0/2147483648.0)
#define RANECU_k      (4.656612873077392578125E-10) // 1/2^31

#define RANECU_left   (RANECU_min_FP+RANECU_k*RANECU_max_FP)
#define RANECU_right  (RANECU_max_FP+RANECU_k*RANECU_min_FP)

#define RANECU_twom31   (2147483648.0f)
#define RANECU_icons1   2147483563
#define RANECU_icons2   2147483399
#define RANECU_icons3   2147483562
#define RANECU_seedP11  53668
#define RANECU_seedP12  12211
#define RANECU_seedP13  40014
#define RANECU_seedP21  52774
#define RANECU_seedP22  3791
#define RANECU_seedP23  40692


//________________________________________________________________________________________________________ RANECU PRNG
__attribute__((always_inline)) void
ranecu_step(uint4* seed1,uint4* seed2,uint4* rnd)
{
	uint4 k = (*seed1) / ((uint4) RANECU_seedP11);
    uint4 RANECU_test_1 = ((uint4) RANECU_seedP13) * ((*seed1) - k * ((uint4) RANECU_seedP11));
    uint4 RANECU_test_2 = k * ((uint4) RANECU_seedP12);

    if (RANECU_test_1.x > RANECU_test_2.x) (*seed1).x = RANECU_test_1.x - RANECU_test_2.x; else (*seed1).x = RANECU_test_1.x - RANECU_test_2.x + RANECU_icons1;
    if (RANECU_test_1.y > RANECU_test_2.y) (*seed1).y = RANECU_test_1.y - RANECU_test_2.y; else (*seed1).y = RANECU_test_1.y - RANECU_test_2.y + RANECU_icons1;
    if (RANECU_test_1.z > RANECU_test_2.z) (*seed1).z = RANECU_test_1.z - RANECU_test_2.z; else (*seed1).z = RANECU_test_1.z - RANECU_test_2.z + RANECU_icons1;
    if (RANECU_test_1.w > RANECU_test_2.w) (*seed1).w = RANECU_test_1.w - RANECU_test_2.w; else (*seed1).w = RANECU_test_1.w - RANECU_test_2.w + RANECU_icons1;

	k = (*seed2) / ((uint4) RANECU_seedP21);

    RANECU_test_1 = ((uint4) RANECU_seedP23) * ((*seed2) - k * ((uint4) RANECU_seedP21));
    RANECU_test_2 = k * ((uint4) RANECU_seedP22);

    if (RANECU_test_1.x > RANECU_test_2.x) (*seed2).x = RANECU_test_1.x - RANECU_test_2.x; else (*seed2).x = RANECU_test_1.x - RANECU_test_2.x + RANECU_icons2;
    if (RANECU_test_1.y > RANECU_test_2.y) (*seed2).y = RANECU_test_1.y - RANECU_test_2.y; else (*seed2).y = RANECU_test_1.y - RANECU_test_2.y + RANECU_icons2;
    if (RANECU_test_1.z > RANECU_test_2.z) (*seed2).z = RANECU_test_1.z - RANECU_test_2.z; else (*seed2).z = RANECU_test_1.z - RANECU_test_2.z + RANECU_icons2;
    if (RANECU_test_1.w > RANECU_test_2.w) (*seed2).w = RANECU_test_1.w - RANECU_test_2.w; else (*seed2).w = RANECU_test_1.w - RANECU_test_2.w + RANECU_icons2;

    if ((*seed1).x > (*seed2).x) (*rnd).x = (*seed1).x - (*seed2).x; else (*rnd).x = (*seed1).x - (*seed2).x + RANECU_icons3;
    if ((*seed1).y > (*seed2).y) (*rnd).y = (*seed1).y - (*seed2).y; else (*rnd).y = (*seed1).y - (*seed2).y + RANECU_icons3;
    if ((*seed1).z > (*seed2).z) (*rnd).z = (*seed1).z - (*seed2).z; else (*rnd).z = (*seed1).z - (*seed2).z + RANECU_icons3;
    if ((*seed1).w > (*seed2).w) (*rnd).w = (*seed1).w - (*seed2).w; else (*rnd).w = (*seed1).w - (*seed2).w + RANECU_icons3;
}

#ifdef PRECISION_DOUBLE  // if double precision is defined
__attribute__((always_inline)) hgpu_double4
ranecu_step_double(uint4* seed1,uint4* seed2)
{
    hgpu_double4 result;
    uint4 rnd1, rnd2;
    uint4 sed1 = (*seed1);
    uint4 sed2 = (*seed2);
    bool flag = true;
    ranecu_step(&sed1,&sed2,&rnd1);
    while (flag) {
        ranecu_step(&sed1,&sed2,&rnd2);
        result = hgpu_uint4_to_double4(rnd1,rnd2,RANECU_min,RANECU_max,RANECU_k);
        if ((result.x>=RANECU_left) && (result.x<RANECU_right) &&
            (result.y>=RANECU_left) && (result.y<RANECU_right) &&
            (result.z>=RANECU_left) && (result.z<RANECU_right) &&
            (result.w>=RANECU_left) && (result.w<RANECU_right))
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
ranecu(__global uint4* seed_table, 
       __global hgpu_float4* randoms,
          const uint N)
{
    uint giddst = GID;

    uint4 seed1 = seed_table[GID];
    uint4 seed2 = seed_table[GID + GID_SIZE];

#ifdef PRECISION_DOUBLE  // if double precision is defined
    uint4 rnd1, rnd2;
    hgpu_double4 result;
#else
    uint4 result;
    float4 normal = (float4) RANECU_twom31;
#endif

    for (uint i = 0; i < N; i++) {
#ifdef PRECISION_DOUBLE // if double precision is defined
        result = ranecu_step_double(&seed1,&seed2);
        randoms[giddst] = result;
#else
        ranecu_step(&seed1,&seed2,&result);
        randoms[giddst] = hgpu_uint4_to_float4(result) / normal;
#endif
        giddst += GID_SIZE;
    }
    seed_table[GID] = seed1;
    seed_table[GID + GID_SIZE] = seed2;
}

#endif
 