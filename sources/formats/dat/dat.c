#include <libycres.h>
#include <private.h>

#include <stdlib.h>
#include <memory.h>

#include <stdio.h>

void yc_res_dat_tree(yc_res_platform_reader_t* reader, const void* input, yc_res_dat_directory_t** root) {
    if (NULL == reader || NULL == input || NULL == root)
        return; // TODO: Handle error.
    
    unsigned long i;
    unsigned long read;
    unsigned long offset = 0;
    
    unsigned long count = 0;
    
    yc_res_dat_private_load_count(reader, input, 0, &count, &read);
    offset += read;
    offset += 3 * 4; // skip attributes
    
    // list of directories presented as is within dat file
    yc_res_dat_directory_t* list = malloc(sizeof(typeof(*list)) * count);
    
    if (NULL == list)
        return; //TODO: Handle errors.
    
    for (i = 0; i < count; ++i) {
        char* path;
        
        yc_res_dat_private_load_string(reader, input, offset, &path, &read);
        offset += read;
        
        unsigned long path_size = read;
        if (1 > path_size || path[0] != '.') {
            path = realloc(path, path_size + 2);
            
            if (NULL == path) {
                free(list);
                return; // TODO: Handle errors.
            }
            
            memmove(path + 2, path, path_size);
            path_size += 2;
            
            path[0] = '.';
            path[1] = '\\';
        }
        
        printf("Fixed path: %s\n", path);
        
        unsigned long di;
        for (di = path_size - 1; di > 0; --di) {
            if (path[di] == '\\') {
                di++;
                break;
            }
        }
        
        list[i].name = malloc(sizeof(typeof(*list[i].name)) * path_size - di);
        
        if (NULL == list[i].name) {
            free(list);
            free(path);
            return; // TODO: Handle errors.
        }
        
        memcpy(list[i].name, &path[di], path_size - di);
        printf("Name: %s\n", list[i].name);
                
        list[i].directories = NULL;
        list[i].directories_count = 0;
        
        list[i].files = NULL;
        list[i].files_count = 0;
        
        //
        
        free(path);
        path = NULL;
    }
    
    for (i = 0; i < count; ++i) {
        yc_res_dat_private_load_count(reader, input, offset, &list[i].files_count, &read);
        offset += read;
        offset += 3 * 4; // skip attributes
        
        list[i].files = malloc(list[i].files_count * sizeof(typeof(*list[i].files)));
        
        if (NULL == list[i].files)
            return; // TODO: Handle errors.
        
        unsigned long j;
        for (j = 0; j < list[i].files_count; ++j) {
            yc_res_dat_private_load_string(reader, input, offset, &list[i].files[j].name, &read);
            offset += read;
            offset += 4; // skip attributes
            
            yc_res_dat_private_load_count(reader, input, offset, &list[i].files[j].start, &read);
            offset += read;
            
            unsigned long plain_size, packed_size;
            
            yc_res_dat_private_load_count(reader, input, offset, &plain_size, &read);
            offset += read;
            
            yc_res_dat_private_load_count(reader, input, offset, &packed_size, &read);
            offset += read;
            
            list[i].files[j].size = packed_size > 0 ? packed_size : plain_size;
            list[i].files[j].original_size = packed_size > 0 ? plain_size : 0;
        }
    }
    
    free(list);
    list = NULL;
}

void yc_res_dat_free_tree(yc_res_dat_directory_t* directory) {
    if (NULL == directory)
        return;
    
    if (NULL != directory->name) {
        free(directory->name);
        directory->name = NULL;
    }
    
    if (NULL != directory->files && 0 < directory->files_count) {
        unsigned long i;
        for (i = 0; i < directory->files_count; ++i) {
            yc_res_dat_free_file(&directory->files[i]);
        }
        
        free(directory->files);
        directory->files = NULL;
    }
    
    if (NULL != directory->directories && 0 < directory->directories_count) {
        unsigned long i;
        for (i = 0; i < directory->directories_count; ++i) {
            yc_res_dat_free_tree(&directory->directories[i]);
        }
        
        free(directory->files);
        directory->files = NULL;
    }
}

void yc_res_dat_free_file(yc_res_dat_file_t* file) {
    if (NULL == file)
        return;
    
    if (NULL != file->name){
        free(file->name);
        file->name = NULL;
    }
}
