/******************************************************************************
 * @file     hgpu_io.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0
 *
 * @brief    [HGPU library]
 *           i/o submodule for HGPU package
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

#include"../include/hgpu_io.h"

    char* HGPU_io_path_working = NULL;
    char* HGPU_io_path_inf     = NULL;
    char* HGPU_io_path_current = NULL;
    char* HGPU_io_path_root    = NULL;

// path section___________________________________________________________________________________________________

// get path
void
HGPU_io_path_get(char** path){
    if (!path) HGPU_error(HGPU_ERROR_ARRAY_OUT_OF_BOUND);
    size_t path_length = HGPU_FILENAME_MAX;
    char* path_new = *path;
    if (!path_new) HGPU_string_resize(&path_new,path_length);
    if (!GetCurrentDir(path_new,(int) path_length)) HGPU_error_message(HGPU_ERROR_DEVICE_INITIALIZATION_FAILED,"could not obtain inf path");
    HGPU_io_path_set_separator(&path_new,path_length);
    *path = path_new;
}

// get .cl root path
char*
HGPU_io_path_get_root(void){
    HGPU_io_path_get(&HGPU_io_path_current);
    HGPU_io_path_get(&HGPU_io_path_root);
    char* j_prev = HGPU_io_path_root;
    char* j_cur  = HGPU_io_path_root;
    bool flag_found = false;
    while((!flag_found)&&(j_cur)){
        sprintf_s(HGPU_io_path_root,HGPU_FILENAME_MAX,"%s%s",HGPU_io_path_current,HGPU_GPU_PATH_SEPARATOR);
        j_cur = strstr(j_prev+1,HGPU_GPU_PATH_SEPARATOR);
        if((j_cur) && (j_cur>j_prev)) {
            j_prev=j_cur;
            sprintf_s(j_cur+1,HGPU_FILENAME_MAX-(j_cur+1-HGPU_io_path_root),"%s",HGPU_GPU_ROOT_FILE);

            if (HGPU_io_file_check_existence(HGPU_io_path_root)) {
                sprintf_s(j_cur,HGPU_FILENAME_MAX-(j_cur+1-HGPU_io_path_root),"%s",HGPU_GPU_PATH_SEPARATOR);
                flag_found = true;
            }
        }
    }
    if (!flag_found) {
        HGPU_io_path_root[0] = 0;
        HGPU_error_message(HGPU_ERROR_PATH_NOT_FOUND_ROOT,"could not determine OpenCL kernels root path (file hgpucl.txt not found)");
    }
    return HGPU_io_path_root;
}

// setup HGPU_GPU_PATH_SEPARATOR as last symbol in path
void
HGPU_io_path_set_separator(char** path,size_t path_length){
    if ((!path) || (!(*path)) || (!strlen(*path))) return;
    char** path_new = path;
    size_t path_length_old = strlen(*path_new);
    char* path_last_symbol = (char*) (*path_new)+path_length_old;
    char* j_cur  = strstr(path_last_symbol - strlen(HGPU_GPU_PATH_SEPARATOR), HGPU_GPU_PATH_SEPARATOR);
    char* j_cur2 = strstr(path_last_symbol - strlen(HGPU_GPU_PATH_SEPARATOR2),HGPU_GPU_PATH_SEPARATOR2);
    if ((!j_cur) && (!j_cur2)) {
        size_t new_path_length = path_length_old+strlen(HGPU_GPU_PATH_SEPARATOR)+1;
        if (new_path_length>path_length) HGPU_string_resize(path_new,new_path_length); // resize path
        char* path_last_symbol_new = (char*) (*path_new)+path_length_old;
        sprintf_s(path_last_symbol_new,new_path_length,"%s",HGPU_GPU_PATH_SEPARATOR);
        *path = *path_new;
    }
#ifdef _WIN32
        HGPU_string_char_replace(*path,92,47); // replace "\\" symbols with "/" in root cl path
#endif
}

// combine path with filename
void
HGPU_io_path_join_filename(char** file_path_and_name,const size_t file_path_and_name_length,const char* file_path,const char* file_name){
    if (!file_path_and_name) HGPU_error(HGPU_ERROR_ARRAY_OUT_OF_BOUND);
    size_t j = 0;
    char*  file_new = *file_path_and_name;
    size_t f_length = strlen(file_name)+2;
    if (file_path) f_length += strlen(file_path);
    if (file_path_and_name_length<f_length) HGPU_string_resize(&file_new,f_length);
    if (file_path){
        j = sprintf_s(file_new,f_length,"%s",file_path);
        HGPU_io_path_set_separator(&file_new,f_length);
        j = strlen(file_new);
    }
    j += sprintf_s(file_new+j,f_length-j,"%s",file_name);
#ifdef _WIN32
    HGPU_string_char_replace(file_new,92,47);   // replace "\\" symbols with "/" in root cl path
#endif
    (*file_path_and_name) = file_new;
}

// files section__________________________________________________________________________________________________

// check file existance
bool
HGPU_io_file_check_existence(const char* file_name){
    return HGPU_io_file_check_existence_with_path(NULL,file_name);
}

// check file existance with path
bool
HGPU_io_file_check_existence_with_path(const char* file_path,const char* file_name){
	FILE * is_file;
    bool flag = false;
        if (!file_name) return flag;
        char* buffer = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
        if (!buffer) HGPU_error(HGPU_ERROR_NO_MEMORY);
        HGPU_io_path_join_filename(&buffer,HGPU_FILENAME_MAX,file_path,file_name);
        fopen_s(&is_file,buffer,"r");
        if (is_file) {
            flag = true;
            fclose(is_file);
        }
        free(buffer);
    return flag;
}

// read file into buffer
char*
HGPU_io_file_read(const char* file_name){	
    FILE* file_to_read;
    char buffer[HGPU_FILENAME_MAX];
    sprintf_s(buffer,sizeof(buffer),"%s",file_name);
    fopen_s(&file_to_read,buffer,"r");
    if(!file_to_read){
        HGPU_error_message(HGPU_ERROR_FILE_NOT_FOUND,".cl-file not found");
        return NULL;
    }
    fseek (file_to_read, 0, SEEK_END);
    unsigned int file_length = ftell(file_to_read);
    fseek (file_to_read, 0 , SEEK_SET);
    char* cl_kernels_source = (char*) calloc(file_length + 2, sizeof(char));
    if (!cl_kernels_source) HGPU_error(HGPU_ERROR_NO_MEMORY);
    unsigned int file_read_length = (unsigned int) fread(cl_kernels_source, sizeof(char), file_length, file_to_read);
    if (!file_read_length) free(cl_kernels_source);
    if (fclose(file_to_read)) HGPU_error_note(HGPU_ERROR_FILE_NOT_CLOSED,"the file was not closed");
    return cl_kernels_source;
}

// write buffer into file
void
HGPU_io_file_write(const char* file_name,const char* buffer){
    HGPU_io_file_write_with_path(NULL,file_name,buffer);
}

// write buffer into file with path
void
HGPU_io_file_write_with_path(const char* file_path,const char* file_name,const char* buffer){
    char* filename_with_path = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    if (!filename_with_path) HGPU_error(HGPU_ERROR_NO_MEMORY);
    HGPU_io_path_join_filename(&filename_with_path,HGPU_FILENAME_MAX,file_path,file_name);
    FILE* file_for_write;
    fopen_s(&file_for_write,filename_with_path,"w+");
    free(filename_with_path);
    if(file_for_write){
        fprintf(file_for_write,buffer);
        if (fclose(file_for_write)) HGPU_error_note(HGPU_ERROR_FILE_NOT_CLOSED,"the file was not closed");
    } else
        HGPU_error_message(HGPU_ERROR_FILE_NOT_FOUND,"error writting file");
}


// write buffer into binary file
void
HGPU_io_file_write_binary(const char* file_name,const unsigned char* buffer,const size_t bytes_to_write){
    FILE* file_for_write;
    fopen_s(&file_for_write,file_name,"wb");
    if(file_for_write){
        fwrite(buffer,sizeof(unsigned char),bytes_to_write,file_for_write);
        if (fclose(file_for_write)) HGPU_error_note(HGPU_ERROR_FILE_NOT_CLOSED,"the file was not closed");
    } else
        HGPU_error_message(HGPU_ERROR_FILE_NOT_FOUND,"error writting binary file");
}

// file read and add to existing buffer
void
HGPU_io_file_read_add(char** source,const char* file_name){
	char* temporary_source = HGPU_io_file_read(file_name);
    if (!temporary_source) return;
    size_t source_length = strlen(*source);
    size_t temporary_source_length = strlen(temporary_source);
    const char* source_separator = "\n\n\n";  // separator between two source codes
    size_t source_separator_length = strlen(source_separator);
    size_t new_source_length = source_length + temporary_source_length + source_separator_length + 2;
    char* new_source = (char*) realloc(*source,new_source_length);
    if (!new_source) {
        HGPU_error_note(HGPU_ERROR_NO_MEMORY,NULL);
        free(temporary_source);
        return;
    }

    memcpy_s(new_source+source_length,new_source_length,source_separator,source_separator_length);
    memcpy_s(new_source+source_length+source_separator_length,new_source_length,temporary_source,temporary_source_length);
    new_source[new_source_length-2] = 0;
    free(temporary_source);
    (*source) = new_source;
}

// read file into buffer
char*
HGPU_io_file_read_with_path(const char* file_path,const char* file_name){
    char* buffer = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    if (!buffer) HGPU_error(HGPU_ERROR_NO_MEMORY);
    HGPU_io_path_join_filename(&buffer,HGPU_FILENAME_MAX,file_path,file_name);
    char* result = HGPU_io_file_read(buffer);
    free(buffer);
    return result;
}

// file read and add to existing buffer
void
HGPU_io_file_read_add_with_path(char** source,const char* file_path,const char* file_name){
    char* buffer = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    if (!buffer) HGPU_error(HGPU_ERROR_NO_MEMORY);
    HGPU_io_path_join_filename(&buffer,HGPU_FILENAME_MAX,file_path,file_name);
    HGPU_io_file_read_add(source,buffer);
    free(buffer);
}


// .inf-files section_____________________________________________________________________________________________

// delete .inf and .bin files
int
HGPU_io_inf_file_delete(int inf_index){
	int err;
    char* buffer_path = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    char* buffer_inf  = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    if ((!buffer_inf) || (!buffer_path)) HGPU_error(HGPU_ERROR_NO_MEMORY);

	sprintf_s(buffer_inf,HGPU_FILENAME_MAX,"program%u.inf",inf_index);
    HGPU_io_path_join_filename(&buffer_path,HGPU_FILENAME_MAX,HGPU_io_path_inf,buffer_inf);
    err = remove(buffer_path);      // kill .inf-file
    if (!err){
		sprintf_s(buffer_inf,HGPU_FILENAME_MAX,"program%u.bin",inf_index);
        HGPU_io_path_join_filename(&buffer_path,HGPU_FILENAME_MAX,HGPU_io_path_inf,buffer_inf);
        err = remove(buffer_path);  // kill .bin-file
    }
    free(buffer_inf);
    free(buffer_path);
    return err;
}

// kill all .inf and .bin files
void
HGPU_io_inf_file_delete_all(void){
    int err = 0;
    int inf_index = 1;
    while (!err) err = HGPU_io_inf_file_delete(inf_index++);
}

// rename .inf and .bin files
int
HGPU_io_inf_file_rename(int inf_index_old,int inf_index_new){
	int err = 0;
    char* buffer_path_old = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    char* buffer_path_new = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    char* buffer_inf_old  = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    char* buffer_inf_new  = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    if ((!buffer_path_old) || (!buffer_path_new) || (!buffer_inf_old) || (!buffer_inf_new)) HGPU_error(HGPU_ERROR_NO_MEMORY);

	sprintf_s(buffer_inf_old,HGPU_FILENAME_MAX,"program%u.inf",inf_index_old);
    HGPU_io_path_join_filename(&buffer_path_old,HGPU_FILENAME_MAX,HGPU_io_path_inf,buffer_inf_old);
	sprintf_s(buffer_inf_new,HGPU_FILENAME_MAX,"program%u.inf",inf_index_new);
    HGPU_io_path_join_filename(&buffer_path_new,HGPU_FILENAME_MAX,HGPU_io_path_inf,buffer_inf_new);
    err = rename(buffer_path_old,buffer_path_new);    // rename .inf-file
    if (!err) {
    	sprintf_s(buffer_inf_old,HGPU_FILENAME_MAX,"program%u.bin",inf_index_old);
        HGPU_io_path_join_filename(&buffer_path_old,HGPU_FILENAME_MAX,HGPU_io_path_inf,buffer_inf_old);
	    sprintf_s(buffer_inf_new,HGPU_FILENAME_MAX,"program%u.bin",inf_index_new);
        HGPU_io_path_join_filename(&buffer_path_new,HGPU_FILENAME_MAX,HGPU_io_path_inf,buffer_inf_new);
        err = rename(buffer_path_old,buffer_path_new);    // rename .inf-file
    }
    free(buffer_path_old);
    free(buffer_path_new);
    free(buffer_inf_old);
    free(buffer_inf_new);

    return err;
}

// get max index of .inf and .bin files
int
HGPU_io_inf_file_get_max_index(void){
    int result = -1;
    bool flag = true;
    while(flag){
        flag = HGPU_io_inf_file_check_existence(result+2);
        result++;
    }
    return result;
}

// check .inf file existance
bool
HGPU_io_inf_file_check_existence(int inf_index){
    bool result = false;
    char buffer_inf[HGPU_FILENAME_MAX];
	sprintf_s(buffer_inf,HGPU_FILENAME_MAX,"program%u.inf",inf_index);
    result = HGPU_io_file_check_existence_with_path(HGPU_io_path_inf,buffer_inf);
    return result;
}


// .bin-files section_____________________________________________________________________________________________

// check .bin file existance
bool
HGPU_io_bin_file_check_existence(int inf_index){
    bool result = false;
    char buffer_inf[HGPU_FILENAME_MAX];
	sprintf_s(buffer_inf,HGPU_FILENAME_MAX,"program%u.bin",inf_index);
    result = HGPU_io_file_check_existence_with_path(HGPU_io_path_inf,buffer_inf);
    return result;
}

// read .bin-file into buffer
unsigned char*
HGPU_io_bin_file_read(int inf_index,size_t* binary_size){
    unsigned char* result = NULL;
    FILE* file_to_read;
    char* buffer_bin = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    char* buffer_inf = (char*) calloc(HGPU_FILENAME_MAX,sizeof(char));
    if ((!buffer_bin) || (!buffer_inf)) HGPU_error(HGPU_ERROR_NO_MEMORY);

	sprintf_s(buffer_inf,HGPU_FILENAME_MAX,"program%u.bin",inf_index);
    HGPU_io_path_join_filename(&buffer_bin,HGPU_FILENAME_MAX,HGPU_io_path_inf,buffer_inf);
    if (HGPU_io_file_check_existence(buffer_bin)) {
        fopen_s(&file_to_read,buffer_bin,"rb");
        fseek (file_to_read, 0, SEEK_END);
        *binary_size = ftell(file_to_read);
        rewind(file_to_read);
        result = (unsigned char*) calloc ((*binary_size) + 2,sizeof(unsigned char));
        if (!result) HGPU_error(HGPU_ERROR_NO_MEMORY);
        size_t file_read_length = fread(result,1,(*binary_size),file_to_read);
        if (!file_read_length) free(result);
        if (fclose(file_to_read)) HGPU_error_note(HGPU_ERROR_FILE_NOT_CLOSED,"the file was not closed");
        (*binary_size) = file_read_length;
    }
    free(buffer_bin);
    free(buffer_inf);
    return result;
}
