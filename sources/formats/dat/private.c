#include "dat.h"

#include <stddef.h>
#include <stdlib.h>

void yc_res_dat_private_load_uint(yc_res_platform_reader_t* reader, const void* input,
                                  unsigned int offset, unsigned int* value) {
    const length = 4;
    unsigned char slice[length];
    
    reader(input, offset, length, &slice[0]);
    *value = (slice[0] << 24) + (slice[1] << 16) + (slice[2] << 8) + slice[3];
}

void yc_res_dat_private_load_string(yc_res_platform_reader_t* reader, const void* input,
                                    unsigned int offset, char** value, unsigned int* read) {
    unsigned char length = 0;
    
    reader(input, offset, 1, &length);
    offset++;
    
    *read = length + 1;
    *value = malloc(length + 1);
    
    if (NULL == *value)
        return; // TODO: Handle errors.
    
    reader(input, offset, length, (unsigned char*)*value);
    (*value)[length] = '\0';
}
