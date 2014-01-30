/******************************************************************************
 * @file     hgpu_string.h
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           string submodule
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

#ifndef HGPU_STRING_H
#define HGPU_STRING_H

#include "../hgpucl/platform.h"
#include "../hgpucl/hgpucl_constants.h"
#include "hgpu_error.h"

                    int   HGPU_string_char_replace(char* str, char search, char replace);
                   void   HGPU_string_to_lowercase(char* str);
                   void   HGPU_string_to_uppercase(char* str);
                   void   HGPU_string_trim(char*& str);
                   char*  HGPU_string_clone(const char* str);
                   void   HGPU_string_resize(char** str,size_t size_new);
                   void   HGPU_string_delete_symbols_except_numbers_and_dots(char*& str);
                   void   HGPU_string_join(char** str_dest,char* str_src);
                   char*  HGPU_string_get_from_integer(int value);
                   char*  HGPU_string_get_from_double(double value);
                   bool   HGPU_string_cmp(const char* str1, const char* str2);

                 size_t*  HGPU_string_size_t_clone(const size_t* source,const size_t size);

#endif
