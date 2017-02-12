/******************************************************************************
 * @file     hgpucl.h
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           Interface for OpenCL AMD APP & nVidia SDK environment
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

#ifndef HGPUCL_H
#define HGPUCL_H

#ifndef HGPU_min
#define HGPU_min(a,b) ((a) < (b)) ? (a) : (b)
#endif
#ifndef HGPU_max
#define HGPU_max(a,b) ((a) > (b)) ? (a) : (b)
#endif


#include <CL/cl.h>
#include "platform.h"
#include "../include/hgpu_types.h"
#include "hgpucl_platforms.h"
#include "hgpucl_devices.h"
#include "hgpucl_context.h"
#include "hgpucl_program.h"
#include "../include/hgpu_io.h"
#include "../include/hgpu_md5.h"
#include "../include/hgpu_timer.h"
#include "../include/hgpu_parameters.h"
#include "../randomcl/hgpu_prng.h"

                   void   HGPU_GPU_test(int argc, char** argv);


#endif
