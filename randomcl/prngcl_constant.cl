/******************************************************************************
 * @file     prngcl_constant.cl
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.1
 *
 * @brief    [PRNGCL library]
 *           contains OpenCL implementation of constant "pseudo-random number generator",
 *           which produces series of constant values for debugging purposes
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

#ifndef PRNGCL_CONSTANT_CL
#define PRNGCL_CONSTANT_CL

#include "prngcl_common.cl"

/**
 * generates the series of constant values predefined by CONSTANT_FP parameter
 * @param randoms output buffer for generated PRNs
 * @param N number of PRNs to be produced
 */
__kernel void
constant_series(__global hgpu_float4* randoms,
                const uint N)
{
    uint giddst = GID;
        for (uint i = 0; i < N; i++) {
        randoms[giddst] = (hgpu_float4) CONSTANT_FP;
        giddst += GID_SIZE;
    }
}


#endif
