/******************************************************************************
* @file     hgpu_types.h
* @version  2.0.0
*
* @brief    HGPU library
*
* This file contains description of used data types for HGPU library
*
* @section  LICENSE
*
* @copyright Copyright (c) 2013-2017 HGPU group. All rights reserved.
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

#ifndef _HGPU_TYPES_H_
#define _HGPU_TYPES_H_

/// ____________________________________________________________ define bitness-dependent constants
#if ( defined( __64BIT__     ) \
   || defined( _AMD64_       ) \
   || defined( _amd64        ) \
   || defined( _amd64__      ) \
   || defined( __amd64       ) \
   || defined( __amd64__     ) \
   || defined( _ARCH_PPC64   ) \
   || defined( _IA64_        ) \
   || defined( __ia64        ) \
   || defined( __IA64__      ) \
   || defined( _ia64_        ) \
   || defined( __ia64__      ) \
   || defined( __itanium__   ) \
   || defined( _M_IA64       ) \
   || defined( _LP64         ) \
   || defined( __LP64__      ) \
   || defined( _M_X64        ) \
   || defined( _M_AMD64      ) \
   || defined( __powerpc64__ ) \
   || defined( __ppc64       ) \
   || defined( __ppc64__     ) \
   || defined( __PPC64__     ) \
   || defined( __sparc64__   ) \
   || defined( _WIN64        ) \
   || defined( WIN64         ) \
   || defined( __x86_64      ) \
   || defined( __x86_64__    ) \
    )
  #define HGPU_OS_64BIT     ///< 64-bit platform is used
#else  // bitness
  #define HGPU_OS_32BIT     ///< 32-bit platform is used
#endif // bitness

/// ___________________________________________________________ define platform-dependent constants
#if (defined( _WIN32  ) \
  || defined( WIN32   ) \
  || defined( _WIN64  ) \
  || defined( WIN64   ) \
  || defined( _M_IX86 ) \
  || defined( _X86_   ) )
  #define HGPU_OS_WINDOWS           ///< Windows platform
#elif (defined( __linux__ ) \
    || defined( __linux   ) \
    || defined( __GNUC__  ) \
    || defined( linux     ) )
  #define HGPU_OS_LINUX             ///< Linux platform
#elif defined( __APPLE__ )
  #include "TargetConditionals.h"
  #define HGPU_OS_APPLE             ///< Apple platform
  #if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
    #define HGPU_OS_APPLE_SIMULATOR ///< Apple simulator
  #elif TARGET_OS_IPHONE
    #define HGPU_OS_APPLE_DEVICE    ///< Apple device
  #elif TARGET_OS_MAC
    #define HGPU_OS_APPLE_MAC       ///< other Apple Mac device
  #else
    #error "Unknown Apple platform" ///< Error: unknown Apple platform
  #endif
#elif ( defined( __unix__ ) \
     || defined( __unix   ) )
  #define HGPU_OS_UNIX              ///< other Unix platform
#elif ( defined( _POSIX_VERSION ) \
     || defined( __posix        ) )
  #define HGPU_OS_POSIX             ///< POSIX platform
#else
  #error "Unknown compiler"         ///< Error: uknown platform
#endif // platform


/// ______________________________________________________________________ define common data types
#undef byte_T
#undef char_T
#undef uchar_T
#undef u64_T

typedef  size_t                  size_T;     ///< size type
typedef  uint8_t                 byte_T;     ///< byte type
typedef  signed char             char_T;     ///< signed char type
typedef  unsigned char           uchar_T;    ///< unsigned char type
typedef  unsigned int            uint_T;     ///< unsigned int type
typedef  uint8_t                 u8_T;       ///< unsigned 8-bit int type
typedef  uint16_t                u16_T;      ///< unsigned 16-bit int type
typedef  uint32_t                u32_T;      ///< unsigned 32-bit int type
typedef  uint64_t                u64_T;      ///< unsigned 64-bit int type
typedef  long long unsigned int  llu_T;      ///< long long unsigned int type
typedef  int                     int_T;      ///< signed int type
typedef  int8_t                  i8_T;       ///< signed 8-bit int type
typedef  int16_t                 i16_T;      ///< signed 16-bit int type
typedef  int32_t                 i32_T;      ///< signed 32-bit int type
typedef  int64_t                 i64_T;      ///< signed 64-bit int type
typedef  long long int           lli_T;      ///< long long signed int type

typedef  float          single_T;   ///< single floating point type
typedef  double         double_T;   ///< double floating point type

#ifdef HGPU_PRECISION_SINGLE
typedef  float          float_T;    ///< user-define floating point type
#else  // HGPU_PRECISION_SINGLE
typedef  double         float_T;    ///< user-defined floating point type
#endif // HGPU_PRECISION_SINGLE


/// constants with all set bits for corresponding data formats
#define MAX_size_T          ( (size_T) ~0 )
#define MAX_i32_T           ( (i32_T) ~0 )
#define MAX_u32_T           ( (u32_T) ~0 )
#define MAX_i64_T           ( (i64_T) ~0 )
#define MAX_u64_T           ( (u64_T) ~0 )


union _DoubleAsUint64 {
    double_T val_double;
    u64_T    val_u64;
};

union _DoubleAsUint32 {
    double_T val_double;
    u64_T    val_u32[2];
};

union _DoubleAsUint16 {
    double_T val_double;
    u64_T    val_u16[4];
};

union _DoubleAsUint8 {
    double_T val_double;
    u8_T     val_u8[8];
};

union _SingleAsUint32 {
    single_T val_float;
    u32_T    val_u32;
};

union _SingleAsUint16 {
    single_T val_float;
    u32_T    val_u16[2];
};

union _SingleAsUint8 {
    single_T val_float;
    u32_T    val_u8[4];
};

#endif // _HGPU_TYPES_H_
