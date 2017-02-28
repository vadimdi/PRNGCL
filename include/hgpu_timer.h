/******************************************************************************
 * @file     hgpu_timer.h
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

#ifndef HGPUCL_TIMER_H
#define HGPUCL_TIMER_H

#include "hgpu.h"

#ifdef SAFE_LINUX_TIMER
#define HGPU_timer time_t
#else
#define HGPU_timer clock_t
#endif

    // structure for buffer
    typedef struct{
                           double   mean;                // mean value
                           double   deviation;           // deviation
                           double   number_of_elements;  // number of elements
    } HGPU_timer_deviation;

    const HGPU_timer_deviation HGPU_timer_deviation_default = {0.0, 0.0, 0};

             HGPU_timer   HGPU_timer_start(void);
                 double   HGPU_timer_get(HGPU_timer timer);
                 double   HGPU_timer_get_from_start(void);
                   char*  HGPU_timer_get_current_datetime(void);

   HGPU_timer_deviation   HGPU_timer_deviation_get(double elapsed_time, double elapsed_time_squared, double number_of_elements);

#endif
