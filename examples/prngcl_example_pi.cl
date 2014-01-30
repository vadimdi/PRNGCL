/******************************************************************************
 * @file     prngcl_example_pi.cl
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.1.1
 *
 * @brief    [PRNGCL library]
 *           EXAMPLE: OpenCL implementation of MRG32k3a pseudo-random number generator
 *
 *
 * @section  LICENSE
 *
 * Copyright (c) 2014, Vadim Demchik
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

#ifndef PRNGCL_EXAMPLE_PI_CL
#define PRNGCL_EXAMPLE_PI_CL

#include "prngcl_common.cl"

__kernel void
calculate_pi(__global hgpu_float4* randoms,
             __global float* acceptance,
             const uint N)
{
    uint index = GID;
    float count = 0;
    hgpu_float4 rnd,rnd2;

    for (uint i=0; i<N; i++) {
        rnd = randoms[index];
        rnd2 = rnd * rnd;
        if ((rnd2.x+rnd2.y) <= 1.0) count += 1.0;
        if ((rnd2.z+rnd2.w) <= 1.0) count += 1.0;

        index += GID_SIZE;
    }
    acceptance[GID] += count;
}


#endif

