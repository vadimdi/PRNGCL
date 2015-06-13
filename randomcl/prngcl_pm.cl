/******************************************************************************
 * @file     prngcl_pm.cl
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.1
 *
 * @brief    [PRNGCL library]
 *           contains OpenCL implementation of Park-Miller pseudo-random number generator
 *
 *
 * @section  CREDITS
 *
 *   Stephen K. Park, Keith W. Miller,
 *   "Random number generators: good ones are hard to find",
 *   Communications of the ACM {\bf 31} (10), 1192 (1988).
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

#ifndef PRNGCL_PM_CL
#define PRNGCL_PM_CL

#include "prngcl_common.cl"

#define PM_m_FP   (2147483647.0)
#define PM_min    1
#define PM_max    2147483646
#define PM_k      (4.6566128752457969241058E-10) // 1/(2^31-1)

#define PM_m     2147483647
#define PM_a     16807
#define PM_q     127773         // (PM_m div PM_a)
#define PM_r     2836           // (PM_m mod PM_a)

//________________________________________________________________________________________________________ PARK-MILLER PRNG
//
// Stephen K. Park and Keith W. Miller
// "Random number generators: good ones are hard to find"
// Communications of the ACM 31 (10), 1192 (1988).
//

__attribute__((always_inline)) void
pm_step(uint4* seed)
//
// produces PRNs in range [1;2147483646]
// if seed=1 then 10001th = 1043618065
//
{
    uint4 PM_test_1, PM_test_2;

    PM_test_1 = ((uint) PM_a) * ((*seed) % PM_q);
    PM_test_2 = ((uint) PM_r) * ((*seed) / PM_q);
    
    if (PM_test_1.x > PM_test_2.x) (*seed).x = PM_test_1.x - PM_test_2.x; else (*seed).x = PM_test_1.x - PM_test_2.x + (uint) PM_m;
    if (PM_test_1.y > PM_test_2.y) (*seed).y = PM_test_1.y - PM_test_2.y; else (*seed).y = PM_test_1.y - PM_test_2.y + (uint) PM_m;
    if (PM_test_1.z > PM_test_2.z) (*seed).z = PM_test_1.z - PM_test_2.z; else (*seed).z = PM_test_1.z - PM_test_2.z + (uint) PM_m;
    if (PM_test_1.w > PM_test_2.w) (*seed).w = PM_test_1.w - PM_test_2.w; else (*seed).w = PM_test_1.w - PM_test_2.w + (uint) PM_m;
}

__kernel void
pm(__global uint4* seed_table, 
   __global hgpu_float4* randoms,
      const uint N)
{
    uint giddst = GID;
#ifdef PRECISION_DOUBLE  // if double precision is defined
    uint4 rnd1, rnd2;
    hgpu_double4 result;
#else
    float4 normal = (float4) PM_m_FP;
#endif
    uint4 seed = seed_table[GID];
    for (uint i = 0; i < N; i++) {
#ifdef PRECISION_DOUBLE // if double precision is defined

        bool flag = true;
        while (flag) {
            pm_step(&seed);
            rnd1 = seed;
            if ((rnd1.x>PM_min) && (rnd1.x<PM_max) &&
                (rnd1.y>PM_min) && (rnd1.y<PM_max) &&
                (rnd1.z>PM_min) && (rnd1.z<PM_max) &&
                (rnd1.w>PM_min) && (rnd1.w<PM_max)) {
                pm_step(&seed);
                rnd2 = seed;
                result = hgpu_uint4_to_double4(rnd1,rnd2,PM_min,PM_max,PM_k);
                flag = false;
            }
        }
        randoms[giddst] = result;
#else
        pm_step(&seed);
        randoms[giddst] = hgpu_uint4_to_float4(seed) / normal;
#endif
        giddst += GID_SIZE;
    }
    seed_table[GID] = seed;
}



#endif
