/******************************************************************************
 * @file     hgpu_io.h
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           i/o submodule for HGPU package
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

#ifndef HGPUCL_IO_H
#define HGPUCL_IO_H

#include "hgpu.h"
//#include "../hgpucl/platform.h"
//#include "hgpu_error.h"
//#include "hgpu_string.h"
//#include "../hgpucl/hgpucl_constants.h"


            extern char*  HGPU_io_path_working;
            extern char*  HGPU_io_path_inf;
            extern char*  HGPU_io_path_current;
            extern char*  HGPU_io_path_root;

                   void   HGPU_io_path_get(char** path);
                   char*  HGPU_io_path_get_root(void);
                   void   HGPU_io_path_set_separator(char** path,size_t path_length);
                   void   HGPU_io_path_join_filename(char** file_path_and_name,const size_t file_path_and_name_length,const char* file_path,const char* file_name);

                   char*  HGPU_io_file_read(const char* file_name);
                   void   HGPU_io_file_read_add(char** source, const char* file_name);
                   char*  HGPU_io_file_read_with_path(const char* file_path,const char* file_name);
//                   void   HGPU_io_file_read_add_path(char** source,const char* file_path,const char* file_name);
                   void   HGPU_io_file_write(const char* file_name,const char* buffer);
                   void   HGPU_io_file_write_with_path(const char* file_path,const char* file_name,const char* buffer);
                   void   HGPU_io_file_write_binary(const char* file_name,const unsigned char* buffer,const size_t bytes_to_write);
                   bool   HGPU_io_file_check_existence(const char* file_name);
                   bool   HGPU_io_file_check_existence_with_path(const char* file_path,const char* file_name);

                    int   HGPU_io_inf_file_delete(int inf_index);
                   void   HGPU_io_inf_file_delete_all(void);
                    int   HGPU_io_inf_file_rename(int inf_index_old,int inf_index_new);
                    int   HGPU_io_inf_file_get_max_index(void);
                   bool   HGPU_io_inf_file_check_existence(int inf_index);

                   bool   HGPU_io_bin_file_check_existence(int inf_index);
          unsigned char*  HGPU_io_bin_file_read(int inf_index,size_t* binary_size);
#endif
