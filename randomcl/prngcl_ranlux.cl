/******************************************************************************
 * @file     prngcl_ranlux.cl
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.1
 *
 * @brief    [PRNGCL library]
 *           contains OpenCL implementation of RANLUX pseudo-random number generator
 *
 *
 * @section  CREDITS
 *
 *   Martin Luscher,
 *   "A portable high-quality random number generator for lattice field theory simulations",
 *   Computer Physics Communications, 79 (1994), 100--110.
 *
 *   F. James,
 *   "RANLUX: A Fortran implementation of the high-quality pseudorandom number generator of Luscher",
 *   Computer Physics Communications, 79 (1994), 111--114.
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

#ifndef PRNGCL_RANLUX_CL
#define PRNGCL_RANLUX_CL

#include "prngcl_common.cl"

#define RL_min    (1.0/16777216.0)
#define RL_max    (16777215.0/16777216.0)
#define RL_k      (5.9604644775390625E-8) // 1/2^24

#define RL_left   (RL_min+RL_k*RL_max)
#define RL_right  (RL_max+RL_k*RL_min)

#define RL_zero     0.0f
#define RL_icons    2147483563
#define RL_itwo24   16777216 // 1<<24
#define RL_twom24   0.000000059604644775390625f
#define RL_twom12   0.000244140625f
#define RL_twom24sq RL_twom24*RL_twom24

#ifndef RL_skip
#define RL_skip     223
#endif

#define RL_skip_24 (RL_skip / 24)
#if RL_skip == 97
#define RL_RANLUX2
#define RL_skip_4   0
#define RL_skip_1   0
#endif
#if RL_skip == 223
#define RL_RANLUX3
#define RL_skip_4   0
#define RL_skip_1   0
#endif
#if RL_skip == 389
#define RL_RANLUX4
#define RL_skip_4   0
#define RL_skip_1   0
#endif
#ifndef RL_skip_4
#define RL_skip_4  ((RL_skip - RL_skip_24 * 24) / 4)
#endif
#ifndef RL_skip_1
#define RL_skip_1  (RL_skip % 4)
#endif


//________________________________________________________________________________________________________ RANLUX PRNG
__attribute__((always_inline)) int4
rlseedint(int RL_jseed)
{
	int4 RL_output_int;
		int k = RL_jseed / 53668;
		RL_jseed = 40014 * (RL_jseed - k * 53668) - k * 12211;
		if (RL_jseed < 0) {RL_jseed = RL_jseed + RL_icons;}
		RL_output_int.x = RL_jseed;

		k = RL_jseed / 53668;
		RL_jseed = 40014 * (RL_jseed - k * 53668) - k * 12211;
		if (RL_jseed < 0) {RL_jseed = RL_jseed + RL_icons;}
		RL_output_int.y = RL_jseed;

		k = RL_jseed / 53668;
		RL_jseed = 40014 * (RL_jseed - k * 53668) - k * 12211;
		if (RL_jseed < 0) {RL_jseed = RL_jseed + RL_icons;}
		RL_output_int.z = RL_jseed;

		k = RL_jseed / 53668;
		RL_jseed = 40014 * (RL_jseed - k * 53668) - k * 12211;
		if (RL_jseed < 0) {RL_jseed = RL_jseed + RL_icons;}
		RL_output_int.w = RL_jseed;

    return RL_output_int;
}

__attribute__((always_inline)) float4
rlseedint2(int4 RL_temp)
{
	float4 RL_output_int;
	float4 RL_tmp = (float4) (RL_twom24,RL_twom24,RL_twom24,RL_twom24);
		RL_output_int.x = ((float) (RL_temp.x % RL_itwo24));
		RL_output_int.y = ((float) (RL_temp.y % RL_itwo24));
		RL_output_int.z = ((float) (RL_temp.z % RL_itwo24));
		RL_output_int.w = ((float) (RL_temp.w % RL_itwo24));
	RL_output_int = RL_output_int * RL_tmp;
    return RL_output_int;
}

__kernel void
ranlux_init(const __global uint * seeds, __global float4 * seedtable)
{
    int RL_jseed = (int) seeds[GID];
	int4		RL_temp1 = rlseedint(RL_jseed);
	seedtable[GID + 0 * GID_SIZE] = rlseedint2(RL_temp1);
				RL_temp1 = rlseedint(RL_temp1.w);
	seedtable[GID + 1 * GID_SIZE] = rlseedint2(RL_temp1);
				RL_temp1 = rlseedint(RL_temp1.w);
	seedtable[GID + 2 * GID_SIZE] = rlseedint2(RL_temp1);
				RL_temp1 = rlseedint(RL_temp1.w);
	seedtable[GID + 3 * GID_SIZE] = rlseedint2(RL_temp1);
				RL_temp1 = rlseedint(RL_temp1.w);
	seedtable[GID + 4 * GID_SIZE] = rlseedint2(RL_temp1);
				RL_temp1 = rlseedint(RL_temp1.w);
	float4		RL_temp2 = rlseedint2(RL_temp1);
	seedtable[GID + 5 * GID_SIZE] = RL_temp2;
	float4		RL_carin = (float4) (23.0f, 9.0f, 6.0f, 0.0f);
	if (RL_temp2.w == 0) {RL_carin.w = RL_twom24;}
	seedtable[GID + 6 * GID_SIZE] = RL_carin; // RL_i24, RL_j24, RL_in24, RL_carry
}

__attribute__((always_inline)) float4
rlproducefloat4(float4 * RL_seeds_i24, const float4 RL_seeds_j24, const float RL_seeds_j24P, float * RL_carry)
{
	float RL_c = *RL_carry;
	float4 RL_output_int = RL_seeds_j24 - (*RL_seeds_i24).wzyx;
	RL_output_int.x = RL_output_int.x - RL_c;
		if (RL_output_int.x < 0.0f) { RL_output_int.x = RL_output_int.x + 1.0f; RL_c = RL_twom24; } else { RL_c = 0.0f; }
	RL_output_int.y = RL_output_int.y - RL_c;
		if (RL_output_int.y < 0.0f) { RL_output_int.y = RL_output_int.y + 1.0f; RL_c = RL_twom24; } else { RL_c = 0.0f; }
	RL_output_int.z = RL_output_int.z - RL_c;
		if (RL_output_int.z < 0.0f) { RL_output_int.z = RL_output_int.z + 1.0f; RL_c = RL_twom24; } else { RL_c = 0.0f; }
	RL_output_int.w = RL_output_int.w - RL_c;
		if (RL_output_int.w < 0.0f) { RL_output_int.w = RL_output_int.w + 1.0f; RL_c = RL_twom24; } else { RL_c = 0.0f; }

	*RL_seeds_i24 = RL_output_int.wzyx;

    float4 RL_temporary = RL_seeds_j24.yzwx;
    RL_temporary.w = RL_seeds_j24P;

	float4 RL_output_int2 = select(RL_output_int, (RL_output_int + RL_twom24 * RL_temporary), RL_output_int < (float4) RL_twom12);
			RL_output_int = select(RL_output_int2, RL_twom24sq, RL_output_int2 == (float4) 0.0f);

	*RL_carry = RL_c;
	return RL_output_int;
}

__attribute__((always_inline)) void
rlproduceupd4(float4 * RL_seeds_i24, const float4 RL_seeds_j24, float * RL_carry)
{
	float RL_c = *RL_carry;
	float4 RL_output_int = RL_seeds_j24 - (*RL_seeds_i24).wzyx;
	RL_output_int.x = RL_output_int.x - RL_c;
		if (RL_output_int.x < 0.0f) { RL_output_int.x = RL_output_int.x + 1.0f; RL_c = RL_twom24; } else { RL_c = 0.0f; }
	RL_output_int.y = RL_output_int.y - RL_c;
		if (RL_output_int.y < 0.0f) { RL_output_int.y = RL_output_int.y + 1.0f; RL_c = RL_twom24; } else { RL_c = 0.0f; }
	RL_output_int.z = RL_output_int.z - RL_c;
		if (RL_output_int.z < 0.0f) { RL_output_int.z = RL_output_int.z + 1.0f; RL_c = RL_twom24; } else { RL_c = 0.0f; }
	RL_output_int.w = RL_output_int.w - RL_c;
		if (RL_output_int.w < 0.0f) { RL_output_int.w = RL_output_int.w + 1.0f; RL_c = RL_twom24; } else { RL_c = 0.0f; }

	*RL_seeds_i24 = RL_output_int.wzyx;
	*RL_carry = RL_c;
}

__attribute__((always_inline)) float4
rl_step(float4* RL_seed0,float4* RL_seed1,float4* RL_seed2,float4* RL_seed3,
        float4* RL_seed4,float4* RL_seed5,float4* RL_carin)
{
	float4 RL_seeds_j24, uni, RL_seed_temp, RL_seed_temp2;
 	float RL_carry = (*RL_carin).w;
		if ((*RL_carin).z >= 6.0f) {
			for (int t=0; t<RL_skip_24; t++) {
				RL_seeds_j24.xy = (*RL_seed2).yx;
				RL_seeds_j24.zw = (*RL_seed1).wz;
					rlproduceupd4(RL_seed5,RL_seeds_j24,&RL_carry);
				RL_seeds_j24.xy = (*RL_seed1).yx;
				RL_seeds_j24.zw = (*RL_seed0).wz;
					rlproduceupd4(RL_seed4,RL_seeds_j24,&RL_carry);
				RL_seeds_j24.xy = (*RL_seed0).yx;
				RL_seeds_j24.zw = (*RL_seed5).wz;
					rlproduceupd4(RL_seed3,RL_seeds_j24,&RL_carry);
				RL_seeds_j24.xy = (*RL_seed5).yx;
				RL_seeds_j24.zw = (*RL_seed4).wz;
					rlproduceupd4(RL_seed2,RL_seeds_j24,&RL_carry);
				RL_seeds_j24.xy = (*RL_seed4).yx;
				RL_seeds_j24.zw = (*RL_seed3).wz;
					rlproduceupd4(RL_seed1,RL_seeds_j24,&RL_carry);
				RL_seeds_j24.xy = (*RL_seed3).yx;
				RL_seeds_j24.zw = (*RL_seed2).wz;
					rlproduceupd4(RL_seed0,RL_seeds_j24,&RL_carry);
			}
#if RL_skip_4 == 1
				RL_seeds_j24.xy = (*RL_seed2).yx;
				RL_seeds_j24.zw = (*RL_seed1).wz;
					rlproduceupd4(RL_seed5,RL_seeds_j24,&RL_carry);
                RL_seed_temp = (*RL_seed5);    // 5->4, 4->3, 3->2, 2->1, 1->0, 0->5
                (*RL_seed5) = (*RL_seed4);
                (*RL_seed4) = (*RL_seed3);
                (*RL_seed3) = (*RL_seed2);
                (*RL_seed2) = (*RL_seed1);
                (*RL_seed1) = (*RL_seed0);
                (*RL_seed0) = RL_seed_temp;
#elif RL_skip_4 == 2
				RL_seeds_j24.xy = (*RL_seed2).yx;
				RL_seeds_j24.zw = (*RL_seed1).wz;
					rlproduceupd4(RL_seed5,RL_seeds_j24,&RL_carry);
				RL_seeds_j24.xy = (*RL_seed1).yx;
				RL_seeds_j24.zw = (*RL_seed0).wz;
					rlproduceupd4(RL_seed4,RL_seeds_j24,&RL_carry);
                RL_seed_temp = (*RL_seed5);    // 5->3, 4->2, 3->1, 2->0, 1->5, 0->4
                (*RL_seed5) = (*RL_seed3);
                (*RL_seed3) = (*RL_seed1);
                (*RL_seed1) = RL_seed_temp;
                RL_seed_temp = (*RL_seed2);
                (*RL_seed2) = (*RL_seed0);
                (*RL_seed0) = (*RL_seed4);
                (*RL_seed4) = RL_seed_temp;
#elif RL_skip_4 == 3
				RL_seeds_j24.xy = (*RL_seed2).yx;
				RL_seeds_j24.zw = (*RL_seed1).wz;
					rlproduceupd4(RL_seed5,RL_seeds_j24,&RL_carry);
				RL_seeds_j24.xy = (*RL_seed1).yx;
				RL_seeds_j24.zw = (*RL_seed0).wz;
					rlproduceupd4(RL_seed4,RL_seeds_j24,&RL_carry);
				RL_seeds_j24.xy = (*RL_seed0).yx;
				RL_seeds_j24.zw = (*RL_seed5).wz;
					rlproduceupd4(RL_seed3,RL_seeds_j24,&RL_carry);
                RL_seed_temp = (*RL_seed5);    // 5->2, 4->1, 3->0, 2->5, 1->4, 0->3
                (*RL_seed5) = (*RL_seed2);
                (*RL_seed2) = RL_seed_temp;
                RL_seed_temp = (*RL_seed4);
                (*RL_seed4) = (*RL_seed1);
                (*RL_seed1) = RL_seed_temp;
                RL_seed_temp = (*RL_seed3);
                (*RL_seed3) = (*RL_seed0);
                (*RL_seed0) = RL_seed_temp;
#elif RL_skip_4 == 4
				RL_seeds_j24.xy = (*RL_seed2).yx;
				RL_seeds_j24.zw = (*RL_seed1).wz;
					rlproduceupd4(RL_seed5,RL_seeds_j24,&RL_carry);
				RL_seeds_j24.xy = (*RL_seed1).yx;
				RL_seeds_j24.zw = (*RL_seed0).wz;
					rlproduceupd4(RL_seed4,RL_seeds_j24,&RL_carry);
				RL_seeds_j24.xy = (*RL_seed0).yx;
				RL_seeds_j24.zw = (*RL_seed5).wz;
					rlproduceupd4(RL_seed3,RL_seeds_j24,&RL_carry);
				RL_seeds_j24.xy = (*RL_seed5).yx;
				RL_seeds_j24.zw = (*RL_seed4).wz;
					rlproduceupd4(RL_seed2,RL_seeds_j24,&RL_carry);
                RL_seed_temp = (*RL_seed5);    // 5->1, 4->0, 3->5, 2->4, 1->3, 0->2
                (*RL_seed5) = (*RL_seed1);
                (*RL_seed1) = (*RL_seed3);
                (*RL_seed3) = RL_seed_temp;
                RL_seed_temp = (*RL_seed4);
                (*RL_seed4) = (*RL_seed0);
                (*RL_seed0) = (*RL_seed2);
                (*RL_seed2) = RL_seed_temp;
#elif RL_skip_4 == 5
				RL_seeds_j24.xy = (*RL_seed2).yx;
				RL_seeds_j24.zw = (*RL_seed1).wz;
					rlproduceupd4(RL_seed5,RL_seeds_j24,&RL_carry);
				RL_seeds_j24.xy = (*RL_seed1).yx;
				RL_seeds_j24.zw = (*RL_seed0).wz;
					rlproduceupd4(RL_seed4,RL_seeds_j24,&RL_carry);
				RL_seeds_j24.xy = (*RL_seed0).yx;
				RL_seeds_j24.zw = (*RL_seed5).wz;
					rlproduceupd4(RL_seed3,RL_seeds_j24,&RL_carry);
				RL_seeds_j24.xy = (*RL_seed5).yx;
				RL_seeds_j24.zw = (*RL_seed4).wz;
					rlproduceupd4(RL_seed2,RL_seeds_j24,&RL_carry);
				RL_seeds_j24.xy = (*RL_seed4).yx;
				RL_seeds_j24.zw = (*RL_seed3).wz;
					rlproduceupd4(RL_seed1,RL_seeds_j24,&RL_carry);
                RL_seed_temp = (*RL_seed5);    // 5->0, 4->5, 3->4, 2->3, 1->2, 0->1
                (*RL_seed5) = (*RL_seed0);
                (*RL_seed0) = (*RL_seed1);
                (*RL_seed1) = (*RL_seed2);
                (*RL_seed2) = (*RL_seed3);
                (*RL_seed3) = (*RL_seed4);
                (*RL_seed4) = RL_seed_temp;
#endif
#if RL_skip_1 == 1
                float RL_output_int = (*RL_seed2).y - (*RL_seed5).w;
                RL_output_int = RL_output_int - RL_carry;
                    if (RL_output_int < 0.0f) { RL_output_int = RL_output_int + 1.0f; RL_carry = RL_twom24; } else { RL_carry = 0.0f; }
                (*RL_seed5).w = RL_output_int;

                RL_seed_temp = (*RL_seed5);
                (*RL_seed5).yzw = (*RL_seed5).xyz;
                (*RL_seed5).x = (*RL_seed4).w;
                (*RL_seed4).yzw = (*RL_seed4).xyz;
                (*RL_seed4).x = (*RL_seed3).w;
                (*RL_seed3).yzw = (*RL_seed3).xyz;
                (*RL_seed3).x = (*RL_seed2).w;
                (*RL_seed2).yzw = (*RL_seed2).xyz;
                (*RL_seed2).x = (*RL_seed1).w;
                (*RL_seed1).yzw = (*RL_seed1).xyz;
                (*RL_seed1).x = (*RL_seed0).w;
                (*RL_seed0).yzw = (*RL_seed0).xyz;
                (*RL_seed0).x = RL_seed_temp.w;
#elif RL_skip_1 == 2
                float4 RL_output_int;
                RL_output_int.xy = (*RL_seed2).yx - (*RL_seed5).wz;
                RL_output_int.x = RL_output_int.x - RL_carry;
                    if (RL_output_int.x < 0.0f) { RL_output_int.x = RL_output_int.x + 1.0f; RL_carry = RL_twom24; } else { RL_carry = 0.0f; }
                RL_output_int.y = RL_output_int.y - RL_carry;
                    if (RL_output_int.y < 0.0f) { RL_output_int.y = RL_output_int.y + 1.0f; RL_carry = RL_twom24; } else { RL_carry = 0.0f; }

                (*RL_seed5).zw = RL_output_int.yx;

                RL_seed_temp = (*RL_seed5);
                (*RL_seed5).zw = (*RL_seed5).xy;
                (*RL_seed5).xy = (*RL_seed4).zw;
                (*RL_seed4).zw = (*RL_seed4).xy;
                (*RL_seed4).xy = (*RL_seed3).zw;
                (*RL_seed3).zw = (*RL_seed3).xy;
                (*RL_seed3).xy = (*RL_seed2).zw;
                (*RL_seed2).zw = (*RL_seed2).xy;
                (*RL_seed2).xy = (*RL_seed1).zw;
                (*RL_seed1).zw = (*RL_seed1).xy;
                (*RL_seed1).xy = (*RL_seed0).zw;
                (*RL_seed0).zw = (*RL_seed0).xy;
                (*RL_seed0).xy = RL_seed_temp.zw;
#elif RL_skip_1 == 3
                float4 RL_output_int;
                RL_output_int.xy = (*RL_seed2).yx - (*RL_seed5).wz;
                RL_output_int.z = (*RL_seed1).w - (*RL_seed5).y;
                RL_output_int.x = RL_output_int.x - RL_carry;
                    if (RL_output_int.x < 0.0f) { RL_output_int.x = RL_output_int.x + 1.0f; RL_carry = RL_twom24; } else { RL_carry = 0.0f; }
                RL_output_int.y = RL_output_int.y - RL_carry;
                    if (RL_output_int.y < 0.0f) { RL_output_int.y = RL_output_int.y + 1.0f; RL_carry = RL_twom24; } else { RL_carry = 0.0f; }
                RL_output_int.z = RL_output_int.z - RL_carry;
                    if (RL_output_int.z < 0.0f) { RL_output_int.z = RL_output_int.z + 1.0f; RL_carry = RL_twom24; } else { RL_carry = 0.0f; }

                (*RL_seed5).yzw = RL_output_int.zyx;

                RL_seed_temp = (*RL_seed5);
                (*RL_seed5).w = (*RL_seed5).x;
                (*RL_seed5).xyz = (*RL_seed4).yzw;
                (*RL_seed4).w = (*RL_seed4).x;
                (*RL_seed4).xyz = (*RL_seed3).yzw;
                (*RL_seed3).w = (*RL_seed3).x;
                (*RL_seed3).xyz = (*RL_seed2).yzw;
                (*RL_seed2).w = (*RL_seed2).x;
                (*RL_seed2).xyz = (*RL_seed1).yzw;
                (*RL_seed1).w = (*RL_seed1).x;
                (*RL_seed1).xyz = (*RL_seed0).yzw;
                (*RL_seed0).w = (*RL_seed0).x;
                (*RL_seed0).xyz = RL_seed_temp.yzw;
#endif
#ifdef RL_RANLUX2
                float RL_output_int = (*RL_seed2).y - (*RL_seed5).w;
                RL_output_int = RL_output_int - RL_carry;
                    if (RL_output_int < 0.0f) { RL_output_int = RL_output_int + 1.0f; RL_carry = RL_twom24; } else { RL_carry = 0.0f; }
                (*RL_seed5).w = RL_output_int;

                RL_seed_temp = (*RL_seed5);
                (*RL_seed5).yzw = (*RL_seed5).xyz;
                (*RL_seed5).x = (*RL_seed4).w;
                (*RL_seed4).yzw = (*RL_seed4).xyz;
                (*RL_seed4).x = (*RL_seed3).w;
                (*RL_seed3).yzw = (*RL_seed3).xyz;
                (*RL_seed3).x = (*RL_seed2).w;
                (*RL_seed2).yzw = (*RL_seed2).xyz;
                (*RL_seed2).x = (*RL_seed1).w;
                (*RL_seed1).yzw = (*RL_seed1).xyz;
                (*RL_seed1).x = (*RL_seed0).w;
                (*RL_seed0).yzw = (*RL_seed0).xyz;
                (*RL_seed0).x = RL_seed_temp.w;
#endif
#ifdef RL_RANLUX3
                RL_seeds_j24.xy = (*RL_seed2).yx;
                RL_seeds_j24.zw = (*RL_seed1).wz;
                    rlproduceupd4(RL_seed5,RL_seeds_j24,&RL_carry);
                float4 RL_output_int;
                RL_output_int.xy = (*RL_seed1).yx - (*RL_seed4).wz;
                RL_output_int.z = (*RL_seed0).w - (*RL_seed4).y;
                RL_output_int.x = RL_output_int.x - RL_carry;
                    if (RL_output_int.x < 0.0f) { RL_output_int.x = RL_output_int.x + 1.0f; RL_carry = RL_twom24; } else { RL_carry = 0.0f; }
                RL_output_int.y = RL_output_int.y - RL_carry;
                    if (RL_output_int.y < 0.0f) { RL_output_int.y = RL_output_int.y + 1.0f; RL_carry = RL_twom24; } else { RL_carry = 0.0f; }
                RL_output_int.z = RL_output_int.z - RL_carry;
                    if (RL_output_int.z < 0.0f) { RL_output_int.z = RL_output_int.z + 1.0f; RL_carry = RL_twom24; } else { RL_carry = 0.0f; }

                RL_seed_temp.xyz  = RL_output_int.zyx;
                RL_seed_temp.w    = (*RL_seed5).x;
                RL_seed_temp2.xyz = (*RL_seed5).yzw;
                RL_seed_temp2.w   = (*RL_seed0).x;

                (*RL_seed5).w   = (*RL_seed4).x;
                (*RL_seed5).xyz = (*RL_seed3).yzw;
                (*RL_seed4).w   = (*RL_seed3).x;
                (*RL_seed4).xyz = (*RL_seed2).yzw;
                (*RL_seed3).w   = (*RL_seed2).x;
                (*RL_seed3).xyz = (*RL_seed1).yzw;
                (*RL_seed2).w   = (*RL_seed1).x;
                (*RL_seed2).xyz = (*RL_seed0).yzw;
                (*RL_seed1)  = RL_seed_temp2;
                (*RL_seed0)  = RL_seed_temp;
#endif
#ifdef RL_RANLUX4
                RL_seeds_j24.xy = (*RL_seed2).yx;
                RL_seeds_j24.zw = (*RL_seed1).wz;
                    rlproduceupd4(RL_seed5,RL_seeds_j24,&RL_carry);
                float RL_output_int = (*RL_seed1).y - (*RL_seed4).w;
                RL_output_int = RL_output_int - RL_carry;
                    if (RL_output_int < 0.0f) { RL_output_int = RL_output_int + 1.0f; RL_carry = RL_twom24; } else { RL_carry = 0.0f; }

                RL_seed_temp  = (*RL_seed5);
                RL_seed_temp2.x = RL_output_int;

                (*RL_seed5).yzw = (*RL_seed4).xyz;
                (*RL_seed5).x   = (*RL_seed3).w;
                (*RL_seed4).yzw = (*RL_seed3).xyz;
                (*RL_seed4).x   = (*RL_seed2).w;
                (*RL_seed3).yzw = (*RL_seed2).xyz;
                (*RL_seed3).x   = (*RL_seed1).w;
                (*RL_seed2).yzw = (*RL_seed1).xyz;
                (*RL_seed2).x   = (*RL_seed0).w;
                (*RL_seed1).yzw = (*RL_seed0).xyz;
                (*RL_seed1).x   = RL_seed_temp.w;
                (*RL_seed0).yzw = RL_seed_temp.xyz;
                (*RL_seed0).x   = RL_seed_temp2.x;
#endif
            (*RL_carin).z = 0.0f;
		}
        if ((*RL_carin).z < 1.0f) {
			RL_seeds_j24.xy = (*RL_seed2).yx;
			RL_seeds_j24.zw = (*RL_seed1).wz;
				uni = rlproducefloat4(RL_seed5,RL_seeds_j24,(*RL_seed1).y,&RL_carry);
			(*RL_carin).z = 1.0f;
		} else if ((*RL_carin).z == 1.0f) {
			RL_seeds_j24.xy = (*RL_seed1).yx;
			RL_seeds_j24.zw = (*RL_seed0).wz;
				uni = rlproducefloat4(RL_seed4,RL_seeds_j24,(*RL_seed0).y,&RL_carry);
			(*RL_carin).z = 2.0f;
		} else if ((*RL_carin).z == 2.0f) {
			RL_seeds_j24.xy = (*RL_seed0).yx;
			RL_seeds_j24.zw = (*RL_seed5).wz;
				uni = rlproducefloat4(RL_seed3,RL_seeds_j24,(*RL_seed5).y,&RL_carry);
			(*RL_carin).z = 3.0f;
		} else if ((*RL_carin).z == 3.0f) {
			RL_seeds_j24.xy = (*RL_seed5).yx;
			RL_seeds_j24.zw = (*RL_seed4).wz;
				uni = rlproducefloat4(RL_seed2,RL_seeds_j24,(*RL_seed4).y,&RL_carry);
			(*RL_carin).z = 4.0f;
		} else if ((*RL_carin).z == 4.0f) {
			RL_seeds_j24.xy = (*RL_seed4).yx;
			RL_seeds_j24.zw = (*RL_seed3).wz;
				uni = rlproducefloat4(RL_seed1,RL_seeds_j24,(*RL_seed3).y,&RL_carry);
			(*RL_carin).z = 5.0f;
		} else if ((*RL_carin).z == 5.0f) {
			RL_seeds_j24.xy = (*RL_seed3).yx;
			RL_seeds_j24.zw = (*RL_seed2).wz;
				uni = rlproducefloat4(RL_seed0,RL_seeds_j24,(*RL_seed2).y,&RL_carry);
			(*RL_carin).z = 6.0f;
		}
	(*RL_carin).w = RL_carry;
    return uni;
}

#ifdef PRECISION_DOUBLE  // if double precision is defined
__attribute__((always_inline)) hgpu_double4
rl_step_double(float4* RL_seed0,float4* RL_seed1,float4* RL_seed2,float4* RL_seed3,
               float4* RL_seed4,float4* RL_seed5,float4* RL_carin)
{
    hgpu_double4 result;
    hgpu_double2 rnd_double;
    float4 rnd1,rnd2;
    bool flag = true;
    while (flag) {
        rnd1 = rl_step(RL_seed0,RL_seed1,RL_seed2,RL_seed3,RL_seed4,RL_seed5,RL_carin);
        rnd_double = hgpu_float4_to_double2(rnd1,RL_min,RL_max,RL_k);
        if ((rnd_double.x>=RL_left) && (rnd_double.x<RL_right) &&
            (rnd_double.y>=RL_left) && (rnd_double.y<RL_right)) flag = false;
    }
    result.x = rnd_double.x;
    result.y = rnd_double.y;

    flag = true;
    while (flag) {
        rnd2 = rl_step(RL_seed0,RL_seed1,RL_seed2,RL_seed3,RL_seed4,RL_seed5,RL_carin);
        rnd_double = hgpu_float4_to_double2(rnd2,RL_min,RL_max,RL_k);
        if ((rnd_double.x>=RL_left) && (rnd_double.x<RL_right) &&
            (rnd_double.y>=RL_left) && (rnd_double.y<RL_right)) flag = false;
    }
    result.z = rnd_double.x;
    result.w = rnd_double.y;

    return result;
}
#endif


__kernel void
ranlux(__global float4 * seedtable,__global hgpu_float4 * prns, const uint samples)
{
    uint giddst = GID;
		float4 RL_seed0 = seedtable[GID + 0 * GID_SIZE];
		float4 RL_seed1 = seedtable[GID + 1 * GID_SIZE];
		float4 RL_seed2 = seedtable[GID + 2 * GID_SIZE];
		float4 RL_seed3 = seedtable[GID + 3 * GID_SIZE];
		float4 RL_seed4 = seedtable[GID + 4 * GID_SIZE];
		float4 RL_seed5 = seedtable[GID + 5 * GID_SIZE];
		float4 RL_carin = seedtable[GID + 6 * GID_SIZE];    // RL_i24, RL_j24, RL_in24, RL_carry

	for (int i=0; i<samples; i++) {
#ifdef PRECISION_DOUBLE // if double precision is defined
        hgpu_double4 result = rl_step_double(&RL_seed0,&RL_seed1,&RL_seed2,&RL_seed3,&RL_seed4,&RL_seed5,&RL_carin);
        prns[giddst] = result;
#else
        float4 uni = rl_step(&RL_seed0,&RL_seed1,&RL_seed2,&RL_seed3,&RL_seed4,&RL_seed5,&RL_carin);
		prns[giddst] = uni;
#endif
		giddst += GID_SIZE;
	}

	seedtable[GID + 0 * GID_SIZE] = RL_seed0;
	seedtable[GID + 1 * GID_SIZE] = RL_seed1;
	seedtable[GID + 2 * GID_SIZE] = RL_seed2;
	seedtable[GID + 3 * GID_SIZE] = RL_seed3;
	seedtable[GID + 4 * GID_SIZE] = RL_seed4;
	seedtable[GID + 5 * GID_SIZE] = RL_seed5;
	seedtable[GID + 6 * GID_SIZE] = RL_carin;
}



#endif
                                                                                                                              
                                                                                                                              
                                                                                                                               
                                                                                                                              
                                                                                                                              
                                                                                                                              
                                                                                                                              
 
