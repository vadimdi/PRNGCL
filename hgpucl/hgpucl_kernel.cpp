/******************************************************************************
 * @file     hgpucl_kernel.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           Interface for OpenCL AMD APP & nVidia SDK environment
 *           kernel submodule
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

#include "hgpucl_kernel.h"

// add kernel to array of kernels
void
HGPU_GPU_kernels_add(HGPU_GPU_kernel*** kernels,HGPU_GPU_kernel* kernel){
    if ((!kernels) || (!kernel)) return;
    unsigned int number_of_kernels = HGPU_GPU_kernels_get_number(*kernels);
    HGPU_GPU_kernel** kernels_new  = (HGPU_GPU_kernel**) realloc(*kernels,(number_of_kernels+2)*sizeof(HGPU_GPU_kernel*));
    if (!kernels_new) HGPU_error(HGPU_ERROR_NO_MEMORY);
    kernels_new[number_of_kernels]   = kernel;
    kernels_new[number_of_kernels+1] = NULL;
    (*kernels) = kernels_new;
}

// delete array of kernels
void
HGPU_GPU_kernels_delete(HGPU_GPU_kernel*** kernels){
    if ((!kernels) || (!(*kernels))) return;
    unsigned int index = 0;
    while (((*kernels)[index]) && (index<HGPU_GPU_MAX_KERNELS)) HGPU_GPU_kernel_delete(&((*kernels)[index++]));
    free(*kernels);
    (*kernels) = NULL;
}

// get number of kernels in (HGPU_GPU_kernel) array
unsigned int
HGPU_GPU_kernels_get_number(HGPU_GPU_kernel** kernels){
    unsigned int result = 0;
    if ((!kernels) || (!(*kernels))) return 0;
    while ((result<HGPU_GPU_MAX_KERNELS) && (kernels[result])) result++;
    return result;
}

// create new empty kernel
HGPU_GPU_kernel*
HGPU_GPU_kernel_new_empty(void){
    HGPU_GPU_kernel* result = (HGPU_GPU_kernel*) calloc(1,sizeof(HGPU_GPU_kernel));
        result->kernel = 0;
        result->argument_id     = 0;
        result->work_dimensions = 0;
        result->global_size     = NULL;
        result->local_size      = NULL;
        result->kernel_number_of_starts     = 0;
        result->kernel_elapsed_time         = 0.0;
        result->kernel_elapsed_time_squared = 0.0;
    return result;
}

// delete kernel
void
HGPU_GPU_kernel_delete(HGPU_GPU_kernel** kernel){
    if ((!kernel) || (!(*kernel))) return;
    free((*kernel)->global_size);
    free((*kernel)->local_size);
    free(*kernel);
    (*kernel) = NULL;
}

// get index of kernel in (HGPU_GPU_kernel) array
unsigned int
HGPU_GPU_kernel_get_index(HGPU_GPU_kernel** kernels,HGPU_GPU_kernel* kernel){
    unsigned int result = 0;
    bool found = false;
    if ((!kernels) || (!kernel)) return HGPU_GPU_MAX_KERNELS;
    while ((result<HGPU_GPU_MAX_KERNELS) && (kernels[result])) {
        if (kernels[result]==kernel) {found=true; break;}
        result++;
    }
    if (!found) result = HGPU_GPU_MAX_KERNELS;
    return result;
}

// get kernel by index in (HGPU_GPU_kernel) array
HGPU_GPU_kernel*
HGPU_GPU_kernel_get_by_index(HGPU_GPU_kernel** kernels,unsigned int index){
    unsigned int number_of_kernels = HGPU_GPU_kernels_get_number(kernels);
    if ((!number_of_kernels) || (index>number_of_kernels)) return NULL;
    return kernels[index];
}

// get name of kernel
char*
HGPU_GPU_kernel_get_name(HGPU_GPU_kernel* kernel){
    char* result = NULL;
    size_t result_size = 0;
    HGPU_GPU_error_message(clGetKernelInfo(kernel->kernel,CL_KERNEL_FUNCTION_NAME,0,NULL,&result_size),"clGetKernelInfo failed!");
    result_size++;
    result = (char*) calloc(result_size,sizeof(char));
    HGPU_GPU_error_message(clGetKernelInfo(kernel->kernel,CL_KERNEL_FUNCTION_NAME,result_size,result,&result_size),"clGetKernelInfo failed!");
    return result;
}

// get number of kernel's arguments
unsigned int
HGPU_GPU_kernel_get_args_number(HGPU_GPU_kernel* kernel){
    unsigned int result = 0;
    HGPU_GPU_error_message(clGetKernelInfo(kernel->kernel,CL_KERNEL_NUM_ARGS,sizeof(result),&result,NULL),"clGetKernelInfo failed!");
    return result;
}

// get kernel execution time
HGPU_timer_deviation
HGPU_GPU_kernel_get_time_execution(HGPU_GPU_kernel* kernel){
    HGPU_timer_deviation result = HGPU_timer_deviation_default;
    if (kernel)
        result = HGPU_timer_deviation_get(kernel->kernel_elapsed_time,kernel->kernel_elapsed_time_squared,(double) kernel->kernel_number_of_starts);
    return result;
}

// kernel initialization
unsigned int
HGPU_GPU_kernel_init(HGPU_GPU_kernel*** kernels,cl_program program,const char* kernel_name,
                     unsigned int work_dimensions,const size_t* global_size,const size_t* local_size){
    if ((!kernels) || (!kernel_name) || (!program) || (!global_size) ||
        (work_dimensions>HGPU_GPU_MAX_WORK_DIMENSIONS) || (!work_dimensions)) return HGPU_GPU_MAX_KERNELS;
    HGPU_GPU_kernel* kernel = HGPU_GPU_kernel_new_empty();
    kernel->work_dimensions = work_dimensions;
    kernel->global_size = HGPU_string_size_t_clone(global_size,work_dimensions);
    kernel->local_size  = HGPU_string_size_t_clone(local_size, work_dimensions);

    // create kernel
    cl_kernel clkernel = HGPU_GPU_kernel_create(program,kernel_name);
    kernel->kernel = clkernel;

    HGPU_GPU_kernels_add(&(*kernels),kernel);
    unsigned int result = HGPU_GPU_kernel_get_index(*kernels,kernel);

    return result;    
}

// limit local workgroup size on some devices (for example, Intel)
void
HGPU_GPU_kernel_limit_local_workgroup_size(HGPU_GPU_kernel* kernel,cl_device_id device,size_t max_local_workgroup_size){
    if (!kernel){
        HGPU_GPU_error_note(HGPU_ERROR_BAD_KERNEL,"Trying to use null-kernel!");
        return;
    }
    size_t* local_workgroup_size = HGPU_GPU_kernel_get_local_worksize(kernel,device);
    if ((local_workgroup_size) && (max_local_workgroup_size)){
        for (unsigned int i=0;i<kernel->work_dimensions;i++)
            if (max_local_workgroup_size<local_workgroup_size[i]) local_workgroup_size[i] = max_local_workgroup_size;
        free(kernel->local_size);
        kernel->local_size = local_workgroup_size;
    }
}

// get kernel workgroup size
size_t*
HGPU_GPU_kernel_get_local_worksize(HGPU_GPU_kernel* kernel,cl_device_id device){
    size_t* result = NULL;
    if ((!kernel) || (!device)) return result;
    result = (size_t*) calloc(kernel->work_dimensions,sizeof(size_t));
    if (kernel->local_size)
        result = HGPU_string_size_t_clone(kernel->local_size,kernel->work_dimensions);
    else {
		size_t local_wg_size = 0;
        HGPU_GPU_error_message(clGetKernelWorkGroupInfo(kernel->kernel,device,CL_KERNEL_WORK_GROUP_SIZE,sizeof(size_t),&local_wg_size,NULL),"clGetKernelWorkGroupInfo failed");
		result[0] = local_wg_size;
	}
    return result;
}




// run kernel
void
HGPU_GPU_kernel_run(HGPU_GPU_kernel* kernel,cl_command_queue queue){
    HGPU_GPU_kernel_run_async(kernel,queue);
    HGPU_GPU_kernel_run_wait_for_finish(&kernel->kernel_event);
    HGPU_GPU_kernel_run_wait_for_queue_finish(queue);
}


// run kernel with profiling
void
HGPU_GPU_kernel_run_with_profiling(HGPU_GPU_kernel* kernel,cl_command_queue queue){
	HGPU_GPU_kernel_run(kernel,queue);
    HGPU_GPU_kernel_run_profile(kernel);
    HGPU_GPU_error_message(clReleaseEvent(kernel->kernel_event),"clReleaseEvent failed");
}

// run kernel without profiling
void
HGPU_GPU_kernel_run_without_profiling(HGPU_GPU_kernel* kernel,cl_command_queue queue){
	HGPU_GPU_kernel_run(kernel,queue);
    HGPU_GPU_error_message(clReleaseEvent(kernel->kernel_event),"clReleaseEvent failed");
}



// run kernel (asynchronous)
void
HGPU_GPU_kernel_run_async(HGPU_GPU_kernel* kernel,cl_command_queue queue){
    if (!kernel){
        HGPU_GPU_error_note(HGPU_ERROR_BAD_KERNEL,"Trying to use null-kernel!");
        return;
    }
    cl_event kernel_event = HGPU_GPU_kernel_start(kernel->kernel,queue,kernel->work_dimensions,kernel->global_size,kernel->local_size);
	kernel->kernel_event  = kernel_event;
    kernel->kernel_number_of_starts++;
}

// profile kernel run
void
HGPU_GPU_kernel_run_profile(HGPU_GPU_kernel* kernel){
    if ((kernel) && (kernel->kernel_event)){
        cl_ulong kernel_start  = 0;
        cl_ulong kernel_finish = 0;
        HGPU_GPU_kernel_run_wait_for_finish(&kernel->kernel_event);
        HGPU_GPU_error_message(clGetEventProfilingInfo(kernel->kernel_event,CL_PROFILING_COMMAND_START,sizeof(cl_ulong),&kernel_start, NULL),"clGetEventProfilingInfo failed");
        HGPU_GPU_error_message(clGetEventProfilingInfo(kernel->kernel_event,CL_PROFILING_COMMAND_END,  sizeof(cl_ulong),&kernel_finish,NULL),"clGetEventProfilingInfo failed");
        double elapsed_time = (double) (kernel_finish-kernel_start);
            kernel->kernel_elapsed_time          += elapsed_time;
            kernel->kernel_elapsed_time_squared  += elapsed_time * elapsed_time;
            kernel->kernel_start                  = kernel_start;
            kernel->kernel_finish                 = kernel_finish;
    }
}

// simple functions_______________________________________________________________________________________________

// create kernel
cl_kernel
HGPU_GPU_kernel_create(cl_program program,const char* kernel_name){
    cl_int GPU_error = CL_SUCCESS;
    cl_kernel kernel = 0;
    kernel = clCreateKernel(program,kernel_name, &GPU_error);
    HGPU_GPU_error_message(GPU_error,"clCreateKernel failed");
    return kernel;
}

// kernel bind argument
void
HGPU_GPU_kernel_bind_argument(cl_kernel kernel,unsigned int argument_id,size_t argument_size,const void* argument_ptr){
    cl_int GPU_error = CL_SUCCESS;
    GPU_error = clSetKernelArg(kernel,argument_id,argument_size,argument_ptr);
    HGPU_GPU_error_message(GPU_error,"clSetKernelArg failed");
}

// simple run of kernel
cl_event
HGPU_GPU_kernel_start(cl_kernel kernel,cl_command_queue queue,unsigned int work_dimensions,const size_t* global_size,const size_t* local_size){
    cl_int GPU_error = CL_SUCCESS;
    cl_event kernel_event  = 0;
    GPU_error = clEnqueueNDRangeKernel(queue,kernel,work_dimensions,NULL,global_size,local_size, 0, NULL, &kernel_event);
        HGPU_GPU_error_message(GPU_error,"clEnqueueNDRangeKernel failed");
    return kernel_event;
}

// wait for kernel finishing
void
HGPU_GPU_kernel_run_wait_for_finish(cl_event* kernel_event){
    if (kernel_event) {
        cl_uint GPU_error = CL_SUCCESS;
        cl_uint status = CL_RUNNING;
        do
            GPU_error = clGetEventInfo((*kernel_event),CL_EVENT_COMMAND_EXECUTION_STATUS,sizeof(cl_int),&status,NULL);
        while ((GPU_error==CL_SUCCESS) && (status!=CL_COMPLETE) && (status!=CL_QUEUED));

        HGPU_GPU_error_message(GPU_error,"clGetEventInfo failed");
        GPU_error = clWaitForEvents(1, kernel_event);
        if (GPU_error!=CL_SUCCESS)
            HGPU_GPU_error_message(GPU_error,"clWaitForEvents failed");
    }
}

// wait for kernel finishing
void
HGPU_GPU_kernel_run_wait_for_queue_finish(cl_command_queue queue){
    if (queue) {
        HGPU_GPU_error_message(clFinish(queue),"clFinish failed");
    }
}
