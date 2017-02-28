/******************************************************************************
* @file     hgpu_structs.h
* @author   Vadim Demchik <vadimdi@yahoo.com>
* @version  2.0.0
*
* @brief    [HGPU library]
*           structures in HGPUCL packages
*
*
* @section  LICENSE
*
* Copyright (c) 2013-2017 Vadim Demchik
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

#ifndef HGPU_STRUCTS_H
#define HGPU_STRUCTS_H

// #include "hgpu.h"

    // structure for arrays of init parameters
    typedef struct{
                             char*  parameter;
                              int   value_integer;
                           double   value_double;
                             char*  value_text;
    } HGPU_parameter;


    typedef enum enum_HGPU_precision {
            HGPU_precision_none   = 0,          // none
            HGPU_precision_single = 1,          // float  (32 bit)
            HGPU_precision_double = 2,          // double (64 bit)
            HGPU_precision_mixed  = 3           // mixed precision (32 bit + 32 bit)
    } HGPU_precision;



#endif // HGPU_STRUCTS_H
