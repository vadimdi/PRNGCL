/******************************************************************************
 * @file     hgpu_convert.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           Functions for convertation
 *
 *
 * @section  LICENSE
 *
 * Copyright (c) 2013, 2014 Vadim Demchik
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

#include "../include/hgpu_convert.h"


// convert (unsigned int) to (float)
float
HGPU_convert_uint_to_float(unsigned int value){
	HGPU_convert_uint_and_float result;
	result.uint_value[0]=value;
	return result.float_value;
}

// convert (unsigned int) to (double)
double
HGPU_convert_uint_to_double(unsigned int value)
{
	HGPU_convert_uint_and_float result;
	result.uint_value[0]=value;
	return (double) result.float_value;
}

// convert (unsigned int high and low) to (double)
double
HGPU_convert_uint2_to_double(unsigned int value_low,unsigned int value_high){
    HGPU_convert_int_and_double result;
        result.uint_value[0] = value_low;
        result.uint_value[1] = value_high;
    return result.double_value;
}


// convert (float) to (uint)
unsigned int
HGPU_convert_float_to_uint(float value){
	HGPU_convert_uint_and_float result;
	result.float_value = value;
	return result.uint_value[0];
}

// convert (float) to (double)
double
HGPU_convert_float_to_double(float value){
	return (double) value;
}


// convert (double) to (uint_high)
unsigned int
HGPU_convert_double_to_uint_high(double x){
    HGPU_convert_int_and_double result;
        result.double_value = x;
    return result.uint_value[1];
}

// convert (double) to (uint_low)
unsigned int
HGPU_convert_double_to_uint_low(double x){
    HGPU_convert_int_and_double result;
        result.double_value = x;
    return result.uint_value[0];
}

// convert (double) to (float)
float
HGPU_convert_double_to_float(double value){
	return (float) value;
}

// convert (bytes) to (kilobytes)
double
HGPU_convert_B_to_KB(double bytes){
    return bytes/1024.;
}

// convert (bytes) to (megabytes)
double
HGPU_convert_B_to_MB(double bytes){
    return bytes/1024./1024.;
}

// convert (bytes) to (gigabytes)
double
HGPU_convert_B_to_GB(double bytes){
    return bytes/1024./1024./1024.;
}

// convert (bytes) and (time) into (megabytes per second)
double
HGPU_convert_B_to_MBS(double bytes,double time_nanoseconds){
    double result = 0.0;
        if (time_nanoseconds) result = (bytes/time_nanoseconds/(1.024*1.024*1.024)*1024.0);
    return result;
}

// convert (bytes) and (time) into (gigabytes per second)
double
HGPU_convert_B_to_GBS(double bytes,double time_nanoseconds){
    double result = 0.0;
        if (time_nanoseconds) result = (bytes/time_nanoseconds/(1.024*1.024*1.024));
    return result;
}

// convert (seconds) into (nanoseconds)
double
HGPU_convert_s_to_ns(double time_seconds){
    return (time_seconds * 1.E+9);
}


// round value to nearest smallest power of 2
unsigned int
HGPU_convert_round_to_power_2(unsigned int value){
    unsigned int result = (unsigned int) (1<<((unsigned int) floor(log((double) value)/log(2.0))));
    return result;
}
