/******************************************************************************
 * @file     hgpu_parameters.h
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           Parameters submodule
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

#ifndef HGPU_PARAMETERS_H
#define HGPU_PARAMETERS_H

#include "../hgpucl/platform.h"
#include "../hgpucl/hgpucl_constants.h"
#include "hgpu_error.h"
#include "hgpu_string.h"
#include "hgpu_io.h"
#include "hgpu_enum.h"

    // structure for arrays of init parameters
    typedef struct{
                             char*  parameter;
                              int   value_integer;
                           double   value_double;
                             char*  value_text;
    } HGPU_parameter;

         HGPU_parameter** HGPU_parameters_new(unsigned int number_of_parameters);
         HGPU_parameter** HGPU_parameters_clone(HGPU_parameter** parameters);
                   void   HGPU_parameters_delete(HGPU_parameter*** parameters);
                   void   HGPU_parameters_delete_by_name(HGPU_parameter** parameters,char* parameter_name);
                   void   HGPU_parameters_add(HGPU_parameter*** parameters,HGPU_parameter* parameter);
                   void   HGPU_parameters_join(HGPU_parameter*** parameters1,HGPU_parameter** parameters2);
                   void   HGPU_parameters_resize(HGPU_parameter*** parameters,unsigned int parameters_to_be_added);
                   void   HGPU_parameters_print(HGPU_parameter** parameters);
                   bool   HGPU_parameters_check(HGPU_parameter** parameters_where,HGPU_parameter** parameters_what);
                   bool   HGPU_parameters_check_name(HGPU_parameter** parameters,char* parameter);
                   void   HGPU_parameters_path_setup(HGPU_parameter** parameters);

           unsigned int   HGPU_parameters_get_index(HGPU_parameter** parameters,char* parameter_name);
           unsigned int   HGPU_parameters_get_last_index(HGPU_parameter** parameters);
         HGPU_parameter*  HGPU_parameters_get_by_index(HGPU_parameter** parameters,unsigned int parameter_index);
         HGPU_parameter*  HGPU_parameters_get_by_name(HGPU_parameter** parameters,char* parameter_name);
         HGPU_parameter** HGPU_parameters_get_from_file(const char* file_name);
         HGPU_parameter** HGPU_parameters_get_from_file_with_path(const char* file_path,const char* file_name);
         HGPU_parameter** HGPU_parameters_get_from_commandline(int argc, char** argv);
         HGPU_parameter** HGPU_parameters_get_from_environment(void);
         HGPU_parameter** HGPU_parameters_get_from_inf_file(int index);
         HGPU_parameter** HGPU_parameters_get_all(int argc, char** argv);

         HGPU_precision   HGPU_parameters_get_precision(HGPU_parameter** parameters,const char* parameter_name);

                   char*  HGPU_parameters_put(HGPU_parameter** parameters);
                   void   HGPU_parameters_write_to_inf_file(HGPU_parameter** parameters,const unsigned char* binary,const size_t bytes_in_binary,int inf_index);

         HGPU_parameter*  HGPU_parameter_new(void);
                   void   HGPU_parameter_delete(HGPU_parameter** parameter);
         HGPU_parameter*  HGPU_parameter_clone(HGPU_parameter* parameter);
                   void   HGPU_parameter_copy(HGPU_parameter** parameter_dst,HGPU_parameter* parameter_src);
                   void   HGPU_parameter_print(HGPU_parameter* parameter);
                   bool   HGPU_parameter_check_name(HGPU_parameter* parameter,const char* parameter_name);
                   bool   HGPU_parameter_check_value(HGPU_parameter* parameter1,HGPU_parameter* parameter2);

         HGPU_parameter*  HGPU_parameter_get(char* line);
         HGPU_parameter*  HGPU_parameter_get_with_name_text(char* parameter,const char* value_text);
         HGPU_parameter*  HGPU_parameter_get_with_name_integer(char* parameter,int value_integer);
         HGPU_parameter*  HGPU_parameter_get_with_name_double(char* parameter,double value_double);
         HGPU_parameter*  HGPU_parameter_get_from_file(FILE** stream,const char* file_name);
         HGPU_parameter*  HGPU_parameter_get_from_file_with_path(FILE** stream,const char* file_path,const char* file_name);
         HGPU_parameter*  HGPU_parameter_get_from_file_next(FILE** stream);
         HGPU_parameter*  HGPU_parameter_get_from_commandline(char* line);
         HGPU_parameter*  HGPU_parameter_get_from_environment(const char* parameter_name);

                   char*  HGPU_parameter_put(HGPU_parameter* parameter);

         HGPU_precision   HGPU_parameter_get_precision(HGPU_parameter* parameter,const char* parameter_name);

#endif
