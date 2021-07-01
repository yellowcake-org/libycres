#include <private.h>

#include <stdlib.h>
#include <assert.h>

void yc_res_dat_private_load_count(yc_res_platform_reader_t* reader, void* input,
                                   unsigned long offset, unsigned long* value, unsigned long* read) {
    const length = 4;
    unsigned char slice[4];
    
    reader(input, offset, length, &slice[0]);
    
    if (NULL != read)
        *read = length;
    
    *value = (slice[3] << 0) + (slice[2] << 8) + (slice[1] << 16) + (slice[0] << 24);
}

void yc_res_dat_private_load_string(yc_res_platform_reader_t* reader, void* input, unsigned long offset,
                                    char** value, unsigned long *length, unsigned long* read) {
    unsigned long length_buf = 0;
    
    if (NULL != read)
        *read = 0;
    
    reader(input, offset, 1, (unsigned char*)&length_buf);
    offset++;
    
    if (NULL != read)
        (*read)++;
    
    *value = malloc(length_buf + 1);
    
    assert(NULL != *value);
    if (NULL == *value)
        return;
    
    reader(input, offset, length_buf, (unsigned char*)*value);
    (*value)[length_buf] = '\0';
    
    if (NULL != read)
        *read += length_buf;
    
    if (NULL != length)
        *length = length_buf;
}
