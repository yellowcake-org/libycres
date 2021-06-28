#include "formats/dat/dat.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

void yc_res_dat_count(yc_res_platform_reader_t* reader, const void* input, unsigned int* count) {
    if (NULL == reader || NULL == input || NULL == count)
        return; // TODO: Handle error.
    
    const length = sizeof(typeof(*count));
    unsigned char slice[length];
    
    reader(input, 0, length, &slice[0]);
    *count = (slice[0] << 24) + (slice[1] << 16) + (slice[2] << 8) + slice[3];
}

void yc_res_dat_directories(yc_res_platform_reader_t* reader, const void* input,
                            unsigned int count, yc_res_dat_directory_t* directories) {
    assert(count != 0);
    
    if (NULL == reader || NULL == input || NULL == directories)
        return; // TODO: Handle error.
    
    unsigned int i;
    unsigned int offset = 4 * 4;
    
    for (i = 0; i < count; ++i) {
        unsigned char length = 0;
        
        reader(input, offset, 1, &length);
        offset++;
        
        directories[i].name = malloc(length + 1);
        
        if (NULL == directories[i].name)
            return; // TODO: Handle errors.
        
        reader(input, offset, length, (unsigned char*)directories[i].name);
        offset += length;
        
        directories[i].name[length] = '\0';
    }
    
    for (i = 0; i < count; ++i) {
        const length = sizeof(typeof(directories[i].count));
        unsigned char slice[length];
        
        reader(input, offset, length, &slice[0]);
        offset += length;
        offset += 3 * 4; // skip unused flags
        
        directories[i].count = (slice[0] << 24) + (slice[1] << 16) + (slice[2] << 8) + slice[3];
        directories[i].files = malloc(directories[i].count * sizeof(typeof(*directories[i].files)));
        
        if (NULL == directories[i].files)
            return; // TODO: Handle errors.
        
        unsigned int j;
        for (j = 0; j < directories[i].count; ++j) {
            unsigned char length = 0;
            
            reader(input, offset, 1, &length);
            offset++;
            
            directories[i].files[j].name = malloc(length + 1);
            
            if (NULL == directories[i].files[j].name)
                return; // TODO: Handle errors.
            
            reader(input, offset, length, (unsigned char*)directories[i].files[j].name);
            offset += length;
            
            directories[i].files[j].name[length] = '\0';
            
            offset += 16;
        }
    }
}

void yc_res_dat_directory_free(yc_res_dat_directory_t *directory) {
    if (NULL == directory)
        return;
    
    if (NULL != directory->name) {
        free(directory->name);
        directory->name = NULL;
    }
    
    if (NULL != directory->files) {
        unsigned int i;
        for (i = 0; i < directory->count; ++i) {
            yc_res_dat_file_free(&directory->files[i]);
        }
        
        free(directory->files);
        directory->files = NULL;
    }
}

void yc_res_dat_file_free(yc_res_dat_file_t *file) {
    if (NULL == file)
        return;
    
    if (NULL != file->name){
        free(file->name);
        file->name = NULL;
    }
}
