/******************************************************************************
* @file     hgpu.h
* @version  2.0.0
*
* @brief    HGPU library
*
* This file contains including of all header files for HGPU library
*
* @section  LICENSE
*
* @copyright Copyright (c) 2013-2016 HGPU group. All rights reserved.
*
* @author   Vadim Demchik <vadimdi@yahoo.com>
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

#pragma once

#if defined(_WIN32) || defined(_WIN64)
#pragma warning(push,0) // store the current state of compiler warnings
// #pragma warning(disable:4710)    // remove warning: function not inlined
// #pragma warning(disable:4514)    // remove warning: unreferenced inline function has been removed
// #pragma warning(disable:4820)    // remove warning: 'x' bytes padding added after data member 'y'
// #pragma warning(disable:C4996)   // remove warning: 'strerror': This function or variable may be unsafe.
// #pragma warning(disable:C4244)   // remove warning: '=': conversion from 'uint32_t'
// #pragma warning(disable:C4267)   // remove warning: '=': conversion from 'size_t'
#else  // Windows platform
#if ((__GNUC__>4) || ((__GNUC__==4) && (__GNUC_MINOR__>=6)))
#pragma GCC diagnostic push // store the current state of compiler warnings
#endif // GNU C vesion >= 4.6
#pragma GCC system_header
#endif // Windows platform


#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>
#include <algorithm>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include <direct.h>
#include <string>
#else  // _WIN32
//#include <pthread.h>
#include <string.h>
#include <unistd.h>
#endif // _WIN32

#include <math.h>
#include <assert.h>
#include <iostream>



#if defined(_WIN32) && defined(BUILD_DLL)
#if defined(HGPU_EXPORTS)
/// --- inside DLL
#define HGPU_EXPORT_API        __declspec(dllexport)
#define HGPU_EXPIMP_TEMPLATE
#else  // HGPU_EXPORTS
/// --- outside DLL
#define HGPU_EXPORT_API        __declspec(dllimport)
#define HGPU_EXPIMP_TEMPLATE   extern
#endif // HGPU_EXPORTS
#else  // _WIN32 && BUILD_DLL
#define HGPU_EXPORT_API
#endif // _WIN32 && BUILD_DLL


// include external libraries
// #include "lmdb.h" // header for LMDB static library
// #include "lmdb++.h" // header for LMDB++ connector


#if defined(_WIN32) || defined(_WIN64)
#pragma warning(pop) // restore the state of compiler warnings
#else  // Windows platform
#if ((__GNUC__>4) || ((__GNUC__==4) && (__GNUC_MINOR__>=6)))
#pragma GCC diagnostic pop // restore the state of compiler warnings
#endif // GNU C vesion >= 4.6
#endif // Windows platform


// include HGPU library header files
#include "hgpu_constants.h"
// #include "hgpu_default.h"
#include "hgpu_types.h"
#include "hgpu_platform.h"
#include "hgpu_structs.h"
#include "hgpu_enum.h"
#include "hgpu_convert.h"
#include "hgpu_string.h"
#include "hgpu_parameters.h"
#include "hgpu_error.h"
#include "hgpu_timer.h"
// #include "hgpu_critical.h"
// #include "hgpu_vec.h"
// #include "hgpu_memory.h"
// #include "hgpu_util.h"
// #include "hgpu_buf.h"
// #include "hgpu_stack.h"
// #include "hgpu_binstack.h"
// #include "hgpu_hash.h"
#include "hgpu_io.h"
// #include "hgpu_threads.h"
#include "hgpu_md5.h"
