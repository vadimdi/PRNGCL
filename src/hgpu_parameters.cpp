/******************************************************************************
 * @file     hgpu_parameters.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0
 *
 * @brief    [HGPU library]
 *           Parameters submodule
 *
 *
 * @section  LICENSE
 *
 * Copyright (c) 2013, Vadim Demchik
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

#include "../include/hgpu_parameters.h"

#define HGPU_PARAMETER_INCLUDE(parameter_name) {                                    \
            parameter = HGPU_parameter_get_from_environment(parameter_name);        \
            HGPU_parameters_add(&result,parameter);                                 \
            HGPU_parameter_delete(&parameter);                                      \
        };




// make new array of (HGPU_parameter) parameters
HGPU_parameter**
HGPU_parameters_new(unsigned int number_of_parameters){
    if (!number_of_parameters) return NULL;
    HGPU_parameter** result = (HGPU_parameter**) calloc(number_of_parameters+1,sizeof(HGPU_parameter*));
    if (!result) HGPU_error(HGPU_ERROR_NO_MEMORY);
    for (unsigned int i=0;i<number_of_parameters;i++)
        result[i] = HGPU_parameter_new();
    result[number_of_parameters] = NULL;
    return result;
}

// clone array of (HGPU_parameter) parameters
HGPU_parameter**
HGPU_parameters_clone(HGPU_parameter** parameters){
    unsigned int number_of_parameters = HGPU_parameters_get_last_index(parameters);
    HGPU_parameter** result = HGPU_parameters_new(number_of_parameters);
    for (unsigned int i=0;i<number_of_parameters;i++)
        HGPU_parameter_copy(&result[i],parameters[i]);
    return result;
}

// get number of parameters in array of (HGPU_parameter) parameters
unsigned int
HGPU_parameters_get_last_index(HGPU_parameter** parameters){
    unsigned int last_index = 0;
    while ((parameters) && (last_index<HGPU_MAX_PARAMETERS) && (parameters[last_index])) last_index++;
    return last_index;
}

// enlarge array of (HGPU_parameter) parameters on parameters_to_be_added elements
void
HGPU_parameters_resize(HGPU_parameter*** parameters,unsigned int parameters_to_be_added){
    if ((!(*parameters)) || (!parameters_to_be_added)) return;
    unsigned int last_index = HGPU_parameters_get_last_index(*parameters);
    if ((last_index+parameters_to_be_added)>HGPU_MAX_PARAMETERS) {
        HGPU_error_message(HGPU_ERROR_NO_MEMORY,"number of allowed parameters is exceed maximum (please, check HGPU_MAX_PARAMETERS)");
        return;
    }
    unsigned int new_size = (last_index+parameters_to_be_added+1);
    HGPU_parameter** result = (HGPU_parameter**) realloc((*parameters),new_size * sizeof(HGPU_parameter*));
    if (result) {
        (*parameters) = result;
        for (unsigned int i=last_index;i<(new_size-1);i++)
            (*parameters)[i] = HGPU_parameter_new();
        (*parameters)[new_size-1] = NULL;
    }
}

// delete array of (HGPU_parameter) parameters
void
HGPU_parameters_delete(HGPU_parameter*** parameters){
    if (!(*parameters)) return;
    unsigned int index = 0;
    while ((index<HGPU_MAX_PARAMETERS) && ((*parameters)[index]))
        HGPU_parameter_delete(&((*parameters)[index++]));
    free(*parameters);
    (*parameters) = NULL;
}

// delete one element of (HGPU_parameter) parameters by name
void
HGPU_parameters_delete_by_name(HGPU_parameter** parameters,char* parameter_name){
    if ((!parameters) || (!parameter_name) || (!strlen(parameter_name))) return;
    unsigned int last_index = HGPU_parameters_get_last_index(parameters);
    unsigned int index_old = HGPU_parameters_get_index(parameters,parameter_name);
    if (index_old<last_index){
        unsigned int index = last_index-1;
        while((index>0) && (!parameters[index]->parameter)) index--;
        if(index>index_old) {
            HGPU_parameter_delete(&parameters[index_old]);
            parameters[index_old] = parameters[index];
            parameters[index] = HGPU_parameter_new();
        } else {
            HGPU_parameter_delete(&parameters[index_old]);
            parameters[index_old] = HGPU_parameter_new();
        }
    }
}

// add parameter into array of (HGPU_parameter) parameters
void
HGPU_parameters_add(HGPU_parameter*** parameters,HGPU_parameter* parameter){
    if ((!parameter) || (!parameter->parameter)) return;
    if (!(*parameters)) (*parameters) = HGPU_parameters_new(HGPU_PARAMETERS_STEP);

    unsigned int new_index  = 0;
    unsigned int index      = HGPU_parameters_get_index(*parameters,parameter->parameter);
    unsigned int last_index = HGPU_parameters_get_last_index(*parameters);
    while((new_index<last_index) && ((*parameters)[new_index]->parameter)) new_index++;

    if ((index>=last_index) && (new_index>=last_index)) {
        HGPU_parameters_resize(parameters,HGPU_PARAMETERS_STEP);
        new_index = last_index;
    }
    if (index<last_index) new_index=index;
    HGPU_parameter_copy(&((*parameters)[new_index]),parameter);
}

// get parameter index in array of (HGPU_parameter) parameters by name
unsigned int
HGPU_parameters_get_index(HGPU_parameter** parameters,char* parameter_name){
    if ((!parameters) || (!parameter_name) || (!strlen(parameter_name))) return HGPU_MAX_PARAMETERS;
    unsigned int last_index = HGPU_parameters_get_last_index(parameters);
    unsigned int index      = 0;
    while ((index<last_index) && (!HGPU_parameter_check_name(parameters[index],parameter_name))) index++;
    return index;
}

// compare two sets of parameters, true if parameters_what is in parameters_where
bool
HGPU_parameters_check(HGPU_parameter** parameters_where,HGPU_parameter** parameters_what){
    bool result = true;
    if ((parameters_where) && (parameters_what)) {
        unsigned int last_index_where = HGPU_parameters_get_last_index(parameters_where);
        unsigned int last_index_what  = HGPU_parameters_get_last_index(parameters_what);
        unsigned int index = 0;
        unsigned int idx = 0;
        while((result) && (index < last_index_what)){
            result = false;
            if (!parameters_what[index]->parameter) result=true;
            else {
                idx = HGPU_parameters_get_index(parameters_where,parameters_what[index]->parameter);
                if (idx<last_index_where)
                    result |= HGPU_parameter_check_value(parameters_where[idx],parameters_what[index]);
            }
            index++;
        }
    }
    return result;
}

// check parameter existance by parameter name
bool
HGPU_parameters_check_name(HGPU_parameter** parameters,char* parameter){
    bool result = false;
    unsigned int index = HGPU_parameters_get_index(parameters,parameter);
    if (index < HGPU_MAX_PARAMETERS) result = true;
    return result;
}

// get parameter from array of (HGPU_parameter) parameters by index
HGPU_parameter*
HGPU_parameters_get_by_index(HGPU_parameter** parameters,unsigned int parameter_index){
    HGPU_parameter* result = NULL;    
    if (parameter_index<HGPU_MAX_PARAMETERS) result = parameters[parameter_index];
    return result;
}

// get parameter from array of (HGPU_parameter) parameters by name
HGPU_parameter*
HGPU_parameters_get_by_name(HGPU_parameter** parameters,char* parameter_name){
    HGPU_parameter* result = NULL;    
    unsigned int index = HGPU_parameters_get_index(parameters,parameter_name);
    if (index<HGPU_MAX_PARAMETERS) result = HGPU_parameters_get_by_index(parameters,index);
    return result;
}

// print array of (HGPU_parameter) parameters
void
HGPU_parameters_print(HGPU_parameter** parameters){
    unsigned int last_index = HGPU_parameters_get_last_index(parameters);
    unsigned int index = 0;
    while ((last_index>0) && (index<last_index)) HGPU_parameter_print(parameters[index++]);
}

// load array of (HGPU_parameter) parameters from file
HGPU_parameter**
HGPU_parameters_get_from_file(const char* file_name){
    FILE* fil;
    HGPU_parameter** result = NULL;
    HGPU_parameter*  pr = HGPU_parameter_get_from_file(&fil,file_name);
    while (pr) {
        HGPU_parameters_add(&result,pr);
        HGPU_parameter_delete(&pr);
        pr = HGPU_parameter_get_from_file_next(&fil);
    }
    return result;
}

// get parameters from command line arguments
HGPU_parameter**
HGPU_parameters_get_from_commandline(int argc, char** argv){
    HGPU_parameter** result = NULL;
    for(int i=1; i<argc; i++){
        if ((argv[i]) && (strlen(argv[i])) && ((argv[i][0]==HGPU_CHAR_MINUS) || (argv[i][0]==HGPU_CHAR_SLASH))) {
            HGPU_parameter* pr = HGPU_parameter_get_from_commandline(argv[i]);
            HGPU_parameters_add(&result,pr);
            HGPU_parameter_delete(&pr);
        }
    }
    return result;
}

// get parameters from environment variables
HGPU_parameter**
HGPU_parameters_get_from_environment(void){
    HGPU_parameter** result = NULL;
    HGPU_parameter* parameter = NULL;
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_DEVICE);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_PLATFORM);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_DEVICE_TYPE);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_WAIT_FOR_KEYPRESS);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_SHOW_STAGE);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_PROFILING);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_BRIEF_REPORT);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_REBUILD_BINARIES);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_PATH_CL_ROOT);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_PATH_INF);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_PATH_WORKING);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_WARNING_ERROR);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_NO_CACHE);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_MAX_WORKGROUP_SIZE);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_DEVICES_NUMBER);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_PRNG);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_PRNG_RANDSERIES);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_PRNG_PRECISION);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_PRNG_SAMPLES);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_PRNG_INSTANCES);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_PRNG_SEED1);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_PRNG_SEED2);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_PRNG_SEED3);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_PRNG_SEED4);
    HGPU_PARAMETER_INCLUDE(HGPU_PARAMETER_PRNG_RANLUX_NSKIP);
    return result;
}

// get all parameters - from command line and from ini-file (first parameter in command-line without "-" or "/")
// channel importance (1=most important)
// 1. command line
// 2. .ini-file
// 3. environment variables
HGPU_parameter**
HGPU_parameters_get_all(int argc, char** argv){
    HGPU_parameter** result = HGPU_parameters_get_from_environment();
    int i=1;
    while (i<argc){
        if ((argv[i]) && (strlen(argv[i])) && (argv[i][0]!=HGPU_CHAR_MINUS) && (argv[i][0]!=HGPU_CHAR_SLASH)) {
            HGPU_parameter** parameters_ini = HGPU_parameters_get_from_file(argv[i]);
            HGPU_parameters_join(&result,parameters_ini);
            HGPU_parameters_delete(&parameters_ini);
            i=argc;
        }
        i++;
    }
    HGPU_parameter** parameters_commandline = HGPU_parameters_get_from_commandline(argc,argv);
    HGPU_parameters_join(&result,parameters_commandline);
    HGPU_parameters_delete(&parameters_commandline);
    return result;
}

// get parameters from .inf-file
HGPU_parameter**
HGPU_parameters_get_from_inf_file(int inf_index){
    char* file_name = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    char* buffer_inf = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    if ((!file_name) || (!buffer_inf)) HGPU_error(HGPU_ERROR_NO_MEMORY);

	sprintf_s(buffer_inf,HGPU_FILENAME_MAX,"program%u.inf",inf_index);
    HGPU_io_path_join_filename(&file_name,HGPU_FILENAME_MAX,HGPU_io_path_inf,buffer_inf);

    HGPU_parameter** result = HGPU_parameters_get_from_file(file_name);

    free(buffer_inf);
    free(file_name);
    return result;
}

// write parameters to .inf-file
void
HGPU_parameters_write_to_inf_file(HGPU_parameter** parameters,const unsigned char* binary,const size_t bytes_in_binary,int inf_index){
    char* file_name = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    char* buffer_inf = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    if ((!file_name) || (!buffer_inf)) HGPU_error(HGPU_ERROR_NO_MEMORY);

	sprintf_s(buffer_inf,HGPU_FILENAME_MAX,"program%u.inf",inf_index);
    HGPU_io_path_join_filename(&file_name,HGPU_FILENAME_MAX,HGPU_io_path_inf,buffer_inf);

    char* inf_data = HGPU_parameters_put(parameters);
    HGPU_io_file_write(file_name,inf_data);

    sprintf_s(buffer_inf,HGPU_FILENAME_MAX,"program%u.bin",inf_index);
    HGPU_io_path_join_filename(&file_name,HGPU_FILENAME_MAX,HGPU_io_path_inf,buffer_inf);
    HGPU_io_file_write_binary(file_name,binary,bytes_in_binary);

    free(inf_data);
    free(buffer_inf);
    free(file_name);
}


// join two (HGPU_parameter) arrays
void
HGPU_parameters_join(HGPU_parameter*** parameters1,HGPU_parameter** parameters2){
    if ((!parameters2) || ((*parameters1)==parameters2)) return;
    unsigned int index = 0;
    while (parameters2[index]) HGPU_parameters_add(parameters1,parameters2[index++]);
}

// load array of (HGPU_parameter) parameters from file with path
HGPU_parameter**
HGPU_parameters_get_from_file_with_path(const char* file_path,const char* file_name){
    char* buffer = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    if (!buffer) HGPU_error(HGPU_ERROR_NO_MEMORY);
    HGPU_io_path_join_filename(&buffer,HGPU_FILENAME_MAX,file_path,file_name);
    free(buffer);
    return HGPU_parameters_get_from_file(buffer);
}

// create new parameter
HGPU_parameter*
HGPU_parameter_new(void){
    HGPU_parameter* parameter = (HGPU_parameter*) calloc(1,sizeof(HGPU_parameter));
    parameter->parameter          = NULL;
    parameter->value_text         = NULL;
    parameter->value_integer      = 0;
    parameter->value_double       = 0.0;
    return parameter;
}

// delete parameter
void
HGPU_parameter_delete(HGPU_parameter** parameter){
    if (!parameter) return;
    if (*parameter) {
        free((*parameter)->parameter);
        free((*parameter)->value_text);
        free((*parameter));
        (*parameter) = NULL;
    }
}

// clone parameter
HGPU_parameter*
HGPU_parameter_clone(HGPU_parameter* parameter){
    HGPU_parameter* result = NULL;
    if (parameter) {
         result = HGPU_parameter_new();
         result->parameter     = HGPU_string_clone(parameter->parameter);
         result->value_text    = HGPU_string_clone(parameter->value_text);
         result->value_double  = parameter->value_double;
         result->value_integer = parameter->value_integer;
    }
    return result;
}

// copy parameter
void
HGPU_parameter_copy(HGPU_parameter** parameter_dst,HGPU_parameter* parameter_src){
    HGPU_parameter* result = NULL;
    HGPU_parameter_delete(parameter_dst);
    result = HGPU_parameter_clone(parameter_src);
    (*parameter_dst) = result;
}

// print parameter
void
HGPU_parameter_print(HGPU_parameter* parameter){
    if (!parameter->parameter) return;
    printf("[%s]",parameter->parameter);
    if (parameter->value_text) printf(" = [%s] : %u : %e",parameter->value_text,parameter->value_integer,parameter->value_double);
    printf("\n");
}

// compare parameter with required parameter name
bool
HGPU_parameter_check_name(HGPU_parameter* parameter,const char* parameter_name){
    bool result = false;
    if ((parameter) && (parameter_name) && (parameter->parameter) && (strlen(parameter_name)>0)){
        size_t new_name_length = strlen(parameter_name)+1;
        char* new_name = (char*) calloc(new_name_length,sizeof(char));
        if (!new_name) HGPU_error(HGPU_ERROR_NO_MEMORY);
        sprintf_s(new_name,new_name_length,"%s",parameter_name);
        HGPU_string_trim(new_name);
        if (!strcmp(parameter->parameter,new_name)) result = true;
        free(new_name);
    }
    return result;
}

// compare parameter text value with required parameter name
bool
HGPU_parameter_check_value(HGPU_parameter* parameter1,HGPU_parameter* parameter2){
    bool result = false;
    if ((parameter1) && (parameter2) && (parameter1->parameter) && (parameter2->parameter) &&
        (strlen(parameter1->parameter)>0 && (strlen(parameter2->parameter)>0))){
        if (!strcmp(parameter1->parameter,parameter2->parameter)) result = true;
        if ((parameter1->value_text) && (parameter2->value_text) && (strlen(parameter1->value_text)>0 && (strlen(parameter2->value_text)>0))){
            if (!strcmp(parameter1->value_text,parameter2->value_text)) result = true;
            else result = false;
        }
    }
    return result;
}

// get parameter from file, pointed separately by file_path and file_name
HGPU_parameter*
HGPU_parameter_get_from_file_with_path(FILE** stream,const char* file_path,const char* file_name){
    char* buffer = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    if (!buffer) HGPU_error(HGPU_ERROR_NO_MEMORY);
    HGPU_io_path_join_filename(&buffer,HGPU_FILENAME_MAX,file_path,file_name);
    free(buffer);
    return HGPU_parameter_get_from_file(stream,buffer);
}

// get parameter from file, pointed by file_name
HGPU_parameter*
HGPU_parameter_get_from_file(FILE** stream,const char* file_name){
    HGPU_parameter* result = NULL;
	FILE* file_stream = NULL;
	fopen_s(&file_stream,file_name,"r");
	if(file_stream){
        (*stream) = file_stream;
        result = HGPU_parameter_get_from_file_next(stream);
        while ((result) && (!result->parameter)) result = HGPU_parameter_get_from_file_next(stream);
    }
    return result;
}

// get parameter from file, pointed by file-stream
HGPU_parameter*
HGPU_parameter_get_from_file_next(FILE** stream){
    HGPU_parameter* result = NULL;
    char buffer[HGPU_GPU_MAX_FILE_READ_BUFFER];
    if (!feof(*stream)) {
	    if (fgets(buffer,HGPU_GPU_MAX_FILE_READ_BUFFER,*stream)!=NULL)
            result = HGPU_parameter_get(buffer);
        if (!result) result = HGPU_parameter_new();
    } else
        fclose(*stream); // result=NULL if EOF
    return result;
}

// get parameter from string
HGPU_parameter*
HGPU_parameter_get(char* line){
// [0] N1 spaces or tabs[left_index]PARAMETER[left_index2] N2 spaces or tabs[eq_index]=N3 spaces or tabs[right_index]VALUE[right_index2] N4 spaces or tabs or \r or \n[line_length]
// or
// [0] N1 spaces or tabs[left_index]PARAMETER[left_index2] N2 spaces or tabs or \r or \n[line_length]
// N1, N2, N3, N4 >= 0
// # is remark at any line position
//
    HGPU_parameter* result = NULL;
    size_t parameter_length = 0;
    size_t value_length = 0;
    if ((line) && (strlen(line)>0)){
        size_t remark_index = strcspn(line,HGPU_SYMBOL_REMARK);
        size_t eq_index     = strcspn(line,HGPU_SYMBOL_EQ);
        size_t ret_index    = strcspn(line,HGPU_SYMBOL_NEWLINE);
        size_t end_index    = MIN(remark_index,ret_index);
        size_t value_index  = MIN(end_index,eq_index);
        size_t left_index   = 0;
        size_t left_index2  = 0;
        size_t right_index  = 0;
        size_t right_index2 = 0;
        while ((left_index<value_index) &&
               (isspace(line[left_index])  || (line[left_index]==HGPU_CHAR_TAB)  || (line[left_index]==HGPU_CHAR_CR)))
            left_index++;
        left_index2 = value_index;
        while ((left_index2>0) && (left_index<left_index2--) &&
               (isspace(line[left_index2]) || (line[left_index2]==HGPU_CHAR_TAB) || (line[left_index2]==HGPU_CHAR_CR)));
        left_index2++;
        if ((left_index2>left_index) && (left_index2<=eq_index) && (left_index<end_index)) {
            result = HGPU_parameter_new();
            parameter_length = left_index2-left_index;
            result->parameter = (char*) calloc(parameter_length+1,sizeof(char));
            if (!result->parameter) HGPU_error(HGPU_ERROR_NO_MEMORY);
            memcpy_s(result->parameter,parameter_length+1,line+left_index,parameter_length);
            result->parameter[parameter_length] = 0;

            if (eq_index<remark_index) {
                right_index = eq_index+1;
                while ((right_index<end_index) && 
                       (isspace(line[right_index]) || (line[right_index]==HGPU_CHAR_TAB || (line[right_index]==HGPU_CHAR_CR))))
                    right_index++;
                right_index2 = end_index;
                while ((right_index2>left_index2) && (right_index<right_index2--) &&
                       (isspace(line[right_index2]) || (line[right_index2]==HGPU_CHAR_TAB) || (line[right_index2]==HGPU_CHAR_CR)));
                right_index2++;
                if ((right_index2>right_index) && (right_index2<=end_index)) {
                    value_length = right_index2-right_index;
                    result->value_text = (char*) calloc(value_length+1,sizeof(char));
                    if (!result->value_text) HGPU_error(HGPU_ERROR_NO_MEMORY);
                    memcpy_s(result->value_text,value_length+1,line+right_index,value_length);
                    result->value_text[value_length] = 0;
                    sscanf_s(result->value_text,"%d", &result->value_integer);
                    sscanf_s(result->value_text,"%lf",&result->value_double);
                }
            }
        }
    }
    return result;
}

// get parameter from command line
HGPU_parameter*
HGPU_parameter_get_from_commandline(char* line){
    HGPU_parameter* result = NULL;
    if ((line) && (strlen(line))) {
        char* line_new = line;
        size_t line_length = strlen(line);
        size_t line_index = 0;
        while ((line_index<line_length) && ((line[line_index]==HGPU_CHAR_MINUS) || (line[line_index]==HGPU_CHAR_SLASH)
            || (line[line_index]==HGPU_CHAR_SPACE))) line_index++;
        if (line_index<line_length) line_new = line + line_index;
        result = HGPU_parameter_get(line_new);
    }
    return result;
}

// get parameter from from environment variables (it should have HGPU_ENVIRONMENT_PREFIX prefix like: DEVICE->HGPU_DEVICE)
HGPU_parameter*
HGPU_parameter_get_from_environment(const char* parameter_name){
    HGPU_parameter* result = NULL;
    if (!parameter_name) return result;
    char* p_name = (char*) parameter_name;
    if (!strstr(p_name,HGPU_ENVIRONMENT_PREFIX)) {
        p_name = (char*) calloc(HGPU_GPU_MAX_ENVIRONMENT_LENGTH,sizeof(char));
        if (!p_name) HGPU_error(HGPU_ERROR_NO_MEMORY);
        sprintf_s(p_name,HGPU_GPU_MAX_ENVIRONMENT_LENGTH,"%s%s",HGPU_ENVIRONMENT_PREFIX,parameter_name);
    }

    char* parameter = NULL;
#ifdef _WIN32
    size_t parameter_length = HGPU_GPU_MAX_ENVIRONMENT_LENGTH;
#endif
    _dupenv_s(&parameter,&parameter_length,p_name);
    if ((parameter) && (strlen(p_name)>strlen(HGPU_ENVIRONMENT_PREFIX))) {
        char* param = (char*) calloc(HGPU_GPU_MAX_ENVIRONMENT_LENGTH,sizeof(char));
        if (!param) HGPU_error(HGPU_ERROR_NO_MEMORY);
        char* param_name = (char*) p_name + strlen(HGPU_ENVIRONMENT_PREFIX);
        sprintf_s(param,HGPU_GPU_MAX_ENVIRONMENT_LENGTH,"%s=%s",param_name,parameter);
        result = HGPU_parameter_get(param);
        free(param);
    }
    if (p_name!=parameter_name) free(p_name);
    return result;
}

// put parameters into char* string
char*
HGPU_parameters_put(HGPU_parameter** parameters){
    char* result = NULL;
    if (parameters) {
        unsigned int index = 0;
        while(parameters[index]) {
            if (parameters[index]->parameter){
                char* parameter = HGPU_parameter_put(parameters[index]);
                HGPU_string_join(&result,parameter);
                free(parameter);
                HGPU_string_join(&result,(char*) HGPU_SYMBOL_NEWLINE);
            }
            index++;
        }
    }
    return result;
}

// put parameter into char* string
char*
HGPU_parameter_put(HGPU_parameter* parameter){
    char* result = NULL;
    if (parameter) {
        if ((parameter->value_text) && (strlen(parameter->value_text))) {
            size_t str_length = strlen(parameter->parameter) + 1 + strlen(parameter->value_text) + 1;
            result = (char*) calloc(str_length,sizeof(char));
            if (!result) HGPU_error(HGPU_ERROR_NO_MEMORY);
            sprintf_s(result,str_length,"%s=%s",parameter->parameter,parameter->value_text);
        } else {
            size_t str_length = strlen(parameter->parameter) + 1;
            result = (char*) calloc(str_length,sizeof(char));
            if (!result) HGPU_error(HGPU_ERROR_NO_MEMORY);
            sprintf_s(result,str_length,"%s",parameter->parameter);
        }
    }
    return result;
}

// get parameter from couple (parameter,value_text) [equivalent to parameter=value_text]
HGPU_parameter*
HGPU_parameter_get_with_name_text(char* parameter,const char* value_text){
    HGPU_parameter* result = NULL;
    if ((parameter) && (value_text)) {
        result = HGPU_parameter_new();
        result->parameter  = HGPU_string_clone(parameter);
        result->value_text = HGPU_string_clone(value_text);
    }
    return result;
}

// get parameter from couple (parameter,value_integer) [equivalent to parameter=value_integer]
HGPU_parameter*
HGPU_parameter_get_with_name_integer(char* parameter,int value_integer){
    HGPU_parameter* result = NULL;
    if (parameter) {
        result = HGPU_parameter_new();
        result->parameter     = HGPU_string_clone(parameter);
        result->value_integer = value_integer;
        result->value_double  = value_integer;
        result->value_text    = HGPU_string_get_from_integer(value_integer);
    }
    return result;
}

// get parameter from couple (parameter,value_double) [equivalent to parameter=value_double]
HGPU_parameter*
HGPU_parameter_get_with_name_double(char* parameter,double value_double){
    HGPU_parameter* result = NULL;
    if (parameter) {
        result = HGPU_parameter_new();
        result->parameter     = HGPU_string_clone(parameter);
        result->value_integer = (int) value_double;
        result->value_double  = value_double;
        result->value_text    = HGPU_string_get_from_double(value_double);
    }
    return result;
}

// setup pathes according to parameters
void
HGPU_parameters_path_setup(HGPU_parameter** parameters){
    HGPU_io_path_get(&HGPU_io_path_inf);
    HGPU_io_path_get(&HGPU_io_path_working);
    HGPU_io_path_get(&HGPU_io_path_current);
    HGPU_io_path_get_root();

    if (parameters) return;
    HGPU_parameter* path_cl_root = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PATH_CL_ROOT);
    HGPU_parameter* path_inf     = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PATH_INF);
    HGPU_parameter* path_working = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PATH_WORKING);
    if (path_cl_root) {
        memcpy_s(HGPU_io_path_root,HGPU_FILENAME_MAX,path_cl_root->value_text,strlen(path_cl_root->value_text));
        HGPU_io_path_set_separator(&HGPU_io_path_root,HGPU_FILENAME_MAX);
    }
    if (path_inf) {
        memcpy_s(HGPU_io_path_inf,HGPU_FILENAME_MAX,path_inf->value_text,strlen(path_inf->value_text));
        HGPU_io_path_set_separator(&HGPU_io_path_inf,HGPU_FILENAME_MAX);
    }
    if (path_working){
        memcpy_s(HGPU_io_path_working,HGPU_FILENAME_MAX,path_working->value_text,strlen(path_working->value_text));
        HGPU_io_path_set_separator(&HGPU_io_path_working,HGPU_FILENAME_MAX);
    }
}

// get precision from parameters
HGPU_precision
HGPU_parameters_get_precision(HGPU_parameter** parameters,const char* parameter_name){
    HGPU_precision result = HGPU_precision_none;
    HGPU_parameter* parameter_precision = HGPU_parameters_get_by_name(parameters,(char*) parameter_name);
    if (parameter_precision) result = HGPU_parameter_get_precision(parameter_precision,parameter_name);
    return result;
}

// get precision from parameter
HGPU_precision
HGPU_parameter_get_precision(HGPU_parameter* parameter,const char* parameter_name){
    HGPU_precision result = HGPU_precision_none;
    if (parameter && HGPU_string_cmp(parameter->parameter,parameter_name) && (parameter->value_text)) {
        if (strlen(parameter->value_text)>1)
            result = HGPU_convert_precision_from_str(parameter->value_text);
        else
            result = HGPU_convert_precision_from_uint(parameter->value_integer);
    }
    return result;
}
