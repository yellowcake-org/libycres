#include "formats/dat/dat.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

void yc_res_dat_count(yc_res_platform_reader_t* reader, const void* input, unsigned long* count) {
    if (NULL == reader || NULL == input || NULL == count)
        return; // TODO: Handle error.
    
    const bytes = 4;
    unsigned char slice[bytes];
    
    reader(input, 0, bytes, &slice[0]);
    *count = (slice[0] << 24) + (slice[1] << 16) + (slice[2] << 8) + slice[3];
}

void yc_res_dat_directories(yc_res_platform_reader_t* reader, const void* input,
                            unsigned long count, yc_res_dat_directory_t* directories) {
    assert(count != 0);
    
    if (NULL == reader || NULL == input || NULL == directories)
        return; // TODO: Handle error.
    
    unsigned long i;
    unsigned long offset = 4 * 4;
    
    for (i = 0; i < count; ++i) {
        unsigned char length = 0;
        
        reader(input, offset, 1, &length);
        offset++;
        
        directories[i].name = malloc(length + 1);
        
        if (NULL == directories[i].name)
            return; // TODO: Handle errors.
        
        directories[i].name[length] = 0;
        reader(input, offset, length, (unsigned char*)directories[i].name);
        offset += length;
    }
}

void yc_res_dat_directory_free(yc_res_dat_directory_t *directory) {
    if (NULL == directory)
        return;
    
    free(directory->name);
    directory->name = NULL;
}
