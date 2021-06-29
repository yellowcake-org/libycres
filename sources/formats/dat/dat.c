#include <libycres.h>
#include <private.h>

#include <stddef.h>
#include <stdlib.h>

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
        yc_res_dat_private_load_string(reader, input, offset, &list[i].name, &read);
        offset += read;
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
            unsigned long read;
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
