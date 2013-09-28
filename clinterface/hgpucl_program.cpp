/******************************************************************************
 * @file     hgpucl_program.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0
 *
 * @brief    [HGPU library]
 *           Interface for OpenCL AMD APP & nVidia SDK environment
 *           program submodule
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

#include "hgpucl_program.h"

cl_program
HGPU_GPU_program_new(const char* source,HGPU_GPU_context* context){
    return HGPU_GPU_program_with_options_new(source,NULL,context);
}

cl_program
HGPU_GPU_program_with_options_new(const char* source,const char* options,HGPU_GPU_context* context){
    HGPU_timer timer = HGPU_timer_start();
// if no_cache - turn off environmental variable for CUDA (CUDA_CACHE_DISABLE=1)
#ifdef _WIN32
    if (context->debug_flags.no_cache) _putenv_s("CUDA_CACHE_DISABLE","1"); // available in Windows only
#endif
    cl_int GPU_error = CL_SUCCESS;
    cl_program program = 0;
    unsigned int program_number = context->number_of_programs + 1;
    HGPU_parameter** parameters = NULL;
    char* device_name = HGPU_GPU_device_get_name(context->device);
        HGPU_parameter* parameter_device = HGPU_parameter_get_with_name_text((char*) HGPU_PARAMETER_INF_DEVICE,device_name);
        free(device_name);
    char* md5 = HGPU_md5(source);
        HGPU_parameter* parameter_md5 = HGPU_parameter_get_with_name_text((char*) HGPU_PARAMETER_INF_MD5,md5);
        free(md5);
    char* platform_name = HGPU_GPU_platform_get_name(HGPU_GPU_device_get_platform(context->device));
        HGPU_parameter* parameter_platform = HGPU_parameter_get_with_name_text((char*) HGPU_PARAMETER_INF_PLATFORM,platform_name);
        free(platform_name);

    HGPU_parameter* parameter_number = HGPU_parameter_get_with_name_integer((char*) HGPU_PARAMETER_INF_NUMBER,program_number);

    char* options_new = HGPU_string_clone(options);
    if (context->debug_flags.warning_error){
        if ((!options_new) || (options_new && (!strstr(options_new,HGPU_OPENCL_OPTION_ERROR))))
            HGPU_string_join(&options_new,(char*) HGPU_OPENCL_OPTION_ERROR_SP);
    }

    HGPU_parameter* parameter_options = HGPU_parameter_get_with_name_text((char*) HGPU_PARAMETER_INF_OPTIONS,options);

    HGPU_parameters_add(&parameters,parameter_platform);
    HGPU_parameters_add(&parameters,parameter_device);
    HGPU_parameters_add(&parameters,parameter_number);
    HGPU_parameters_add(&parameters,parameter_options);
    HGPU_parameter** parameters_without_md5 = HGPU_parameters_clone(parameters);
    HGPU_parameters_add(&parameters,parameter_md5);

    int GPU_inf_max_n = HGPU_io_inf_file_get_max_index();

    int active_file = 0;
    if (!context->debug_flags.no_cache) {
        for (int i = 1; i <= GPU_inf_max_n; i++){
            HGPU_parameter** inf_data = HGPU_parameters_get_from_inf_file(i);
            if (HGPU_parameters_check(inf_data,parameters_without_md5)){
                if (HGPU_parameters_check(inf_data,parameters)){
                    // .inf-file found
                    active_file = i;
                    i = GPU_inf_max_n;
                } else {
                    // MD5 does not match
                    // kill current .inf-file and rename last .inf-file into current
                    HGPU_io_inf_file_delete(i);
                    if (i<GPU_inf_max_n) HGPU_io_inf_file_rename(active_file,i);    // check if file is last
                    GPU_inf_max_n--;
                    i--;
                }
            }
            HGPU_parameters_delete(&inf_data);
        }
    }

    if (active_file == 0) active_file = GPU_inf_max_n + 1;


    char* current_date = HGPU_timer_get_current_datetime();
        HGPU_parameter* parameter_date = HGPU_parameter_get_with_name_text((char*) HGPU_PARAMETER_INF_DATE,current_date);
        free(current_date);
    HGPU_parameters_add(&parameters,parameter_date);
        HGPU_parameter_delete(&parameter_date);
        HGPU_parameter_delete(&parameter_options);
        HGPU_parameter_delete(&parameter_number);
        HGPU_parameter_delete(&parameter_md5);
        HGPU_parameter_delete(&parameter_device);
        HGPU_parameter_delete(&parameter_platform);
        HGPU_parameters_delete(&parameters_without_md5);


    if ((active_file > GPU_inf_max_n) || (!HGPU_io_bin_file_check_existence(active_file))) {
        // compile file
        printf("\nprogram%u.bin is compiling... \n",program_number);

        program = HGPU_program_build(context->context,context->device,source,options_new);
        char* buildlog = HGPU_program_get_buildlog(program,context->device);
        if (buildlog) {
            if (context->debug_flags.brief_report) printf("Program buid log: [%s]\n",buildlog);
            if (context->debug_flags.warning_error) HGPU_error(HGPU_ERROR_WARNING_ERROR);
            free(buildlog);
        }

        unsigned char* binary_code = NULL;
        size_t binary_size = 0;
        HGPU_program_get_binary(program,context->device,&binary_code,&binary_size);
        if (!context->debug_flags.no_cache) HGPU_parameters_write_to_inf_file(parameters,binary_code,binary_size,active_file); // save .inf and .bin files

        printf("program%u.bin compilation done (%f seconds)!\n",program_number,HGPU_timer_get(timer));

    } else {
        // load binary file
        size_t binary_size = 0;
        unsigned char* binary = HGPU_io_bin_file_read(active_file,&binary_size);

        cl_int status;
        program = clCreateProgramWithBinary(context->context,1,&context->device,&binary_size,(const unsigned char**) &binary,&status, &GPU_error);
        HGPU_GPU_error_message(status,"clCreateProgramWithBinary failed");
        HGPU_GPU_error_message(GPU_error,"clCreateProgramWithBinary failed");

        HGPU_GPU_error_message(clBuildProgram(program,1,&context->device,options_new,NULL,NULL),"clBuildProgram failed");
        char* buildlog = HGPU_program_get_buildlog(program,context->device);
        if (buildlog) {
            if (context->debug_flags.brief_report) printf("Program buid log: [%s]\n",buildlog);
            // exit, if warning = error

            free(buildlog);
        }
        free(binary);
    }
    
    HGPU_parameters_delete(&parameters);
    HGPU_program_add(context,program);  // register program in context
    return program;
}


// add new program to the array of programs
void
HGPU_program_add(HGPU_GPU_context* context,cl_program program){
    if (!program) return;
    unsigned int number_of_programs = context->number_of_programs;
    cl_program* programs_new = (cl_program*) realloc(context->program,(number_of_programs+2)*sizeof(cl_program));
    if (!programs_new) HGPU_error(HGPU_ERROR_NO_MEMORY);
    context->program = programs_new;
    context->program[number_of_programs] = program;
    context->program[number_of_programs+1] = 0;
    context->number_of_programs++;
}

// simple functions_______________________________________________________________________________________________

// build program with source and options
cl_program
HGPU_program_build(cl_context context,cl_device_id device,const char* source,const char* options){
    cl_int GPU_error = CL_SUCCESS;
    cl_program result = clCreateProgramWithSource(context, 1,&source, NULL, &GPU_error);
    char* clBuildLog = NULL;
    size_t clBuildLog_size = 0;
    size_t clBuildLog_actual_size = 0;
    char* logz_head = (char*) "File: source.cl\nOptions: ";
    if (GPU_error!=CL_SUCCESS) {

        clGetProgramInfo(result,CL_PROGRAM_SOURCE,0,NULL,&clBuildLog_size);
		clBuildLog = (char*) calloc(clBuildLog_size+1, sizeof(char));
        clGetProgramInfo(result,CL_PROGRAM_SOURCE,clBuildLog_size+1,clBuildLog,&clBuildLog_actual_size);
        // write build log
        // join options and clBuildLog
        char* time_stamp = HGPU_timer_get_current_datetime();
        size_t logz_length = strlen(logz_head) + strlen(options) + strlen(time_stamp) + strlen(clBuildLog) + 32;
        char* logz = (char*) calloc(logz_length,sizeof(char));
        size_t j = sprintf_s(logz,logz_length,"%s%s\nTime: %s\nLogs:\n%s\n",logz_head,options,time_stamp,clBuildLog);
        printf("%s",logz);
        HGPU_io_file_write((char*) "error.txt",logz);
        free(logz);
        free(time_stamp);
        // write source code
        HGPU_io_file_write((char*) "source.cl",source);

        free(clBuildLog);
        HGPU_GPU_error_message(GPU_error,"clCreateProgramWithSource failed");
    }
    GPU_error = clBuildProgram(result, 1, &device, options, NULL, NULL);
    if (GPU_error!=CL_SUCCESS) {
        clGetProgramBuildInfo(result,device,CL_PROGRAM_BUILD_LOG,0,NULL,&clBuildLog_size);
        clBuildLog = (char*) calloc(clBuildLog_size+1, sizeof(char));
        clGetProgramBuildInfo(result,device, CL_PROGRAM_BUILD_LOG,clBuildLog_size+1,clBuildLog,&clBuildLog_actual_size);
        // write build log
        // join options and clBuildLog
        char* time_stamp = HGPU_timer_get_current_datetime();
        size_t logz_length = strlen(logz_head) + strlen(options) + strlen(time_stamp) + strlen(clBuildLog) + 32;
        char* logz = (char*) calloc(logz_length,sizeof(char));
        size_t j = sprintf_s(logz,logz_length,"%s%s\nTime: %s\nLog:\n%s\n",logz_head,options,time_stamp,clBuildLog);
        printf("%s",logz);
        HGPU_io_file_write((char*) "error.txt",logz);
        free(logz);
        free(time_stamp);
        // write source code
        HGPU_io_file_write((char*) "source.cl",source);

        free(clBuildLog);
    }
    HGPU_GPU_error_message(GPU_error,"clBuildProgram failed");

    return result;
}

// get program build log
char*
HGPU_program_get_buildlog(cl_program program,cl_device_id device){
    char*  buildlog = NULL;
    size_t buildlog_size = 0;
    HGPU_GPU_error_message(clGetProgramBuildInfo(program,device,CL_PROGRAM_BUILD_LOG,0,NULL,&buildlog_size),"clGetProgramBuildInfo failed");
    if (buildlog_size>4) {
        buildlog = (char*) calloc(buildlog_size,sizeof(char));
        if (!buildlog) HGPU_error(HGPU_ERROR_NO_MEMORY);
        HGPU_GPU_error_message(clGetProgramBuildInfo(program,device,CL_PROGRAM_BUILD_LOG,buildlog_size,buildlog, NULL),"clGetProgramBuildInfo failed");
    }
    return buildlog;
}

// get binary for program
void
HGPU_program_get_binary(cl_program program,cl_device_id device,unsigned char** binary_code,size_t* binary_size){
    cl_uint num_devices;
    HGPU_GPU_error_message(clGetProgramInfo(program, CL_PROGRAM_NUM_DEVICES, sizeof(cl_uint), &num_devices, NULL),"clGetProgramInfo1 failed");

    cl_device_id* devices = (cl_device_id*) calloc(num_devices, sizeof(cl_device_id));
    if (!devices) HGPU_error(HGPU_ERROR_NO_MEMORY);

    HGPU_GPU_error_message(clGetProgramInfo(program, CL_PROGRAM_DEVICES, num_devices * sizeof(cl_device_id), devices, 0),"clGetProgramInfo2 failed");

    size_t* binary_sizes = (size_t*) calloc(num_devices, sizeof(size_t));    
    if (!binary_sizes) HGPU_error(HGPU_ERROR_NO_MEMORY);

    HGPU_GPU_error_message(clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, num_devices * sizeof(size_t), binary_sizes, NULL),"clGetProgramInfo3 failed");

    unsigned char** binary = (unsigned char**) calloc(num_devices, sizeof(unsigned char*));
    if (!binary) HGPU_error(HGPU_ERROR_NO_MEMORY);
    for( unsigned int i=0; i<num_devices; ++i) binary[i]= (unsigned char*) malloc(binary_sizes[i]);

    HGPU_GPU_error_message(clGetProgramInfo(program, CL_PROGRAM_BINARIES, num_devices * sizeof(size_t), binary, NULL),"clGetProgramInfo4 failed");


    unsigned int idx = 0;
    while((idx<num_devices) && (devices[idx] != device)) ++idx;

    (*binary_code) = binary[idx];
    (*binary_size) = binary_sizes[idx];

    free(devices);
    free(binary_sizes);
    for(unsigned int i=0; i<num_devices; ++i) if (i!=idx) free(binary[i]);
    free(binary);
}
