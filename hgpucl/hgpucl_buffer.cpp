/******************************************************************************
 * @file     hgpucl_buffer.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           Interface for OpenCL AMD APP & nVidia SDK environment
 *           buffer submodule
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

#include "hgpucl_buffer.h"

// add buffer to array of buffers
void
HGPU_GPU_buffers_add(HGPU_GPU_buffer*** buffers,HGPU_GPU_buffer* buffer){
    if ((!buffer) || (!buffer)) return;
    unsigned int number_of_buffers = HGPU_GPU_buffers_get_number(*buffers);
    HGPU_GPU_buffer** buffers_new  = (HGPU_GPU_buffer**) realloc(*buffers,(number_of_buffers+2)*sizeof(HGPU_GPU_buffer*));
    if (!buffers_new) HGPU_error(HGPU_ERROR_NO_MEMORY);
    buffers_new[number_of_buffers]   = buffer;
    buffers_new[number_of_buffers+1] = NULL;
    (*buffers) = buffers_new;
}

// delete array of buffers
void
HGPU_GPU_buffers_delete(HGPU_GPU_buffer*** buffers){
    if ((!buffers) || (!(*buffers))) return;
    unsigned int index = 0;
    while (((*buffers)[index]) && (index<HGPU_GPU_MAX_BUFFERS)) HGPU_GPU_buffer_delete(&((*buffers)[index++]));
    free(*buffers);
    (*buffers) = NULL;
}

// get number of buffers in (HGPU_GPU_buffer) array
unsigned int
HGPU_GPU_buffers_get_number(HGPU_GPU_buffer** buffers){
    unsigned int result = 0;
    if ((!buffers) || (!(*buffers))) return 0;
    while ((result<HGPU_GPU_MAX_BUFFERS) && (buffers[result])) result++;
    return result;
}



 // create new empty buffer
HGPU_GPU_buffer*
HGPU_GPU_buffer_new_empty(void){
    HGPU_GPU_buffer* result    = (HGPU_GPU_buffer*) calloc(1,sizeof(HGPU_GPU_buffer));
        result->buffer         = 0;
        result->buffer_type    = HGPU_GPU_buffer_type_none;
        result->size           = 0;
        result->sizeof_element = 0;
        result->host_ptr       = NULL;
        result->mapped_ptr     = NULL;
        result->name           = NULL;

        // profiling data __________________________
        result->buffer_write_event   = 0;
        result->buffer_read_event    = 0;
        result->buffer_write_start   = 0;
        result->buffer_write_finish  = 0;
        result->buffer_write_elapsed_time         = 0.0;
        result->buffer_write_elapsed_time_squared = 0.0;
        result->buffer_read_elapsed_time          = 0.0;
        result->buffer_read_elapsed_time_squared  = 0.0;
        result->buffer_write_number_of = 0;
        result->buffer_read_start      = 0;
        result->buffer_read_finish     = 0;
        result->buffer_read_number_of  = 0;
    return result;
}

// delete buffer (free and delete)
// WARNING: order of buffers in parent structure should be preserved (0 buffer == last buffer)
void
HGPU_GPU_buffer_delete(HGPU_GPU_buffer** buffer){
    HGPU_GPU_buffer_release(*buffer);
    free(*buffer);
    (*buffer) = NULL;
}

// release buffer (free, but do not delete (HGPU_GPU_buffer*) structure)
void
HGPU_GPU_buffer_release(HGPU_GPU_buffer* buffer){
    if (!buffer) return;
    if (buffer->buffer) HGPU_GPU_buffer_mem_release(buffer->buffer);
    free(buffer->host_ptr);
    free(buffer->name);
    buffer->buffer   = NULL;
    buffer->host_ptr = NULL;
    buffer->name     = NULL;

    buffer->buffer_type    = HGPU_GPU_buffer_type_none;
    buffer->size           = 0;
    buffer->sizeof_element = 0;

    buffer->buffer_write_event   = 0;
    buffer->buffer_read_event    = 0;
    buffer->buffer_write_start   = 0;
    buffer->buffer_write_finish  = 0;
    buffer->buffer_write_elapsed_time         = 0.0;
    buffer->buffer_write_elapsed_time_squared = 0.0;
    buffer->buffer_read_elapsed_time          = 0.0;
    buffer->buffer_read_elapsed_time_squared  = 0.0;
    buffer->buffer_write_number_of = 0;
    buffer->buffer_read_start      = 0;
    buffer->buffer_read_finish     = 0;
    buffer->buffer_read_number_of  = 0;
}

// get index of buffer in (HGPU_GPU_buffer) array
unsigned int
HGPU_GPU_buffer_get_index(HGPU_GPU_buffer** buffers,HGPU_GPU_buffer* buffer){
    unsigned int result = 0;
    bool found = false;
    if ((!buffers) || (!buffer)) return HGPU_GPU_MAX_BUFFERS;
    while ((result<HGPU_GPU_MAX_BUFFERS) && (buffers[result])) {
        if (buffers[result]==buffer) {found=true; break;}
        result++;
    }
    if (!found) result = HGPU_GPU_MAX_BUFFERS;
    return result;
}

// get buffer by index in (HGPU_GPU_buffer) array
HGPU_GPU_buffer*
HGPU_GPU_buffer_get_by_index(HGPU_GPU_buffer** buffers,unsigned int index){
    unsigned int number_of_buffers = HGPU_GPU_buffers_get_number(buffers);
    if ((!number_of_buffers) || (index>number_of_buffers)) return NULL;
    return buffers[index];
}

// buffer initialization
unsigned int
HGPU_GPU_buffer_init(HGPU_GPU_buffer*** buffers,cl_context context,void* host_ptr_to_buffer,
                     HGPU_GPU_buffer_types buffer_type,size_t buffer_elements,size_t sizeof_element){
    if (!host_ptr_to_buffer) return HGPU_GPU_MAX_BUFFERS;
    HGPU_GPU_buffer* buffer = HGPU_GPU_buffer_new_empty();
    buffer->host_ptr = host_ptr_to_buffer;

    cl_mem clbuffer = NULL;
	cl_mem_flags flags = 0;
	switch (buffer_type) {
		// CL_MEM_USE_HOST_PTR
		// CL_MEM_ALLOC_HOST_PTR
		//
		case HGPU_GPU_buffer_type_input:	{ flags = CL_MEM_READ_ONLY  | CL_MEM_COPY_HOST_PTR; break;} // CL_MEM_COPY_HOST_PTR - or - CL_MEM_USE_HOST_PTR
		case HGPU_GPU_buffer_type_constant: { flags = CL_MEM_READ_ONLY  | CL_MEM_COPY_HOST_PTR; break;} // CL_MEM_COPY_HOST_PTR - or - CL_MEM_USE_HOST_PTR
		case HGPU_GPU_buffer_type_io:		{ flags = CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR; break;} // CL_MEM_COPY_HOST_PTR - or - CL_MEM_USE_HOST_PTR
		case HGPU_GPU_buffer_type_output:	{ flags = CL_MEM_WRITE_ONLY;                        break;} // 
		case HGPU_GPU_buffer_type_global:	{ flags = CL_MEM_READ_WRITE;                        break;}
        case HGPU_GPU_buffer_type_local:    { break;}
        case HGPU_GPU_buffer_type_uav:      { break;}
        case HGPU_GPU_buffer_type_none:     { break;}
	}
    buffer->buffer_type    = buffer_type;
    buffer->size           = buffer_elements;
    buffer->sizeof_element = sizeof_element;

    if (buffer_type!=HGPU_GPU_buffer_type_local)
        clbuffer = HGPU_GPU_buffer_create(context,flags,buffer_elements*sizeof_element,host_ptr_to_buffer);
    buffer->buffer = clbuffer;

    HGPU_GPU_buffers_add(&(*buffers),buffer);
    unsigned int result = HGPU_GPU_buffer_get_index(*buffers,buffer);

    return result;
}

// set literal name for buffer (for debugging and statistics purposes)
void
HGPU_GPU_buffer_set_name(HGPU_GPU_buffer* buffer,const char* buffer_name){
    if ((!buffer) || (!buffer_name)) return;
    free(buffer->name);
    buffer->name = HGPU_string_clone(buffer_name);
}


// buffer mapping_________________________________________________________________________________________________

// buffer map
void*
HGPU_GPU_buffer_map(HGPU_GPU_buffer* buffer,cl_command_queue queue){
    void* result = HGPU_GPU_buffer_map_async(buffer,queue);
    HGPU_GPU_buffer_wait_for_event(&buffer->buffer_read_event);
	return result;
}

// buffer map (asynchronous)
void*
HGPU_GPU_buffer_map_async(HGPU_GPU_buffer* buffer,cl_command_queue queue){
    void* result = NULL;
    if (!buffer){
        HGPU_GPU_error_note(HGPU_ERROR_BAD_BUFFER,"Trying to use null-buffer!");
        return result;
    }

    cl_int GPU_error = 0;
    cl_event buffer_event = NULL;
    size_t size = buffer->size*buffer->sizeof_element;
    result = clEnqueueMapBuffer(queue,buffer->buffer,CL_TRUE,CL_MAP_READ|CL_MAP_WRITE,0,size,0,NULL,&buffer_event,&GPU_error);
        HGPU_GPU_error_message(GPU_error,"clEnqueueMapBuffer failed");
    buffer->buffer_read_event = buffer_event;
    buffer->buffer_read_number_of++;
    buffer->mapped_ptr = (unsigned int*) result;
	return result;
}

// buffer map with profiling
void*
HGPU_GPU_buffer_map_with_profiling(HGPU_GPU_buffer* buffer,cl_command_queue queue){
    void* result = HGPU_GPU_buffer_map(buffer,queue);
    HGPU_GPU_buffer_profile_map(buffer);
	return result;
}

// profile buffer map
void
HGPU_GPU_buffer_profile_map(HGPU_GPU_buffer* buffer){
    if ((buffer) && (buffer->buffer_read_event)){
        cl_ulong buffer_read_start  = 0;
        cl_ulong buffer_read_finish = 0;
        HGPU_GPU_error_message(clGetEventProfilingInfo(buffer->buffer_read_event, CL_PROFILING_COMMAND_END,   sizeof(cl_ulong), &buffer_read_finish, 0),"clGetEventProfilingInfo failed");
        HGPU_GPU_error_message(clGetEventProfilingInfo(buffer->buffer_read_event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &buffer_read_start,  0),"clGetEventProfilingInfo failed");
        buffer->buffer_read_elapsed_time += (double) (buffer_read_finish-buffer_read_start);
        buffer->buffer_read_start         = buffer_read_start;
        buffer->buffer_read_finish        = buffer_read_finish;
    }
}


// buffer unmapping_______________________________________________________________________________________________

// buffer unmap
void
HGPU_GPU_buffer_unmap(HGPU_GPU_buffer* buffer,cl_command_queue queue,void* data_ptr){
    HGPU_GPU_buffer_unmap_async(buffer,queue,data_ptr);
    HGPU_GPU_buffer_wait_for_event(&buffer->buffer_write_event);
}

// buffer unmap with profiling
void
HGPU_GPU_buffer_unmap_with_profiling(HGPU_GPU_buffer* buffer,cl_command_queue queue,void* data_ptr){
    HGPU_GPU_buffer_unmap(buffer,queue,data_ptr);
    HGPU_GPU_buffer_profile_unmap(buffer);
}

// buffer unmap (asynchronous)
void
HGPU_GPU_buffer_unmap_async(HGPU_GPU_buffer* buffer,cl_command_queue queue,void* data_ptr){
    if (!buffer){
        HGPU_GPU_error_note(HGPU_ERROR_BAD_BUFFER,"Trying to use null-buffer!");
        return;
    }

    cl_event buffer_event = NULL;
    HGPU_GPU_error_message(clEnqueueUnmapMemObject(queue,buffer->buffer,data_ptr,0,NULL,&buffer_event),"clEnqueueUnmapMemObject failed");
    buffer->buffer_write_event = buffer_event;
    buffer->buffer_write_number_of++;
}

// profile buffer unmap
void
HGPU_GPU_buffer_profile_unmap(HGPU_GPU_buffer* buffer){
    if ((buffer) && (buffer->buffer_write_event)) {
        cl_ulong buffer_write_start  = 0;
        cl_ulong buffer_write_finish = 0;
        HGPU_GPU_error_message(clGetEventProfilingInfo(buffer->buffer_write_event, CL_PROFILING_COMMAND_END,   sizeof(cl_ulong), &buffer_write_finish, 0),"clGetEventProfilingInfo failed");
        HGPU_GPU_error_message(clGetEventProfilingInfo(buffer->buffer_write_event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &buffer_write_start,  0),"clGetEventProfilingInfo failed");
        buffer->buffer_write_elapsed_time += (double) (buffer_write_finish-buffer_write_start);
        buffer->buffer_write_start  = buffer_write_start;
        buffer->buffer_write_finish = buffer_write_finish;
    }
}




// get buffer write time
HGPU_timer_deviation
HGPU_GPU_buffer_get_time_write(HGPU_GPU_buffer* buffer){
    HGPU_timer_deviation result = HGPU_timer_deviation_default;
    if (buffer)
        result = HGPU_timer_deviation_get(buffer->buffer_write_elapsed_time,buffer->buffer_write_elapsed_time_squared,buffer->buffer_write_number_of);
    return result;
}

// get buffer read time
HGPU_timer_deviation
HGPU_GPU_buffer_get_time_read(HGPU_GPU_buffer* buffer){
    HGPU_timer_deviation result = HGPU_timer_deviation_default;
    if (buffer)
        result = HGPU_timer_deviation_get(buffer->buffer_read_elapsed_time,buffer->buffer_read_elapsed_time_squared,buffer->buffer_read_number_of);
    return result;
}



// simple functions_______________________________________________________________________________________________

// create buffer
cl_mem
HGPU_GPU_buffer_create(cl_context context,cl_mem_flags flags,size_t buffer_size_in_bytes,void* host_ptr_to_buffer){
    cl_mem result = NULL;
    if (!context) return result;
    cl_int GPU_error = CL_SUCCESS;
    result = clCreateBuffer(context,flags,buffer_size_in_bytes,host_ptr_to_buffer,&GPU_error);
    HGPU_GPU_error_message(GPU_error,"clCreateBuffer failed");
    return result;
}

// buffer release
void
HGPU_GPU_buffer_mem_release(cl_mem buffer){
    cl_int GPU_error = clReleaseMemObject(buffer);
    HGPU_GPU_error_message(GPU_error,"clReleaseMemObject failed");
}

// align buffer size
size_t
HGPU_GPU_buffer_size_align_step(size_t buffer_size,size_t step){
    return (1 + (buffer_size-1) / step) * step;
}

// wait for event
void
HGPU_GPU_buffer_wait_for_event(cl_event* buffer_event){
    if (buffer_event) {
        HGPU_GPU_error_message(clWaitForEvents(1, buffer_event),"clWaitForEvents failed");
    }
}
