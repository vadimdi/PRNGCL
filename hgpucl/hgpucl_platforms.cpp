/******************************************************************************
 * @file     hgpucl_platforms.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           Interface for OpenCL AMD APP & nVidia SDK environment
 *           platforms submodule
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

#include "hgpucl_platforms.h"

#define HGPU_GPU_PLATFORM_GET_STR_PARAMETER(param) {                                                                   \
    size_t parameter_length;                                                                                           \
    HGPU_GPU_error(clGetPlatformInfo(platform,param,0,NULL,&parameter_length),"clGetPlatformInfo failed");             \
    char* result_str = (char*) calloc(parameter_length,sizeof(char));                                                  \
    HGPU_GPU_error(clGetPlatformInfo(platform,param,(parameter_length),result_str,NULL),"clGetPlatformInfo failed");   \
    return result_str;                                                                                                 \
}



// new OpenCL platforms
HGPU_GPU_platforms*
HGPU_GPU_platforms_new(unsigned int number_of_platforms){
    if (!number_of_platforms) HGPU_GPU_error_message(HGPU_ERROR_NO_PLATFORM,NULL);
    if (number_of_platforms>HGPU_GPU_MAX_PLATFORMS) HGPU_GPU_error_message(HGPU_ERROR_NO_MEMORY,"exceed maximal number of descriptions for platforms");
    HGPU_GPU_platforms* result = (HGPU_GPU_platforms*) calloc(1,sizeof(HGPU_GPU_platforms));
        result->platforms           = (cl_platform_id*) calloc(number_of_platforms,sizeof(cl_platform_id));
        result->number_of_platforms = number_of_platforms;
    return result;
}

// delete OpenCL platforms
void
HGPU_GPU_platforms_delete(HGPU_GPU_platforms** platforms){
    if (!(*platforms)) return;
    free((*platforms)->platforms);
    free(*platforms);
    (*platforms)=NULL;
}


// get all OpenCL platforms
HGPU_GPU_platforms*
HGPU_GPU_platforms_get(void){
    cl_uint GPU_platforms_number = 0;
    HGPU_GPU_error_message(clGetPlatformIDs(0,NULL,&GPU_platforms_number),"clGetPlatformIDs failed");
    if(!GPU_platforms_number) HGPU_GPU_error_message(HGPU_ERROR_NO_PLATFORM,"there are no any available OpenCL platforms");

    HGPU_GPU_platforms* result = HGPU_GPU_platforms_new(GPU_platforms_number);
    HGPU_GPU_error_message(clGetPlatformIDs(GPU_platforms_number,result->platforms,&result->number_of_platforms),"clGetPlatformIDs failed");
    if(!result->number_of_platforms) HGPU_GPU_error_message(HGPU_ERROR_NO_PLATFORM,"there are no any available OpenCL platforms");

    return result;
}

// get OpenCL platforms by vendor
HGPU_GPU_platforms*
HGPU_GPU_platforms_get_by_vendor(HGPU_GPU_vendor vendor){
    HGPU_GPU_platforms* result = HGPU_GPU_platforms_get();
    HGPU_GPU_platforms_select_by_vendor(&result,vendor);
    return result;
}

// get total number of OpenCL platforms
unsigned int
HGPU_GPU_platforms_get_total_number(void){
    HGPU_GPU_platforms* platforms = HGPU_GPU_platforms_get();
    unsigned int result = HGPU_GPU_platforms_get_number(platforms);
    HGPU_GPU_platforms_delete(&platforms);
    return result;
}

// get number of OpenCL platforms
unsigned int
HGPU_GPU_platforms_get_number(HGPU_GPU_platforms* platforms){
    return platforms->number_of_platforms;
}

// select OpenCL platforms by vendor
void
HGPU_GPU_platforms_select_by_vendor(HGPU_GPU_platforms** platforms,HGPU_GPU_vendor vendor){
    unsigned int platforms_number = HGPU_GPU_platforms_get_number(*platforms);
    if (!platforms_number) HGPU_GPU_error(HGPU_ERROR_NO_PLATFORM);
    char infobuf[HGPU_MAX_STR_INFO_LENGHT];
    cl_uint desired_platforms = 0;
    for(unsigned int i=0; i<platforms_number; i++){
        cl_platform_id GPU_platform = (*platforms)->platforms[i];
        HGPU_GPU_error_message(clGetPlatformInfo(GPU_platform,CL_PLATFORM_VENDOR,sizeof(infobuf),infobuf,NULL),"clGetPlatformInfo failed");
        if ((HGPU_convert_vendor_from_str(infobuf)==vendor) || (vendor==HGPU_GPU_vendor_any)) desired_platforms++;
    }
    if(!desired_platforms) HGPU_GPU_error(HGPU_ERROR_NO_PLATFORM);
    HGPU_GPU_platforms* result = HGPU_GPU_platforms_new(desired_platforms);
    int j=0;
    for(unsigned int i=0; i<platforms_number; i++){
        cl_platform_id GPU_platform = (*platforms)->platforms[i];
        HGPU_GPU_error_message(clGetPlatformInfo(GPU_platform,CL_PLATFORM_VENDOR,sizeof(infobuf),infobuf,NULL),"clGetPlatformInfo failed");
        if ((HGPU_convert_vendor_from_str(infobuf)==vendor) || (vendor==HGPU_GPU_vendor_any)) result->platforms[j++] = GPU_platform;
    }
    HGPU_GPU_platforms_delete(platforms);
    (*platforms) = result;
}

// select OpenCL platforms by OpenCL version
void
HGPU_GPU_platforms_select_by_version(HGPU_GPU_platforms** platforms,HGPU_GPU_version version){
    unsigned int platforms_number = HGPU_GPU_platforms_get_number(*platforms);
    if (!platforms_number) HGPU_GPU_error(HGPU_ERROR_NO_PLATFORM);
    cl_uint desired_platforms = 0;
    for(unsigned int i=0; i<platforms_number; i++){
        cl_platform_id platform = (*platforms)->platforms[i];
        HGPU_GPU_version version_gpu = HGPU_GPU_platform_get_version(platform);
        if (HGPU_GPU_version_check(version_gpu,version)) desired_platforms++;
    }
    if(!desired_platforms) HGPU_GPU_error(HGPU_ERROR_NO_PLATFORM);
    HGPU_GPU_platforms* result = HGPU_GPU_platforms_new(desired_platforms);
    int j=0;
    for(unsigned int i=0; i<platforms_number; i++){
        cl_platform_id platform = (*platforms)->platforms[i];
        HGPU_GPU_version version_gpu = HGPU_GPU_platform_get_version(platform);
        if (HGPU_GPU_version_check(version_gpu,version)) result->platforms[j++] = platform;
    }
    HGPU_GPU_platforms_delete(platforms);
    (*platforms) = result;
}


// single platform_________________________________________________________________________________________________

// get index of OpenCL platform by platform
unsigned int
HGPU_GPU_platform_get_index_of(cl_platform_id platform){
    unsigned int result = 0;
    HGPU_GPU_platforms* platforms = HGPU_GPU_platforms_get();
    bool flag = true;
    while ((result<HGPU_GPU_platforms_get_number(platforms))&&(flag)){
        if (platforms->platforms[result]==platform) flag = false;
        else result++;
    }
    HGPU_GPU_platforms_delete(&platforms);
    if (flag) HGPU_GPU_error_message(HGPU_ERROR_NO_PLATFORM,"there is no any desired OpenCL platform");
    return result;
}

// get OpenCL platforms by index
cl_platform_id
HGPU_GPU_platform_get_by_index(unsigned int platform_index){
    cl_platform_id result;
    HGPU_GPU_platforms* platforms = HGPU_GPU_platforms_get();
    if (HGPU_GPU_platforms_get_number(platforms)<=platform_index) {
        HGPU_GPU_platforms_delete(&platforms);
        HGPU_GPU_error_message(HGPU_ERROR_NO_PLATFORM,"there is no any desired OpenCL platform");
    }
    result = platforms->platforms[platform_index];
    HGPU_GPU_platforms_delete(&platforms);
    return result;
}

// get vendor of OpenCL platform
HGPU_GPU_vendor
HGPU_GPU_platform_get_vendor(cl_platform_id platform){
    size_t parameter_length;
    HGPU_GPU_error_message(clGetPlatformInfo(platform,CL_PLATFORM_VENDOR,0,NULL,&parameter_length),"clGetPlatformInfo failed");
    char* result_str = (char*) calloc(parameter_length+1,sizeof(char));
    HGPU_GPU_error_message(clGetPlatformInfo(platform,CL_PLATFORM_VENDOR,(parameter_length+1),result_str,NULL),"clGetPlatformInfo failed");
    HGPU_GPU_vendor vendor = HGPU_convert_vendor_from_str(result_str);
    free(result_str);
    return vendor;
}

// get OpenCL platform info
char*
HGPU_GPU_platform_get_info_str(cl_platform_id platform,cl_platform_info parameter){
    size_t parameter_length;
    HGPU_GPU_error_message(clGetPlatformInfo(platform,parameter,0,NULL,&parameter_length),"clGetPlatformInfo failed");
    char* result_str = (char*) calloc(parameter_length,sizeof(char));
    HGPU_GPU_error_message(clGetPlatformInfo(platform,parameter,(parameter_length),result_str,NULL),"clGetPlatformInfo failed");
    return result_str;
}

// get name of OpenCL platform
char*
HGPU_GPU_platform_get_name(cl_platform_id platform){
    char* platform_name = HGPU_GPU_platform_get_info_str(platform,CL_PLATFORM_NAME);
    HGPU_string_trim(platform_name);
    return platform_name;
}


// get first available platform
cl_platform_id
HGPU_GPU_platform_get_first(HGPU_GPU_platforms* platforms){
    if (!HGPU_GPU_platforms_get_number(platforms)) HGPU_GPU_error_message(HGPU_ERROR_NO_PLATFORM,"there is no any desired OpenCL platform");
    return platforms->platforms[0];
}


// get platform's OpenCL version
HGPU_GPU_version
HGPU_GPU_platform_get_version(cl_platform_id platform){
    HGPU_GPU_version version;
    char* version_str = HGPU_GPU_platform_get_info_str(platform,CL_PLATFORM_VERSION);
    version = HGPU_GPU_version_get(version_str);
    free(version_str);
    return version;
}

// get version from string
HGPU_GPU_version
HGPU_GPU_version_get(char* version_str){
    HGPU_GPU_version version;
        version.major = 0;
        version.minor = 0; 
    if (strlen(version_str)< 10) HGPU_GPU_error_message(HGPU_ERROR_BAD_OPENCL_VERSION,"bad OpenCL version");
    char* temp = HGPU_string_clone(version_str);
    HGPU_string_delete_symbols_except_numbers_and_dots(temp);
    HGPU_string_trim(temp);
    sscanf_s(temp,"%d.%d",&version.major,&version.minor);
    free(temp);
    return version;
}

// compare OpenCL versions:
// returns: FALSE if GPU_version<version
//          TRUE  else
bool
HGPU_GPU_version_check(HGPU_GPU_version GPU_version,HGPU_GPU_version version){
    if ((GPU_version.major<version.major) || ((GPU_version.major==version.major) && (GPU_version.minor<version.minor))) return false;
    return true;
}

// return MAX version
HGPU_GPU_version
HGPU_GPU_version_max(HGPU_GPU_version version1, HGPU_GPU_version version2){
    if (HGPU_GPU_version_check(version1,version2)) return version1;
    else return version2;
}

// return MIN version
HGPU_GPU_version
HGPU_GPU_version_min(HGPU_GPU_version version1, HGPU_GPU_version version2){
    if (HGPU_GPU_version_check(version1,version2)) return version2;
    else return version1;
}

// get short info on OpenCL platform
char*
HGPU_GPU_platform_get_info_short(cl_platform_id platform){
    char* result = (char*) calloc(HGPU_MAX_STR_INFO_LENGHT,sizeof(char));
    char* name       = HGPU_GPU_platform_get_name(platform);
    char* vendor     = HGPU_GPU_platform_get_info_str(platform,CL_PLATFORM_VENDOR);
    char* profile    = HGPU_GPU_platform_get_info_str(platform,CL_PLATFORM_PROFILE);
    char* version    = HGPU_GPU_platform_get_info_str(platform,CL_PLATFORM_VERSION);
    char* extensions = HGPU_GPU_platform_get_info_str(platform,CL_PLATFORM_EXTENSIONS);
        unsigned int j = 0;
        j  = sprintf_s(result+j,HGPU_MAX_STR_INFO_LENGHT-j,"       Platform: %s\n",name);
        j += sprintf_s(result+j,HGPU_MAX_STR_INFO_LENGHT-j,"platform vendor: %s\n",vendor);
        j += sprintf_s(result+j,HGPU_MAX_STR_INFO_LENGHT-j,"        profile: %s\n",profile);
        j += sprintf_s(result+j,HGPU_MAX_STR_INFO_LENGHT-j,"        version: %s\n",version);
        j += sprintf_s(result+j,HGPU_MAX_STR_INFO_LENGHT-j,"     extensions: %s\n",extensions);
    free(extensions);
    free(version);
    free(profile);
    free(vendor);
    free(name);
    return result;
}

// print short info on OpenCL platform
void
HGPU_GPU_platform_print_info(cl_platform_id platform){
    char* platform_info = HGPU_GPU_platform_get_info_short(platform);
    printf("%s",platform_info);
    free(platform_info);
}

// print OpenCL platform's name
void
HGPU_GPU_platform_print_name(cl_platform_id platform){
    char* platform_name = HGPU_GPU_platform_get_name(platform);
    printf("%s",platform_name);
    free(platform_name);
}
