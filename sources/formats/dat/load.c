#include <private.h>
#include <stdlib.h>

void yc_res_dat_private_load_count(yc_res_platform_reader_t* reader, void* input,
                                   unsigned long offset, unsigned long* value, unsigned long* read) {
    const length = 4;
    unsigned char slice[4];
    
    reader(input, offset, length, &slice[0]);
    
    if (NULL != read) {
        *read = length;
    }
    
    *value = (slice[3] << 0) + (slice[2] << 8) + (slice[1] << 16) + (slice[0] << 24);
}

void yc_res_dat_private_load_string(yc_res_platform_reader_t* reader, void* input,
                                    unsigned long offset, char** value, unsigned long* read) {
    unsigned char length = 0;
    
    reader(input, offset, 1, &length);
    offset++;
    
    
    if (NULL != read) {
        *read = length + 1;
    }
    
    *value = malloc(length + 1);
    
    if (NULL == *value)
        return;
    
    reader(input, offset, length, (unsigned char*)*value);
    (*value)[length] = '\0';
}
