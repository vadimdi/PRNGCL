/******************************************************************************
 * @file     hgpu_convert.h
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           Functions for convertation
 *
 *
 * @section  LICENSE
 *
 * Copyright (c) 2013-2015 Vadim Demchik
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

#ifndef HGPU_CONVERT_H
#define HGPU_CONVERT_H

#include "hgpu.h"

    typedef union{                                  // Uint <---> Float converter
                     unsigned int   uint_value[1];
                            float   float_value;
    } HGPU_convert_uint_and_float;

    typedef union{                                  // Uint <---> Double converter
                     unsigned int   uint_value[2];
                           double   double_value;
    } HGPU_convert_int_and_double;


                  float   HGPU_convert_uint_to_float(unsigned int value);
                 double   HGPU_convert_uint_to_double(unsigned int value);
                 double   HGPU_convert_uint2_to_double(unsigned int value_low,unsigned int value_high);
           unsigned int   HGPU_convert_float_to_uint(float value);
                 double   HGPU_convert_float_to_double(float value);
           unsigned int   HGPU_convert_double_to_uint_high(double x);
           unsigned int   HGPU_convert_double_to_uint_low(double x);
                  float   HGPU_convert_double_to_float(double value);

                 double   HGPU_convert_B_to_KB(double value);
                 double   HGPU_convert_B_to_MB(double value);
                 double   HGPU_convert_B_to_GB(double value);

                 double   HGPU_convert_B_to_MBS(double bytes,double time_nanoseconds);
                 double   HGPU_convert_B_to_GBS(double bytes,double time_nanoseconds);
                 double   HGPU_convert_s_to_ns(double time_seconds);

           unsigned int   HGPU_convert_round_to_power_2(unsigned int value);

#endif
