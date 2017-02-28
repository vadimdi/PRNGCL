/******************************************************************************
 * @file     hgpucl_devices.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           Interface for OpenCL AMD APP & nVidia SDK environment
 *           devices submodule
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

#include "hgpucl_devices.h"

// new OpenCL devices
HGPU_GPU_devices*
HGPU_GPU_devices_new(unsigned int number_of_devices){
    if (!number_of_devices) HGPU_GPU_error(HGPU_ERROR_NO_DEVICE);
    if (number_of_devices>HGPU_GPU_MAX_DEVICES) HGPU_GPU_error_message(HGPU_ERROR_NO_MEMORY,"exceed maximal number of descriptions for devices");
    HGPU_GPU_devices* result = (HGPU_GPU_devices*) calloc(1,sizeof(HGPU_GPU_devices));
        result->devices           = (cl_device_id*) calloc(number_of_devices,sizeof(cl_device_id));
        result->number_of_devices = number_of_devices;
    return result;
}

// new OpenCL devices empty devices
HGPU_GPU_devices*
HGPU_GPU_devices_new_empty(void){
    HGPU_GPU_devices* result = (HGPU_GPU_devices*) calloc(1,sizeof(HGPU_GPU_devices));
        result->devices           = NULL;
        result->number_of_devices = 0;
    return result;
}

// delete OpenCL devices
void
HGPU_GPU_devices_delete(HGPU_GPU_devices** devices){
    if (!(*devices)) return;
    free((*devices)->devices);
    free(*devices);
    (*devices) = NULL;
}

// clone OpenCL devices
HGPU_GPU_devices*
HGPU_GPU_devices_clone(HGPU_GPU_devices* devices){
    unsigned int number_of_devices = HGPU_GPU_devices_get_number(devices);
    if (!number_of_devices) HGPU_GPU_error(HGPU_ERROR_NO_DEVICE);
    HGPU_GPU_devices* result = HGPU_GPU_devices_new(number_of_devices);
    size_t bytes = number_of_devices * sizeof(cl_device_id);
    memcpy_s(result->devices,bytes,devices->devices,bytes);
    return result;
}

// copy OpenCL devices
void
HGPU_GPU_devices_copy(HGPU_GPU_devices** devices_destination,HGPU_GPU_devices* devices_source){
    HGPU_GPU_devices_delete(devices_destination);
    (*devices_destination) = HGPU_GPU_devices_clone(devices_source);
}



// get all OpenCL devices
HGPU_GPU_devices*
HGPU_GPU_devices_get(void){
    HGPU_GPU_platforms* platforms = HGPU_GPU_platforms_get();
    HGPU_GPU_devices* result = HGPU_GPU_devices_get_on_platforms(platforms);
    HGPU_GPU_platforms_delete(&platforms);
    return result;
}

// get all OpenCL devices
HGPU_GPU_devices*
HGPU_GPU_devices_get_on_platforms(HGPU_GPU_platforms* platforms){
    unsigned int number_of_devices = HGPU_GPU_devices_get_number_on_platforms(platforms);
    HGPU_GPU_devices* result = HGPU_GPU_devices_new(number_of_devices);
    unsigned int device_index = 0;
    for (unsigned int i=0; i<HGPU_GPU_platforms_get_number(platforms); i++){
        unsigned int number_of_devices_on_platform = HGPU_GPU_devices_get_number_on_platform(platforms->platforms[i]);
        if (number_of_devices_on_platform){
            HGPU_GPU_devices* devices_on_platform = HGPU_GPU_devices_get_on_platform(platforms->platforms[device_index]);
            for (unsigned int j=0; j<number_of_devices_on_platform; j++)
                result->devices[device_index++] = devices_on_platform->devices[j];
            HGPU_GPU_devices_delete(&devices_on_platform);
        }
    }
    return result;
}

// get all OpenCL devices
HGPU_GPU_devices*
HGPU_GPU_devices_get_on_platform(cl_platform_id platform){
    cl_uint number_of_devices = HGPU_GPU_devices_get_number_on_platform(platform);
    HGPU_GPU_devices* result = HGPU_GPU_devices_new(number_of_devices);
    HGPU_GPU_error_message(clGetDeviceIDs(platform,CL_DEVICE_TYPE_ALL,number_of_devices, result->devices, &result->number_of_devices),"clGetDeviceIDs failed");
    if(!result->number_of_devices) HGPU_GPU_error_message(HGPU_ERROR_NO_PLATFORM,"there are no any available OpenCL devices");
    return result;
}

// get OpenCL devices by vendor
HGPU_GPU_devices*
HGPU_GPU_devices_get_by_vendor(HGPU_GPU_vendor vendor){
    HGPU_GPU_platforms* platforms = HGPU_GPU_platforms_get();
    HGPU_GPU_devices* result = HGPU_GPU_devices_get_on_platforms(platforms);
    HGPU_GPU_devices_select_by_vendor(&result,vendor);
    return result;
}

// get OpenCL devices by vendor on platform
HGPU_GPU_devices*
HGPU_GPU_devices_get_by_vendor_on_platform(cl_platform_id platform,HGPU_GPU_vendor vendor){
    HGPU_GPU_devices* result = HGPU_GPU_devices_get_on_platform(platform);
    HGPU_GPU_devices_select_by_vendor(&result,vendor);
    return result;
}

// get OpenCL devices by vendor on platforms
HGPU_GPU_devices*
HGPU_GPU_devices_get_by_vendor_on_platforms(HGPU_GPU_platforms* platforms,HGPU_GPU_vendor vendor){
    HGPU_GPU_devices* result = HGPU_GPU_devices_get_on_platforms(platforms);
    HGPU_GPU_devices_select_by_vendor(&result,vendor);
    return result;
}

//get max OpenCL version of devices on platforms
HGPU_GPU_version
HGPU_GPU_devices_get_version_max(HGPU_GPU_devices* devices){
    unsigned int number_of_devices = HGPU_GPU_devices_get_number(devices);
    if (!number_of_devices) HGPU_GPU_error(HGPU_ERROR_NO_DEVICE);
    HGPU_GPU_version version = HGPU_GPU_device_get_version(devices->devices[0]);
    for (unsigned int i=1; i<number_of_devices; i++)
        version = HGPU_GPU_version_max(version,HGPU_GPU_device_get_version(devices->devices[i]));
    return version;
}



// get number of OpenCL devices
unsigned int
HGPU_GPU_devices_get_number(HGPU_GPU_devices* devices){
    if (!devices) return 0;
    return devices->number_of_devices;
}

// get total number of OpenCL devices
unsigned int
HGPU_GPU_devices_get_total_number(void){
    HGPU_GPU_platforms* platforms = HGPU_GPU_platforms_get();
    unsigned int result = 0;
    for (unsigned int i=0;i<HGPU_GPU_platforms_get_number(platforms);i++)
        result += HGPU_GPU_devices_get_number_on_platform(platforms->platforms[i]);
    HGPU_GPU_platforms_delete(&platforms);
    return result;
}

// get number of OpenCL devices
unsigned int
HGPU_GPU_devices_get_number_on_platform(cl_platform_id platform){
    cl_uint number_of_devices = 0;
    cl_int get_number_of_devices_result = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &number_of_devices);
    if (get_number_of_devices_result!=CL_DEVICE_NOT_FOUND) HGPU_GPU_error_message(get_number_of_devices_result, "clGetDeviceIDs failed");
    return number_of_devices;
}

// get number of OpenCL devices on platforms
unsigned int
HGPU_GPU_devices_get_number_on_platforms(HGPU_GPU_platforms* platforms){
    unsigned int result = 0;
    for (unsigned int i=0;i<HGPU_GPU_platforms_get_number(platforms);i++)
        result += HGPU_GPU_devices_get_number_on_platform(platforms->platforms[i]);
    return result;
}

// select section_________________________________________________________________________________________________

// select OpenCL devices by vendor
void
HGPU_GPU_devices_select_by_vendor(HGPU_GPU_devices** devices,HGPU_GPU_vendor vendor){
    unsigned int number_of_devices = HGPU_GPU_devices_get_number(*devices);
    if (!number_of_devices) HGPU_GPU_error(HGPU_ERROR_NO_DEVICE);
    char infobuf[HGPU_MAX_STR_INFO_LENGHT];
    cl_uint desired_devices = 0;
    for(unsigned int i=0; i<number_of_devices; i++){
        cl_device_id GPU_device = (*devices)->devices[i];
        HGPU_GPU_error_message(clGetDeviceInfo(GPU_device,CL_DEVICE_VENDOR,sizeof(infobuf),infobuf,NULL),"clGetDeviceInfo failed");
        if ((HGPU_convert_vendor_from_str(infobuf)==vendor) || (vendor==HGPU_GPU_vendor_any)) desired_devices++;
    }
    HGPU_GPU_devices* result;
    if(desired_devices){
        result = HGPU_GPU_devices_new(desired_devices);
        int j=0;
        for(unsigned int i=0; i<number_of_devices; i++){
            cl_device_id GPU_device = (*devices)->devices[i];
            HGPU_GPU_error_message(clGetDeviceInfo(GPU_device,CL_DEVICE_VENDOR,sizeof(infobuf),infobuf,NULL),"clGetDeviceInfo failed");
            if ((HGPU_convert_vendor_from_str(infobuf)==vendor) || (vendor==HGPU_GPU_vendor_any)) result->devices[j++] = GPU_device;
        }
    } else
        result = HGPU_GPU_devices_new_empty();
    HGPU_GPU_devices_delete(devices);
    (*devices) = result;
}

// select OpenCL devices by OpenCL version
void
HGPU_GPU_devices_select_by_version(HGPU_GPU_devices** devices,HGPU_GPU_version version){
    unsigned int number_of_devices = HGPU_GPU_devices_get_number(*devices);
    if (!number_of_devices) HGPU_GPU_error(HGPU_ERROR_NO_DEVICE);
    cl_uint desired_devices = 0;
    for(unsigned int i=0; i<number_of_devices; i++){
        cl_device_id device = (*devices)->devices[i];
        HGPU_GPU_version version_gpu = HGPU_GPU_device_get_version(device);
        if (HGPU_GPU_version_check(version_gpu,version)) desired_devices++;
    }
    HGPU_GPU_devices* result;
    if(desired_devices){
        result = HGPU_GPU_devices_new(desired_devices);
        int device_index=0;
        for(unsigned int i=0; i<number_of_devices; i++){
            cl_device_id device = (*devices)->devices[i];
            HGPU_GPU_version version_gpu = HGPU_GPU_device_get_version(device);
            if (HGPU_GPU_version_check(version_gpu,version)) result->devices[device_index++] = device;
        }
    } else
        result = HGPU_GPU_devices_new_empty();
    HGPU_GPU_devices_delete(devices);
    (*devices) = result;
}

// select OpenCL devices by type
void
HGPU_GPU_devices_select_by_type(HGPU_GPU_devices** devices,cl_device_type device_type){
    unsigned int number_of_devices = HGPU_GPU_devices_get_number(*devices);
    if (!number_of_devices) HGPU_GPU_error(HGPU_ERROR_NO_DEVICE);
    cl_uint desired_devices = 0;
    for(unsigned int i=0; i<number_of_devices; i++){
        cl_device_id device = (*devices)->devices[i];
        cl_device_type dev_type;
        HGPU_GPU_error_message(clGetDeviceInfo(device,CL_DEVICE_TYPE,sizeof(dev_type),&dev_type,NULL),"clGetDeviceInfo failed");
        if (dev_type & device_type) desired_devices++;
    }
    HGPU_GPU_devices* result;
    if(desired_devices){
        result = HGPU_GPU_devices_new(desired_devices);
        int device_index=0;
        for(unsigned int i=0; i<number_of_devices; i++){
            cl_device_id device = (*devices)->devices[i];
            cl_device_type dev_type;
            HGPU_GPU_error_message(clGetDeviceInfo(device,CL_DEVICE_TYPE,sizeof(dev_type),&dev_type,NULL),"clGetDeviceInfo failed");
            if (dev_type & device_type) result->devices[device_index++] = device;
        }
    } else
        result = HGPU_GPU_devices_new_empty();
    HGPU_GPU_devices_delete(devices);
    (*devices) = result;
}

// select OpenCL devices by device available
void
HGPU_GPU_devices_select_by_device_and_compiler_available(HGPU_GPU_devices** devices){
    unsigned int number_of_devices = HGPU_GPU_devices_get_number(*devices);
    if (!number_of_devices) HGPU_GPU_error(HGPU_ERROR_NO_DEVICE);
    cl_uint desired_devices = 0;
    for(unsigned int i=0; i<number_of_devices; i++){
        cl_device_id device = (*devices)->devices[i];
        cl_bool device_available;
        cl_bool compiler_available;
        HGPU_GPU_error_message(clGetDeviceInfo(device,CL_DEVICE_AVAILABLE,sizeof(device_available),&device_available,NULL),"clGetDeviceInfo failed");
        HGPU_GPU_error_message(clGetDeviceInfo(device,CL_DEVICE_COMPILER_AVAILABLE,sizeof(device_available),&compiler_available,NULL),"clGetDeviceInfo failed");
        if (device_available & compiler_available) desired_devices++;
    }
    HGPU_GPU_devices* result;
    if(desired_devices){
        result = HGPU_GPU_devices_new(desired_devices);
        int device_index=0;
        for(unsigned int i=0; i<number_of_devices; i++){
            cl_device_id device = (*devices)->devices[i];
            cl_bool device_available;
            cl_bool compiler_available;
            HGPU_GPU_error_message(clGetDeviceInfo(device,CL_DEVICE_AVAILABLE,sizeof(device_available),&device_available,NULL),"clGetDeviceInfo failed");
            HGPU_GPU_error_message(clGetDeviceInfo(device,CL_DEVICE_COMPILER_AVAILABLE,sizeof(device_available),&compiler_available,NULL),"clGetDeviceInfo failed");
            if (device_available & compiler_available) result->devices[device_index++] = device;
        }
    } else
        result = HGPU_GPU_devices_new_empty();
    HGPU_GPU_devices_delete(devices);
    (*devices) = result;
}

// select OpenCL devices by global memory size
void
HGPU_GPU_devices_select_by_global_mem_size(HGPU_GPU_devices** devices,unsigned long int memory_size_in_bytes){
    HGPU_GPU_devices* result = (*devices);
    HGPU_GPU_devices_select_by_parameter_uint(&result,CL_DEVICE_GLOBAL_MEM_SIZE,memory_size_in_bytes);
    (*devices) = result;
}

// select OpenCL devices by max allocable memory size
void
HGPU_GPU_devices_select_by_max_alloc_mem(HGPU_GPU_devices** devices,unsigned long int memory_size_in_bytes){
    HGPU_GPU_devices* result = (*devices);
    HGPU_GPU_devices_select_by_parameter_uint(&result,CL_DEVICE_MAX_MEM_ALLOC_SIZE,memory_size_in_bytes);
    (*devices) = result;
}

// select OpenCL devices by uint parameter
void
HGPU_GPU_devices_select_by_parameter_uint(HGPU_GPU_devices** devices,cl_device_info parameter,unsigned long int min_value){
    unsigned int number_of_devices = HGPU_GPU_devices_get_number(*devices);
    if (!number_of_devices) HGPU_GPU_error(HGPU_ERROR_NO_DEVICE);
    cl_uint desired_devices = 0;
    for(unsigned int i=0; i<number_of_devices; i++){
        cl_device_id device = (*devices)->devices[i];
        cl_ulong device_output;
        HGPU_GPU_error_message(clGetDeviceInfo(device,parameter,sizeof(device_output),&device_output,NULL),"clGetDeviceInfo failed");
        if (device_output>=min_value) desired_devices++;
    }
    HGPU_GPU_devices* result;
    if(desired_devices){
        result = HGPU_GPU_devices_new(desired_devices);
        int device_index=0;
        for(unsigned int i=0; i<number_of_devices; i++){
            cl_device_id device = (*devices)->devices[i];
            cl_ulong device_output;
            HGPU_GPU_error_message(clGetDeviceInfo(device,parameter,sizeof(device_output),&device_output,NULL),"clGetDeviceInfo failed");
            if (device_output>=min_value) result->devices[device_index++] = device;
        }
    } else
        result = HGPU_GPU_devices_new_empty();
    HGPU_GPU_devices_delete(devices);
    (*devices) = result;
}

// select OpenCL devices, supporting double precision
void
HGPU_GPU_devices_select_by_double_precision(HGPU_GPU_devices** devices){
    unsigned int number_of_devices = HGPU_GPU_devices_get_number(*devices);
    if (!number_of_devices) HGPU_GPU_error(HGPU_ERROR_NO_DEVICE);
    cl_uint desired_devices = 0;
    for(unsigned int i=0; i<number_of_devices; i++)
        if (HGPU_GPU_device_check_double_precision((*devices)->devices[i])) desired_devices++;
    HGPU_GPU_devices* result;
    if(desired_devices){
        result = HGPU_GPU_devices_new(desired_devices);
        int device_index=0;
        for(unsigned int i=0; i<number_of_devices; i++)
            if (HGPU_GPU_device_check_double_precision((*devices)->devices[i])) result->devices[device_index++] = (*devices)->devices[i];
    } else
        result = HGPU_GPU_devices_new_empty();
    HGPU_GPU_devices_delete(devices);
    (*devices) = result;
}

void
HGPU_GPU_devices_select_by_highest_version(HGPU_GPU_devices** devices){
    HGPU_GPU_version version = HGPU_GPU_devices_get_version_max(*devices);
    HGPU_GPU_devices_select_by_version(devices,version);
}

void
HGPU_GPU_devices_exclude_device(HGPU_GPU_devices** devices,cl_device_id device){
    unsigned int number_of_devices = HGPU_GPU_devices_get_number(*devices);
    if (!number_of_devices){
        HGPU_GPU_devices_delete(devices);
        return;
    }
    unsigned int number_of_devices_result = 0;
    for (unsigned int i=0; i<number_of_devices; i++) if ((*devices)->devices[i]!=device) number_of_devices_result++;
    if (!number_of_devices_result){
        HGPU_GPU_devices_delete(devices);
        return;
    }
    HGPU_GPU_devices* result = HGPU_GPU_devices_new(number_of_devices_result);
    unsigned int j = 0;
    for (unsigned int i=0; i<number_of_devices; i++)
        if (((*devices)->devices[i]) && ((*devices)->devices[i]!=device))
            result->devices[j++]=(*devices)->devices[i];
    HGPU_GPU_devices_delete(devices);
    (*devices) = result;
}

// print names of OpenCL devices
void
HGPU_GPU_devices_print(HGPU_GPU_devices* devices){
    if (!devices) return;
    unsigned int number_of_devices = HGPU_GPU_devices_get_number(devices);
    for (unsigned int i=0; i<number_of_devices; i++){
        char* device_name = HGPU_GPU_device_get_name(devices->devices[i]);
        printf("Device [%u]: %s\n",i,device_name);
        free(device_name);
    }
}

// print names of platforms of OpenCL devices
void
HGPU_GPU_devices_print_platforms(HGPU_GPU_devices* devices){
    if (!devices) return;
    unsigned int number_of_devices = HGPU_GPU_devices_get_number(devices);
    for (unsigned int i=0; i<number_of_devices; i++){
        char* platform_name = HGPU_GPU_platform_get_name(HGPU_GPU_device_get_platform(devices->devices[i]));
        printf("Platform [%u]: %s\n",i,platform_name);
        free(platform_name);
    }
}


// single device___________________________________________________________________________________________________

// get index of OpenCL device by device
unsigned int
HGPU_GPU_device_get_index_of(cl_device_id device){
    unsigned int result = 0;
    HGPU_GPU_devices* devices = HGPU_GPU_devices_get();
    bool flag = true;
    while ((result<HGPU_GPU_devices_get_number(devices))&&(flag)){
        if (devices->devices[result]==device) flag = false;
        else result++;
    }
    HGPU_GPU_devices_delete(&devices);
    if (flag) HGPU_GPU_error_message(HGPU_ERROR_NO_DEVICE,"there is no any desired OpenCL device");
    return result;
}

// get index of OpenCL device by device on platform
unsigned int
HGPU_GPU_device_get_index_of_on_platform(cl_device_id device){
    unsigned int result = 0;
    cl_platform_id platform = HGPU_GPU_device_get_platform(device);
    HGPU_GPU_devices* devices = HGPU_GPU_devices_get_on_platform(platform);
    bool flag = true;
    while ((result<HGPU_GPU_devices_get_number(devices))&&(flag)){
        if (devices->devices[result]==device) flag = false;
        else result++;
    }
    HGPU_GPU_devices_delete(&devices);
    if (flag) HGPU_GPU_error_message(HGPU_ERROR_NO_DEVICE,"there is no any desired OpenCL device");
    return result;
}

// get platform of OpenCL device
cl_platform_id
HGPU_GPU_device_get_platform(cl_device_id device){
    cl_platform_id platform;
    HGPU_GPU_error_message(clGetDeviceInfo(device,CL_DEVICE_PLATFORM,sizeof(platform),&platform,NULL),"clGetDeviceInfo (get platform) failed");
    return platform;
}

// get index of platform for OpenCL device
unsigned int
HGPU_GPU_device_get_platform_index_of(cl_device_id device){
    unsigned int platform_index = HGPU_GPU_platform_get_index_of(HGPU_GPU_device_get_platform(device));
    return platform_index;
}

// get OpenCL device by index
cl_device_id
HGPU_GPU_device_get_by_index(unsigned int device_index){
    cl_device_id result;
    HGPU_GPU_devices* devices = HGPU_GPU_devices_get();
    if (HGPU_GPU_devices_get_number(devices)<=device_index) {
        HGPU_GPU_devices_delete(&devices);
        HGPU_GPU_error_message(HGPU_ERROR_NO_DEVICE,"there is no any desired OpenCL device");
    }
    result = devices->devices[device_index];
    HGPU_GPU_devices_delete(&devices);
    return result;
}

// get OpenCL device by index
cl_device_id
HGPU_GPU_device_get_by_index_on_platform(cl_platform_id platform,unsigned int device_index){
    cl_device_id result;
    HGPU_GPU_devices* devices = HGPU_GPU_devices_get_on_platform(platform);
    if (HGPU_GPU_devices_get_number(devices)<=device_index) {
        HGPU_GPU_devices_delete(&devices);
        HGPU_GPU_error_message(HGPU_ERROR_NO_DEVICE,"there is no any desired OpenCL device");
    }
    result = devices->devices[device_index];
    HGPU_GPU_devices_delete(&devices);
    return result;
}

// get OpenCL device info
char*
HGPU_GPU_device_get_info_str(cl_device_id device,cl_device_info parameter){
    size_t parameter_length;
    HGPU_GPU_error_message(clGetDeviceInfo(device,parameter,0,NULL,&parameter_length),"clGetDeviceInfo failed");
    char* result_str = (char*) calloc(parameter_length,sizeof(char));
    HGPU_GPU_error_message(clGetDeviceInfo(device,parameter,(parameter_length),result_str,NULL),"clGetDeviceInfo failed");
    return result_str;
}

// get OpenCL device info (unsigned long int)
unsigned long int
HGPU_GPU_device_get_info_uint(cl_device_id device,cl_device_info parameter){
    cl_ulong result = 0;
    HGPU_GPU_error_message(clGetDeviceInfo(device,parameter,sizeof(result),&result,NULL),"clGetDeviceInfo failed");
    return (unsigned long int) result;
}

char*
HGPU_GPU_device_get_name(cl_device_id device){
    char* device_name = HGPU_GPU_device_get_info_str(device,CL_DEVICE_NAME);
    HGPU_string_trim(device_name);
    return device_name;
}

// get short info on OpenCL device
char*
HGPU_GPU_device_get_info_short(cl_device_id device){
    char* result = (char*) calloc(HGPU_MAX_STR_INFO_LENGHT,sizeof(char));
    char* name       = HGPU_GPU_device_get_name(device);
    char* vendor     = HGPU_GPU_device_get_info_str(device,CL_DEVICE_VENDOR);
    char* profile    = HGPU_GPU_device_get_info_str(device,CL_DEVICE_PROFILE);
    char* version    = HGPU_GPU_device_get_info_str(device,CL_DEVICE_VERSION);
    char* extensions = HGPU_GPU_device_get_info_str(device,CL_DEVICE_EXTENSIONS);
    unsigned int j = 0;
    j  = sprintf_s(result+j,HGPU_MAX_STR_INFO_LENGHT-j,"         Device: %s\n",name);
    j += sprintf_s(result+j,HGPU_MAX_STR_INFO_LENGHT-j,"device's vendor: %s\n",vendor);
    j += sprintf_s(result+j,HGPU_MAX_STR_INFO_LENGHT-j,"        profile: %s\n",profile);
    j += sprintf_s(result+j,HGPU_MAX_STR_INFO_LENGHT-j,"        version: %s\n",version);
    j += sprintf_s(result+j,HGPU_MAX_STR_INFO_LENGHT-j,"     extensions: %s\n",extensions);
    j += sprintf_s(result+j,HGPU_MAX_STR_INFO_LENGHT-j,"max alloc. size: %lu\n",HGPU_GPU_device_get_info_uint(device,CL_DEVICE_MAX_MEM_ALLOC_SIZE));
    j += sprintf_s(result+j,HGPU_MAX_STR_INFO_LENGHT-j,"          index: [%u] [%u]\n",HGPU_GPU_device_get_index_of(device),HGPU_GPU_device_get_index_of_on_platform(device));
    free(extensions);
    free(version);
    free(profile);
    free(vendor);
    free(name);
    return result;
}

// get device's OpenCL version (min of DEVICE and DRIVER version)
HGPU_GPU_version
HGPU_GPU_device_get_version(cl_device_id device){
    HGPU_GPU_version version, version2;
    char* version_str  = HGPU_GPU_device_get_info_str(device,CL_DEVICE_VERSION);
    version = HGPU_GPU_version_get(version_str);
    if (version.minor>=1) {
        char* version_str2 = HGPU_GPU_device_get_info_str(device,CL_DEVICE_OPENCL_C_VERSION); // CL_DEVICE_OPENCL_C_VERSION option only in OpenCL 1.1
        version2 = HGPU_GPU_version_get(version_str2);
        version = HGPU_GPU_version_min(version,version2);
        free(version_str2);
    }
    free(version_str);
    return version;
}

// get vendor of OpenCL device
HGPU_GPU_vendor
HGPU_GPU_device_get_vendor(cl_device_id device){
    size_t parameter_length;
    HGPU_GPU_error_message(clGetDeviceInfo(device,CL_DEVICE_VENDOR,0,NULL,&parameter_length),"clGetDeviceInfo failed");
    char* result_str = (char*) calloc(parameter_length+1,sizeof(char));
    HGPU_GPU_error_message(clGetDeviceInfo(device,CL_DEVICE_VENDOR,(parameter_length+1),result_str,NULL),"clGetDeviceInfo failed");
    HGPU_GPU_vendor vendor = HGPU_convert_vendor_from_str(result_str);
    free(result_str);
    return vendor;
}

// get first available device
cl_device_id
HGPU_GPU_device_get_first(HGPU_GPU_devices* devices){
    if ((!devices) || (!HGPU_GPU_devices_get_number(devices))) HGPU_GPU_error_message(HGPU_ERROR_NO_DEVICE,"there is no any desired OpenCL device");
    return devices->devices[0];
}

// get best available device
cl_device_id
HGPU_GPU_device_get_best(HGPU_GPU_devices* devices){
    if ((!devices) || (!HGPU_GPU_devices_get_number(devices))) HGPU_GPU_error_message(HGPU_ERROR_NO_DEVICE,"there is no any desired OpenCL device");
    HGPU_GPU_devices*  devs = HGPU_GPU_devices_clone(devices);
    HGPU_GPU_devices_sort(&devs);
    cl_device_id result = devs->devices[0];
    return result;
}

// get next available device
cl_device_id
HGPU_GPU_device_get_next(HGPU_GPU_devices* devices,cl_device_id device){
    unsigned int number_of_devices = HGPU_GPU_devices_get_number(devices);
    if (!number_of_devices) HGPU_GPU_error_message(HGPU_ERROR_NO_DEVICE,"there is no any desired OpenCL device");
    unsigned int device_last_index = number_of_devices;
    for (unsigned int i=0;i<number_of_devices;i++){
        if (devices->devices[i]==device){
            device_last_index = i;
            i = number_of_devices;
        }
    }
    if (device_last_index>=(number_of_devices-1)) HGPU_GPU_error_message(HGPU_ERROR_NO_DEVICE,"there is no any desired OpenCL device");
    return devices->devices[device_last_index+1];
}

// check if OpenCL supports double precision
bool
HGPU_GPU_device_check_double_precision(cl_device_id device){
    bool result = false;
    char device_output[HGPU_MAX_STR_INFO_LENGHT];
    HGPU_GPU_error_message(clGetDeviceInfo(device,CL_DEVICE_EXTENSIONS,HGPU_MAX_STR_INFO_LENGHT,&device_output,NULL),"clGetDeviceInfo failed");
    HGPU_string_to_lowercase(device_output);
    if (strstr(device_output,"cl_khr_fp64")) result = true;
    return result;
}

// automatically select OpenCL devices according to (HGPU_parameter) parameters
HGPU_GPU_devices*
HGPU_GPU_devices_select_auto(HGPU_parameter** parameters){
    HGPU_GPU_platforms* platforms = NULL;
    HGPU_GPU_devices*   devices   = NULL;
    HGPU_parameter* parameter_device      = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_DEVICE);
    HGPU_parameter* parameter_device_type = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_DEVICE_TYPE);
    HGPU_parameter* parameter_platform    = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PLATFORM);

    if (parameter_platform) {
        unsigned int platform_id = parameter_platform->value_integer;
        if ((!platform_id) && (strlen(parameter_platform->value_text)>1)){
            // get platforms by vendor
            platforms = HGPU_GPU_platforms_get();
            HGPU_GPU_vendor platform_vendor = HGPU_convert_vendor_from_str(parameter_platform->value_text);
            HGPU_GPU_platforms_select_by_vendor(&platforms,platform_vendor);
        } else {
            platforms = HGPU_GPU_platforms_new(1);
            platforms[0].platforms[0] = HGPU_GPU_platform_get_by_index(platform_id);
        }
        devices = HGPU_GPU_devices_get_on_platforms(platforms);
    }

    if (parameter_device) {
        unsigned int device_id = parameter_device->value_integer;
        if ((!device_id) && (strlen(parameter_device->value_text)>1)){
            HGPU_GPU_vendor device_vendor = HGPU_convert_vendor_from_str(parameter_device->value_text);
            // get devices by vendor
            if (parameter_platform)
                HGPU_GPU_devices_select_by_vendor(&devices,device_vendor);
            else
                devices = HGPU_GPU_devices_get_by_vendor(device_vendor);
        } else {
            devices = HGPU_GPU_devices_new(1);
            if (parameter_platform)
                devices[0].devices[0] = HGPU_GPU_device_get_by_index_on_platform(platforms[0].platforms[0],device_id);
            else
                devices[0].devices[0] = HGPU_GPU_device_get_by_index(device_id);
        }
    }

    if ((!parameter_platform) && (!parameter_device))
        devices = HGPU_GPU_devices_get();

    if (parameter_device_type) {
        if (!strcmp(parameter_device_type->value_text,"GPU")) HGPU_GPU_devices_select_by_type(&devices,CL_DEVICE_TYPE_GPU);
        if (!strcmp(parameter_device_type->value_text,"CPU")) HGPU_GPU_devices_select_by_type(&devices,CL_DEVICE_TYPE_CPU);
        if (!strcmp(parameter_device_type->value_text,"ACCELERATOR")) HGPU_GPU_devices_select_by_type(&devices,CL_DEVICE_TYPE_ACCELERATOR);
    }

    HGPU_GPU_devices_sort(&devices);

    return devices;
}

// get rating of OpenCL device
unsigned int
HGPU_GPU_device_get_rating(cl_device_id device){
    //+++ GPU                  - 1000 points
    //+++ Double precision     - 100 points
    //+++ OpenCL major version - 10 points
    //+++ OpenCL minor version - 1 point
    unsigned int result = 0;
    if (device){
        cl_device_type device_type;
        HGPU_GPU_error_message(clGetDeviceInfo(device,CL_DEVICE_TYPE,sizeof(device_type),&device_type,NULL),"clGetDeviceInfo failed");
        if (device_type==CL_DEVICE_TYPE_GPU) result += 1000;
        if (HGPU_GPU_device_check_double_precision(device)) result += 100;
        HGPU_GPU_version version = HGPU_GPU_device_get_version(device);
            result += version.major * 10;
            result += version.minor;
    }
    return result;
}

// sort OpenCL devices by characteristics
void
HGPU_GPU_devices_sort(HGPU_GPU_devices** devices){
    // 1. GPU which supports double precision with highest version of OpenCL
    // 2. GPU with highest version of OpenCL
    // 3. device which supports double precision with highest version of OpenCL
    // 4. any other OpenCL device
    if (!devices) return;
    unsigned int number_of_devices = HGPU_GPU_devices_get_number(*devices);
    if (number_of_devices>0){
        unsigned int* rating = (unsigned int*) calloc(number_of_devices,sizeof(unsigned int));
        for (unsigned int i=0;i<number_of_devices;i++) rating[i]=HGPU_GPU_device_get_rating((*devices)->devices[i]);
        // sort by result
        for (unsigned int i=0;i<(number_of_devices-1);i++){
            unsigned int max_rating = rating[i];
            unsigned int max_index  = i;
            for (unsigned int j=(i+1);j<number_of_devices;j++){
                if (rating[j]>max_rating) {
                    max_index  = j;
                    max_rating = rating[j];
                }
            }
            if (max_index>i) {
                unsigned int temp_rating = rating[i];
                cl_device_id temp_device = (*devices)->devices[i];

                (*devices)->devices[i] = (*devices)->devices[max_index];
                rating[i] = rating[max_index];
                (*devices)->devices[max_index] = temp_device;
                rating[max_index] = temp_rating;
            }
        }
    }
}

// automatically select OpenCL device
cl_device_id
HGPU_GPU_device_select_auto(void){
    return HGPU_GPU_device_select_auto_with_parameters(NULL);
}

// automatically select OpenCL device from devices
cl_device_id
HGPU_GPU_device_select_auto_from_devices(HGPU_GPU_devices* devices){
    return HGPU_GPU_device_get_best(devices);
}

// automatically select next OpenCL device from devices
cl_device_id
HGPU_GPU_device_select_auto_from_devices_next(HGPU_GPU_devices* devices,cl_device_id device){
    HGPU_GPU_devices* results = HGPU_GPU_devices_clone(devices);
    HGPU_GPU_devices_sort(&results);
    cl_device_id result = NULL;
    bool device_found = false;
    cl_device_id temp_device;
    while ((results) && (HGPU_GPU_devices_get_number(results)) && (!device_found)) {
        temp_device = HGPU_GPU_device_get_first(results);
        if (temp_device==device) device_found = true;
        HGPU_GPU_devices_exclude_device(&results,temp_device);
    }
    if ((results) && (HGPU_GPU_devices_get_number(results))) result = HGPU_GPU_device_get_first(results);
    HGPU_GPU_devices_delete(&results);
    return result;
}


// automatically select next OpenCL device
cl_device_id
HGPU_GPU_device_select_auto_next(cl_device_id device){
    return HGPU_GPU_device_select_auto_with_parameters_next(device,NULL);
}


// automatically select OpenCL device with parameters
cl_device_id
HGPU_GPU_device_select_auto_with_parameters(HGPU_parameter** parameters){
    return HGPU_GPU_device_select_auto_with_parameters_next(NULL,parameters);
}

// automatically select next OpenCL device with parameters
cl_device_id
HGPU_GPU_device_select_auto_with_parameters_next(cl_device_id device,HGPU_parameter** parameters){
    HGPU_GPU_devices* devices = HGPU_GPU_devices_select_auto(parameters);
    cl_device_id result;
    if (!device)
        result = HGPU_GPU_device_get_best(devices);
    else
        result = HGPU_GPU_device_select_auto_from_devices_next(devices,device);
    HGPU_GPU_devices_delete(&devices);
    return result;
}

// get OpenCL device info
HGPU_GPU_device_info
HGPU_GPU_device_get_info(cl_device_id device){
    HGPU_GPU_device_info result;
        result.platform_vendor    = HGPU_GPU_platform_get_vendor(HGPU_GPU_device_get_platform(device));
        result.device_vendor      = HGPU_GPU_device_get_vendor(device);
        result.global_memory_size = HGPU_GPU_device_get_info_uint(device,CL_DEVICE_GLOBAL_MEM_SIZE);
        result.local_memory_size  = HGPU_GPU_device_get_info_uint(device,CL_DEVICE_LOCAL_MEM_SIZE);
        result.max_compute_units  = HGPU_GPU_device_get_info_uint(device,CL_DEVICE_MAX_COMPUTE_UNITS);
        result.max_constant_size  = HGPU_GPU_device_get_info_uint(device,CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE);
        result.max_memory_height  = (size_t) HGPU_GPU_device_get_info_uint(device,CL_DEVICE_IMAGE3D_MAX_HEIGHT);
        result.max_memory_width   = (size_t) HGPU_GPU_device_get_info_uint(device,CL_DEVICE_IMAGE3D_MAX_WIDTH);
        result.max_workgroup_size = HGPU_GPU_device_get_info_uint(device,CL_DEVICE_MAX_WORK_GROUP_SIZE);
        result.memory_align_factor= HGPU_GPU_device_get_info_uint(device,CL_DEVICE_MAX_WORK_GROUP_SIZE);

        if (result.max_memory_width)  result.max_memory_width  = HGPU_GPU_MAX_MEMORY_WIDTH;
        if (result.max_memory_height) result.max_memory_height = HGPU_GPU_MAX_MEMORY_HEIGHT;

#if defined(HGPU_GPU_MAX_INTEL_WORKGROUP_SIZE)
        if (result.device_vendor == HGPU_GPU_vendor_intel) result.max_workgroup_size = HGPU_GPU_MAX_INTEL_WORKGROUP_SIZE;
#endif

    return result;
}

// print short info on OpenCL device
void
HGPU_GPU_device_print_info(cl_device_id device){
    char* device_info = HGPU_GPU_device_get_info_short(device);
    printf("%s",device_info);
    free(device_info);
}

// print OpenCL device's name
void
HGPU_GPU_device_print_name(cl_device_id device){
    char* device_name = HGPU_GPU_device_get_name(device);
    printf("%s",device_name);
    free(device_name);
}

// get max memory width
unsigned int
HGPU_GPU_device_get_max_memory_width(cl_device_id device){
    unsigned int result = (unsigned int) HGPU_GPU_device_get_info_uint(device,CL_DEVICE_IMAGE3D_MAX_WIDTH);
    if (!result) result = 8192;
    return result;
}

// get max allocation memory
unsigned long int
HGPU_GPU_device_get_max_allocation_memory(cl_device_id device){
    unsigned long int result = HGPU_GPU_device_get_info_uint(device,CL_DEVICE_MAX_MEM_ALLOC_SIZE);
    return result;
}
