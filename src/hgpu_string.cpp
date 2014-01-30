/******************************************************************************
 * @file     hgpu_string.cpp
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

#include "../include/hgpu_string.h"

int
HGPU_string_char_replace(char* str, char search, char replace){
      int result = 0;
        for (size_t i = 0; i<strlen(str); i++)
            if (str[i]==search){
                str[i] = replace;
                result++;
            }
      return result;
}

void
HGPU_string_to_lowercase(char* str){
    for(size_t i = 0; str[i]; i++) str[i] = (char) tolower(str[i]);
}

void
HGPU_string_to_uppercase(char* str){
    for(size_t i = 0; str[i]; i++) str[i] = (char) toupper(str[i]);
}

char*
HGPU_string_clone(const char* str){
    if (!str) return NULL;
    size_t len = strlen(str) + 1;
    char* result = (char*) calloc(len,sizeof(char));
    if (!result) {   // error - can not allocate memory
        HGPU_error_note(HGPU_ERROR_NO_MEMORY,NULL);
        return NULL;
    }
    size_t bytes = len*sizeof(char);
    memcpy_s(result,bytes,str,bytes);
    return result;
}

void
HGPU_string_trim(char*& str){
    if (!str) return;
    size_t len_left  = 0;
    size_t len_right = strlen(str)-1;
    while((len_right>0) && (isspace(str[len_right]))) str[len_right--] = 0;
    while((len_left<len_right) && (isspace(str[len_left]))) len_left++;
    if (len_left==len_right) return;
    size_t len_new = (len_right-len_left+2);
    memcpy_s(str,len_new,str+len_left,len_new);
}

void
HGPU_string_delete_symbols_except_numbers_and_dots(char*& str){
    if (!str) return;
    size_t str_length = strlen(str);
    if (!str_length) return;
    for(size_t i=0; i<str_length; i++){
        char symbol = str[i];
        if ((!isdigit(symbol)) && (symbol!=HGPU_CHAR_DOT)) str[i] = HGPU_CHAR_SPACE;
    }
}

void
HGPU_string_resize(char** str,size_t size_new){
    if (!str) return;
    char* str_new = (char*) realloc(*str,size_new);
    if (!str_new) {
        HGPU_error_note(HGPU_ERROR_NO_MEMORY,NULL);
        return;
    }
    if (!(*str)) str_new[0] = 0;
    (*str) = str_new;    
}

void
HGPU_string_join(char** str_dest,char* str_src){
    if (!str_src) return;
    size_t len_src  = strlen(str_src);
    size_t len_dest = len_src + 1;
    if ((str_dest) && (*str_dest)) len_dest += strlen(*str_dest);
    char* result = (char*) calloc(len_dest,sizeof(char));
    if (!result) {   // error - can not allocate memory
        HGPU_error_note(HGPU_ERROR_NO_MEMORY,NULL);
        return;
    }
#ifdef _WIN32
    size_t bytes_dest = len_dest*sizeof(char);
#endif
    if ((str_dest) && (*str_dest)) {
        memcpy_s(result,bytes_dest,*str_dest,strlen(*str_dest)*sizeof(char));
        memcpy_s((result+strlen(*str_dest)*sizeof(char)),bytes_dest,str_src,len_src*sizeof(char));
    } else
        memcpy_s(result,bytes_dest,str_src,strlen(str_src)*sizeof(char));
    free(*str_dest);
    (*str_dest) = result;
}

char*
HGPU_string_get_from_integer(int value){
    char buffer[HGPU_GPU_MAX_STR_INFO_LENGHT];
    int len = sprintf_s(buffer,HGPU_GPU_MAX_STR_INFO_LENGHT,"%d",value) + 1;
    char* result = (char*) calloc(len,sizeof(char));
    if (!result) {   // error - can not allocate memory
        HGPU_error_note(HGPU_ERROR_NO_MEMORY,NULL);
        return NULL;
    }
    size_t bytes = len*sizeof(char);
    memcpy_s(result,bytes,buffer,bytes);
    return result;
}

char*
HGPU_string_get_from_double(double value){
    char buffer[HGPU_GPU_MAX_STR_INFO_LENGHT];
    int len = sprintf_s(buffer,HGPU_GPU_MAX_STR_INFO_LENGHT,"%.16e",value) + 1;
    char* result = (char*) calloc(len,sizeof(char));
    if (!result) {   // error - can not allocate memory
        HGPU_error_note(HGPU_ERROR_NO_MEMORY,NULL);
        return NULL;
    }
    size_t bytes = len*sizeof(char);
    memcpy_s(result,bytes,buffer,bytes);
    return result;
}

size_t*
HGPU_string_size_t_clone(const size_t* source,const size_t size){
    if ((!source) || (!size)) return NULL;
    size_t* result = (size_t*) calloc(size,sizeof(size_t));
    if (!result) {   // error - can not allocate memory
        HGPU_error_note(HGPU_ERROR_NO_MEMORY,NULL);
        return NULL;
    }
    size_t bytes = size*sizeof(size_t);
    memcpy_s(result,bytes,source,bytes);
    return result;
}

bool
HGPU_string_cmp(const char* str1, const char* str2){
    bool result = true;
    if ((!str1) || (!str2) || (strlen(str1)!=strlen(str2))) return false;
    
    for (size_t i=0; i<strlen(str1); i++)
        if (toupper(str1[i]) != toupper(str2[i])) {
            result = false;
            break;
        }
    return result;
}
