/******************************************************************************
 * @file     hgpu_timer.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           timer submodule
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

#include "../include/hgpu_timer.h"

// start timer
HGPU_timer
HGPU_timer_start(void){
#ifdef SAFE_LINUX_TIMER
    HGPU_timer timer;
    time(&timer);
    return (HGPU_timer) timer;
#else
    return clock();
#endif
}

// get timer value in seconds (time delta)
double
HGPU_timer_get(HGPU_timer timer){
#ifdef SAFE_LINUX_TIMER
    double result = difftime(HGPU_timer_start(),timer);
#else
    double result = ((double) (clock() - timer)) / CLOCKS_PER_SEC;
#endif
    return result;
}

// get timer value in seconds (from start)
double
HGPU_timer_get_from_start(void){
    return ((double) clock()) / CLOCKS_PER_SEC;
}

// get date-time string
char*
HGPU_timer_get_current_datetime(void){
    time_t tim;
	time(&tim);
    char* result = (char*) calloc(30,sizeof(char));
#ifdef _WIN32
    ctime_s(result, 30, &tim);
#else
    sprintf(result,"%s",ctime((const time_t*) &tim));
#endif
    HGPU_string_char_replace(result,HGPU_CHAR_CR,0);
    HGPU_string_char_replace(result,HGPU_CHAR_NEWLINE,0);
    return result;
}


// get (HGPU_timer_deviation) from profiling data
HGPU_timer_deviation
HGPU_timer_deviation_get(double elapsed_time, double elapsed_time_squared, double number_of_elements){
    HGPU_timer_deviation execution_time;
    execution_time.mean = elapsed_time;
    execution_time.number_of_elements = number_of_elements;
    execution_time.deviation = number_of_elements < 2 ? 0.0 : sqrt(abs(elapsed_time_squared - pow(elapsed_time,2)) / number_of_elements);
    return execution_time;
}
