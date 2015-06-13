/******************************************************************************
 * @file     hgpucl_context.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0
 *
 * @brief    [HGPU library]
 *           Interface for OpenCL AMD APP & nVidia SDK environment
 *           context submodule
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

#include "hgpucl_context.h"

// create new context
HGPU_GPU_context*
HGPU_GPU_context_new(cl_device_id device,HGPU_GPU_debug debug_flags){
    if (!device) {
        HGPU_error_note(CL_DEVICE_NOT_FOUND,"device not found");
        return NULL;
    }

    cl_int GPU_error;
    cl_command_queue_properties profiling_properties = 0;

	// create context
    HGPU_GPU_context* context = (HGPU_GPU_context*) calloc(1,sizeof(HGPU_GPU_context));

    context->number_of_programs = 0;
    context->device      = device;
    context->debug_flags = debug_flags;
    context->device_info = HGPU_GPU_device_get_info(context->device);
	context->context     = clCreateContext(NULL,1,&device,NULL, NULL, &GPU_error);
    HGPU_GPU_error_message(GPU_error,"clCreateContext failed");
    context->program     = NULL;
    context->kernel      = NULL;
    context->buffer      = NULL;

    if (context->debug_flags.profiling) profiling_properties = (CL_QUEUE_PROFILING_ENABLE); // enable profiling for debuging
	context->queue = clCreateCommandQueue(context->context,context->device,profiling_properties,&GPU_error);
    HGPU_GPU_error_message(GPU_error,"clCreateCommandQueue failed");

    if (context->debug_flags.rebuild_binaries)
        HGPU_io_inf_file_delete_all();

    return context;
}

// delete context
void
HGPU_GPU_context_delete(HGPU_GPU_context** context){
    bool flag = (*context)->debug_flags.wait_for_keypress;
    if (!(*context)->queue)   HGPU_GPU_error_message(clReleaseCommandQueue((*context)->queue),"clReleaseCommandQueue failed");
    if (!(*context)->context) HGPU_GPU_error_message(clReleaseContext((*context)->context),"clReleaseContext failed");
    free((*context)->program);
    HGPU_GPU_kernels_delete(&((*context)->kernel));
    HGPU_GPU_buffers_delete(&((*context)->buffer));
    free(*context);
    (*context) = NULL;

    // output elapsed time
    printf("Elapsed time: %f seconds\n",HGPU_timer_get_from_start());

    // wait for keypress if required
    if (flag) {printf("\nPress any key to exit...\n"); _getch();}
}

// get context
cl_context
HGPU_GPU_context_get(HGPU_GPU_context* context){
    cl_context result = NULL;
    if (context) result = context->context;
    return result;
}

// automatically select best context without parameters
HGPU_GPU_context*
HGPU_GPU_context_autoselect(void){
    return HGPU_GPU_context_select_auto(NULL);
}

// automatically select nax available context without parameters
HGPU_GPU_context*
HGPU_GPU_context_autoselect_next(HGPU_GPU_context* context){
    return HGPU_GPU_context_select_auto_next(context,NULL);
}

// automatically select best context according to parameters
HGPU_GPU_context*
HGPU_GPU_context_select_auto(HGPU_parameter** parameters){
    return HGPU_GPU_context_select_auto_next(NULL,parameters);
}

// automatically select next available context according to parameters
HGPU_GPU_context*
HGPU_GPU_context_select_auto_next(HGPU_GPU_context* context,HGPU_parameter** parameters){
    HGPU_GPU_context* result = NULL;
    HGPU_parameters_path_setup(parameters);
    cl_device_id device;
    if (context) {
        cl_device_id device_last = context->device;
        device = HGPU_GPU_device_select_auto_with_parameters_next(device_last,parameters);
    } else {
        device = HGPU_GPU_device_select_auto_with_parameters(parameters);
    }
    HGPU_GPU_debug debug_flags = HGPU_GPU_debug_get_flags(parameters);
    result = HGPU_GPU_context_new(device,debug_flags);
    HGPU_GPU_context_check_flags(result);
    return result;
}

// setup debug_flags with parameters
HGPU_GPU_debug
HGPU_GPU_debug_get_flags(HGPU_parameter** parameters){
    HGPU_GPU_debug debug_flags = HGPU_GPU_debug_default;
    if (parameters){
        HGPU_parameter* parameter_max_workgroup_size = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_MAX_WORKGROUP_SIZE);
        HGPU_parameter* parameter_wait_for_keypress  = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_WAIT_FOR_KEYPRESS);
        HGPU_parameter* parameter_showstage          = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_SHOW_STAGE);
        HGPU_parameter* parameter_profiling          = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_PROFILING);
        HGPU_parameter* parameter_brief_report       = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_BRIEF_REPORT);
        HGPU_parameter* parameter_rebuid_binaries    = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_REBUILD_BINARIES);
        HGPU_parameter* parameter_warning_error      = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_WARNING_ERROR);
        HGPU_parameter* parameter_no_cache           = HGPU_parameters_get_by_name(parameters,(char*) HGPU_PARAMETER_NO_CACHE);

        if (parameter_max_workgroup_size) debug_flags.max_workgroup_size = parameter_max_workgroup_size->value_integer;
        if (parameter_wait_for_keypress)  debug_flags.wait_for_keypress  = true;
        if (parameter_showstage)          debug_flags.show_stage         = true;
        if (parameter_profiling)          debug_flags.profiling          = true;
        if (parameter_brief_report)       debug_flags.brief_report       = true;
        if (parameter_rebuid_binaries)    debug_flags.rebuild_binaries   = true;
        if (parameter_warning_error)      debug_flags.warning_error      = true;
        if (parameter_no_cache)           debug_flags.no_cache           = true;
    }
    return debug_flags;
}

// check flags according to context
void
HGPU_GPU_context_check_flags(HGPU_GPU_context* context){
    if (!context) return;
    if (context->debug_flags.no_cache) printf("WARNING: [%s] flag is set!\n",HGPU_PARAMETER_NO_CACHE);
}

// kernel create and initialization
unsigned int
HGPU_GPU_context_kernel_init(HGPU_GPU_context* context,cl_program program,const char* kernel_name,
                            unsigned int work_dimensions,const size_t* global_size,const size_t* local_size){
    unsigned int result = HGPU_GPU_MAX_KERNELS;
    if (!context) return result;
    result = HGPU_GPU_kernel_init(&(context->kernel),program,kernel_name,work_dimensions,global_size,local_size);
    return result;
}

// buffer create and initialization
unsigned int
HGPU_GPU_context_buffer_init(HGPU_GPU_context* context,void* host_ptr_to_buffer,
                             HGPU_GPU_buffer_types buffer_type,size_t buffer_elements,size_t sizeof_element){
    unsigned int result = HGPU_GPU_MAX_BUFFERS;
    if (!context) return result;
    result = HGPU_GPU_buffer_init(&(context->buffer),context->context,host_ptr_to_buffer,buffer_type,buffer_elements,sizeof_element);

    return result;
}

//set literal name for buffer
void
HGPU_GPU_context_buffer_set_name(HGPU_GPU_context* context,unsigned int buffer_id,const char* buffer_name){
    if ((!context) || (!buffer_name) || (buffer_id>=HGPU_GPU_MAX_BUFFERS)) return;
    HGPU_GPU_buffer* buffer = HGPU_GPU_buffer_get_by_index(context->buffer,buffer_id);
    HGPU_GPU_buffer_set_name(buffer,buffer_name);
}

// release buffer
void
HGPU_GPU_context_buffer_release(HGPU_GPU_context* context,unsigned int buffer_id){
    if (buffer_id<HGPU_GPU_MAX_BUFFERS) {
        HGPU_GPU_buffer* buffer = HGPU_GPU_buffer_get_by_index(context->buffer,buffer_id);
        HGPU_GPU_buffer_release(buffer);
    }
}

// buffer size align
size_t
HGPU_GPU_context_buffer_size_align(HGPU_GPU_context* context,size_t buffer_size){
    size_t align_factor = HGPU_GPU_device_get_info_uint(context->device,CL_DEVICE_MAX_WORK_GROUP_SIZE);
    size_t result = HGPU_GPU_buffer_size_align_step(buffer_size,align_factor);
    return result;
}

// bind buffer to kernel
unsigned int
HGPU_GPU_context_kernel_bind_buffer(HGPU_GPU_context* context,unsigned int kernel_id,unsigned int buffer_id){
    unsigned int result = HGPU_GPU_MAX_ARGUMENTS;
    if ((!context) || (kernel_id>=HGPU_GPU_MAX_KERNELS) || (buffer_id>=HGPU_GPU_MAX_BUFFERS)) return result;
    HGPU_GPU_kernel* kernel = HGPU_GPU_kernel_get_by_index(context->kernel,kernel_id);
    if (kernel) {
        unsigned int argument_id = kernel->argument_id;
        result = HGPU_GPU_context_kernel_bind_buffer_by_argument_id(context,kernel_id,buffer_id,argument_id);
        if (result<HGPU_GPU_MAX_ARGUMENTS) {
            kernel->argument_id++;
            result = kernel->argument_id;
        }
    }
    return result;
}

// bind buffer to kernel by argument_id
unsigned int
HGPU_GPU_context_kernel_bind_buffer_by_argument_id(HGPU_GPU_context* context,unsigned int kernel_id,unsigned int buffer_id,unsigned int argument_id){
    unsigned int result = HGPU_GPU_MAX_ARGUMENTS;
    if ((!context) || (kernel_id>=HGPU_GPU_MAX_KERNELS) || (buffer_id>=HGPU_GPU_MAX_BUFFERS) || (argument_id>=HGPU_GPU_MAX_ARGUMENTS)) return result;
    HGPU_GPU_kernel* kernel = HGPU_GPU_kernel_get_by_index(context->kernel,kernel_id);
    HGPU_GPU_buffer* buffer = HGPU_GPU_buffer_get_by_index(context->buffer,buffer_id);
    if ((kernel) && (buffer)) {
        HGPU_GPU_kernel_bind_argument(kernel->kernel,argument_id,sizeof(cl_mem),&(buffer->buffer));
        result = argument_id;
    }
    return result;
}

// bind constant to kernel
unsigned int
HGPU_GPU_context_kernel_bind_constant(HGPU_GPU_context* context,unsigned int kernel_id,void* constant_ptr,size_t sizeof_constant){
    unsigned int result = HGPU_GPU_MAX_ARGUMENTS;
    if ((!context) || (kernel_id>=HGPU_GPU_MAX_KERNELS)) return result;
    HGPU_GPU_kernel* kernel = HGPU_GPU_kernel_get_by_index(context->kernel,kernel_id);
    if (kernel) {
        unsigned int argument_id = kernel->argument_id;
        result = HGPU_GPU_context_kernel_bind_constant_by_argument_id(context,kernel_id,constant_ptr,sizeof_constant,argument_id);
        kernel->argument_id++;
        result = kernel->argument_id;
    }
    return result;
}

// bind constant to kernel by argument_id
unsigned int
HGPU_GPU_context_kernel_bind_constant_by_argument_id(HGPU_GPU_context* context,unsigned int kernel_id,void* constant_ptr,
                    size_t sizeof_constant,unsigned int argument_id){
    unsigned int result = HGPU_GPU_MAX_ARGUMENTS;
    if ((!context) || (kernel_id>=HGPU_GPU_MAX_KERNELS) || (argument_id>=HGPU_GPU_MAX_ARGUMENTS)) return result;
    HGPU_GPU_kernel* kernel = HGPU_GPU_kernel_get_by_index(context->kernel,kernel_id);
    if (kernel) {
        HGPU_GPU_kernel_bind_argument(kernel->kernel,argument_id,sizeof_constant,constant_ptr);
        result = argument_id;
    }
    return result;
}

// run kernel
void
HGPU_GPU_context_kernel_run(HGPU_GPU_context* context,unsigned int kernel_id){
    if ((!context) || (kernel_id>=HGPU_GPU_MAX_KERNELS)) return;
    HGPU_GPU_kernel* kernel = HGPU_GPU_kernel_get_by_index(context->kernel,kernel_id);
    if (kernel) {
        size_t max_wg_size = context->debug_flags.max_workgroup_size;
        if (max_wg_size) HGPU_GPU_kernel_limit_local_workgroup_size(kernel,context->device,max_wg_size);
        if (context->debug_flags.profiling)
            HGPU_GPU_kernel_run_with_profiling(kernel,context->queue);
        else
            HGPU_GPU_kernel_run_without_profiling(kernel,context->queue);
    }
}

// run kernel (asynchronous)
void
HGPU_GPU_context_kernel_run_async(HGPU_GPU_context* context,unsigned int kernel_id){
    if ((!context) || (kernel_id>=HGPU_GPU_MAX_KERNELS)) return;
    HGPU_GPU_kernel* kernel = HGPU_GPU_kernel_get_by_index(context->kernel,kernel_id);
    if (kernel) {
        size_t max_wg_size = context->debug_flags.max_workgroup_size;
        if (max_wg_size) HGPU_GPU_kernel_limit_local_workgroup_size(kernel,context->device,max_wg_size);
        HGPU_GPU_kernel_run_async(kernel,context->queue);
    }
}

// limit maximum workgroup size for kernel
void
HGPU_GPU_context_kernel_limit_max_workgroup_size(HGPU_GPU_context* context,unsigned int kernel_id){
    if ((!context) || (kernel_id>=HGPU_GPU_MAX_KERNELS)) return;
    HGPU_GPU_kernel* kernel = HGPU_GPU_kernel_get_by_index(context->kernel,kernel_id);
    HGPU_GPU_kernel_limit_local_workgroup_size(kernel,context->device,context->debug_flags.max_workgroup_size);
}

// get kernel name by index
char*
HGPU_GPU_context_kernel_get_name(HGPU_GPU_context* context,unsigned int kernel_id){
    char* result = NULL;
    if ((!context) || (kernel_id>=HGPU_GPU_MAX_KERNELS)) return result;
    HGPU_GPU_kernel* kernel = HGPU_GPU_kernel_get_by_index(context->kernel,kernel_id);
    result = HGPU_GPU_kernel_get_name(kernel);
    return result;
}

// get number of kernel's arguments by index
unsigned int
HGPU_GPU_context_kernel_get_args_number(HGPU_GPU_context* context,unsigned int kernel_id){
    unsigned int result = 0;
    if ((!context) || (kernel_id>=HGPU_GPU_MAX_KERNELS)) return result;
    HGPU_GPU_kernel* kernel = HGPU_GPU_kernel_get_by_index(context->kernel,kernel_id);
    result = HGPU_GPU_kernel_get_args_number(kernel);
    return result;
}

// get kernel_id by kernel name
unsigned int
HGPU_GPU_context_kernel_get_id(HGPU_GPU_context* context,const char* kernel_name){
    unsigned int result = HGPU_GPU_MAX_KERNELS;
    if ((!context) || (!kernel_name)) {
        HGPU_GPU_error_note(HGPU_ERROR_BAD_KERNEL,"bad kernel name");
        return result;
    }
    unsigned int number_of_kernels = HGPU_GPU_kernels_get_number(context->kernel);
    for (unsigned int i=0;i<number_of_kernels;i++){
        char* t_kernel_name = HGPU_GPU_kernel_get_name(context->kernel[i]);
        if ((t_kernel_name) && (!strcmp(kernel_name,t_kernel_name))) result = i;
        free(t_kernel_name);
    }
    return result;
}

// buffer map
void*
HGPU_GPU_context_buffer_map(HGPU_GPU_context* context,unsigned int buffer_id){
    void* result = NULL;
    if ((!context) || (buffer_id>=HGPU_GPU_MAX_BUFFERS)) return result;
    HGPU_GPU_buffer* buffer = HGPU_GPU_buffer_get_by_index(context->buffer,buffer_id);
    if (context->debug_flags.profiling)
        result = HGPU_GPU_buffer_map_with_profiling(buffer,context->queue);
    else
        result = HGPU_GPU_buffer_map(buffer,context->queue);
    return result;
}

// get buffer mapped ptr
void*
HGPU_GPU_context_buffer_get_mapped(HGPU_GPU_context* context,unsigned int buffer_id){
    void* result = NULL;
    if ((!context) || (buffer_id>=HGPU_GPU_MAX_BUFFERS)) return result;
    HGPU_GPU_buffer* buffer = HGPU_GPU_buffer_get_by_index(context->buffer,buffer_id);
    if (buffer)
        if (!buffer->mapped_ptr) result = HGPU_GPU_context_buffer_map(context,buffer_id);
        result = buffer->mapped_ptr;
    return result;

}

// buffer map (asynchronous)
void*
HGPU_GPU_context_buffer_map_async(HGPU_GPU_context* context,unsigned int buffer_id){
    void* result = NULL;
    if ((!context) || (buffer_id>=HGPU_GPU_MAX_BUFFERS)) return result;
    HGPU_GPU_buffer* buffer = HGPU_GPU_buffer_get_by_index(context->buffer,buffer_id);
    result = HGPU_GPU_buffer_map_async(buffer,context->queue);
    return result;
}

// buffer unmap
void
HGPU_GPU_context_buffer_unmap(HGPU_GPU_context* context,unsigned int buffer_id,void* data_ptr){
    if ((!context) || (buffer_id>=HGPU_GPU_MAX_BUFFERS)) return;
    HGPU_GPU_buffer* buffer = HGPU_GPU_buffer_get_by_index(context->buffer,buffer_id);
    if (context->debug_flags.profiling)
        HGPU_GPU_buffer_unmap_with_profiling(buffer,context->queue,data_ptr);
    else
        HGPU_GPU_buffer_unmap(buffer,context->queue,data_ptr);
}

// buffer unmap (asynchronous)
void
HGPU_GPU_context_buffer_unmap_async(HGPU_GPU_context* context,unsigned int buffer_id,void* data_ptr){
    if ((!context) || (buffer_id>=HGPU_GPU_MAX_BUFFERS)) return;
    HGPU_GPU_buffer* buffer = HGPU_GPU_buffer_get_by_index(context->buffer,buffer_id);
    HGPU_GPU_buffer_unmap_async(buffer,context->queue,data_ptr);
}

// buffer - get memory object
cl_mem
HGPU_GPU_context_buffer_get(HGPU_GPU_context* context,unsigned int buffer_id){
    cl_mem result = NULL;
    if ((!context) || (buffer_id>=HGPU_GPU_MAX_BUFFERS)) return result;
    HGPU_GPU_buffer* buffer = HGPU_GPU_buffer_get_by_index(context->buffer,buffer_id);
    result = buffer->buffer;
    return result;
}


// print memory utilized
void
HGPU_GPU_context_print_memory_utilized(HGPU_GPU_context* context){
    cl_ulong result = 0;    // in bytes
    if (!context) return;
    unsigned int number_of_buffers = HGPU_GPU_buffers_get_number(context->buffer);
    for (unsigned int i=0;i<number_of_buffers;i++){
        if (context->buffer[i])
            result += (context->buffer[i]->size) * (context->buffer[i]->sizeof_element);
    }
    printf("Used GPU memory, MB: %f\n",HGPU_convert_B_to_MB((double) result));
}

// print stage
void
HGPU_GPU_context_print_stage(HGPU_GPU_context* context,const char* stage_message){
    if (context->debug_flags.show_stage) printf(">>> Runtime stage >>> %s\n",stage_message);
}

// print detailed memory report
void
HGPU_GPU_context_print_detailed_report(HGPU_GPU_context* context){
    HGPU_timer_deviation elapsed_time       = HGPU_timer_deviation_default;
    HGPU_timer_deviation elapsed_time_read  = HGPU_timer_deviation_default;
    HGPU_timer_deviation elapsed_time_write = HGPU_timer_deviation_default;
    printf("--------------------------------------------------------\n");
    unsigned int number_of_kernels = HGPU_GPU_kernels_get_number(context->kernel);
    for(unsigned int i=0;i<number_of_kernels;i++){
        HGPU_GPU_kernel* kernel = HGPU_GPU_kernel_get_by_index(context->kernel,i);
        if (kernel) {
            char* kernel_name = HGPU_GPU_context_kernel_get_name(context,i);
            elapsed_time = HGPU_GPU_kernel_get_time_execution(kernel);

            if (context->debug_flags.brief_report)
                printf("[%2u] kernel \"%s\" (N=%llu): %f ms\n",i,kernel_name,kernel->kernel_number_of_starts,elapsed_time.mean*1.e-6);
            else
                printf("[%2u] kernel \"%s\" executed %llu times:\n\t execution time: %f (+/-%f) ms\n",i,kernel_name,
                        kernel->kernel_number_of_starts,elapsed_time.mean*1.E-6f,elapsed_time.deviation*1.e-6);
            free(kernel_name);
        }
    }
    printf("--------------------------------------------------------\n");
    unsigned int number_of_buffers = HGPU_GPU_buffers_get_number(context->buffer);
    for(unsigned int i=0;i<number_of_buffers;i++){
        HGPU_GPU_buffer* buffer = HGPU_GPU_buffer_get_by_index(context->buffer,i);
        if (buffer) {
            elapsed_time_write = HGPU_GPU_buffer_get_time_write(buffer);
            elapsed_time_read  = HGPU_GPU_buffer_get_time_read(buffer);
            double buffer_size = ((double) buffer->size * buffer->sizeof_element);
            printf("[%2u] ",i);
            if (buffer->name) printf("\"%s\" ",buffer->name);
            printf("%f Mbytes:\n",HGPU_convert_B_to_MB(buffer_size));
            if (buffer->buffer_write_number_of){
                if(context->debug_flags.brief_report) {
                    printf("\t\tH>D (N=%u): %f Mbytes/sec\n",buffer->buffer_write_number_of,HGPU_convert_B_to_MBS(buffer_size,elapsed_time_write.mean));
                } else {
                    printf("\t host to device %u times\n\t\t elapsed time: %f (+/-%f) ms (%f Mbytes/sec)\n",
                        buffer->buffer_write_number_of,elapsed_time_write.mean*1.e-6,elapsed_time_write.deviation*1.e-6,HGPU_convert_B_to_MBS(buffer_size,elapsed_time_write.mean));
                }
            }
            if (buffer->buffer_read_number_of){
                if(context->debug_flags.brief_report) {
                    printf("\t\tD>H (N=%u): %f Mbytes/sec\n",buffer->buffer_read_number_of,HGPU_convert_B_to_MBS(buffer_size,elapsed_time_read.mean));
                } else {
                    printf("\t device to host %u times:\n\t\t elapsed time: %f (+/-%f) ms (%f Mbytes/sec)\n",
                        buffer->buffer_read_number_of,elapsed_time_read.mean*1.e-6,elapsed_time_read.deviation*1.e-6,HGPU_convert_B_to_MBS(buffer_size,elapsed_time_read.mean));
                }
	        }
        }
    }
    printf("--------------------------------------------------------\n");
}

// print used hardware
void
HGPU_GPU_context_print_used_hardware(HGPU_GPU_context* context){
    if (!context) return;
    cl_platform_id platform = HGPU_GPU_device_get_platform(context->device);
    printf("********************************************************\n");
    printf("Platform: ");
    HGPU_GPU_platform_print_name(platform);
    printf("\nDevice:   ");
    HGPU_GPU_device_print_name(context->device);
    printf("\n");
    printf("********************************************************\n");
}

// wait for queue finish
void
HGPU_GPU_context_queue_wait_for_finish(HGPU_GPU_context* context){
    HGPU_GPU_error_message(clFinish(context->queue),"clFinish failed");
}

// wait for event
void
HGPU_GPU_context_event_wait_for(cl_event* context_event){
    HGPU_GPU_error_message(clWaitForEvents(1,context_event),"clWaitForEvents failed");
}
