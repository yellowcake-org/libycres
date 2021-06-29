#include "formats/dat/dat.h"
#include "dat.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

void yc_res_dat_count(yc_res_platform_reader_t* reader, const void* input, unsigned long* count) {
    if (NULL == reader || NULL == input || NULL == count)
        return; // TODO: Handle error.
    
    yc_res_dat_private_load_count(reader, input, 0, count, NULL);
}

void yc_res_dat_directories(yc_res_platform_reader_t* reader, const void* input,
                            unsigned long count, yc_res_dat_directory_t* directories) {
    assert(count != 0);
    
    if (NULL == reader || NULL == input || NULL == directories)
        return; // TODO: Handle error.
    
    unsigned long i;
    unsigned long offset = 4 * 4;
    
    for (i = 0; i < count; ++i) {
        unsigned long read;
        yc_res_dat_private_load_string(reader, input, offset, &directories[i].name, &read);
        offset += read;
    }
    
    for (i = 0; i < count; ++i) {
        unsigned long read;
        
        yc_res_dat_private_load_count(reader, input, offset, &directories[i].count, &read);
        offset += read;
        offset += 3 * 4; // skip attributes
        
        directories[i].files = malloc(directories[i].count * sizeof(typeof(*directories[i].files)));
        
        if (NULL == directories[i].files)
            return; // TODO: Handle errors.
        
        unsigned int j;
        for (j = 0; j < directories[i].count; ++j) {
            unsigned long read;
            yc_res_dat_private_load_string(reader, input, offset, &directories[i].files[j].name, &read);
            offset += read;
            offset += 4; // skip attributes
            
            yc_res_dat_private_load_count(reader, input, offset, &directories[i].files[j].start, &read);
            offset += read;
            
            unsigned long plain_size, packed_size;
            yc_res_dat_private_load_count(reader, input, offset, &plain_size, &read);
            offset += read;
            
            yc_res_dat_private_load_count(reader, input, offset, &packed_size, &read);
            offset += read;
            
            directories[i].files[j].size = packed_size > 0 ? packed_size : plain_size;
            directories[i].files[j].original_size = packed_size > 0 ? plain_size : 0;
        }
    }
}

void yc_res_dat_free_directory(yc_res_dat_directory_t* directory) {
    if (NULL == directory)
        return;
    
    if (NULL != directory->name) {
        free(directory->name);
        directory->name = NULL;
    }
    
    if (NULL != directory->files) {
        unsigned int i;
        for (i = 0; i < directory->count; ++i) {
            yc_res_dat_free_file(&directory->files[i]);
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
