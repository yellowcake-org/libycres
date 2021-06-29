#include <libycres.h>
#include <private.h>

#include <stdlib.h>

void yc_res_dat_private_load_count(yc_res_platform_reader_t* reader, const void* input,
                                   unsigned long offset, unsigned long* value, unsigned long* read) {
    const length = 4;
    unsigned char slice[length];
    
    reader(input, offset, length, &slice[0]);
    
    if (NULL != read) {
        *read = length;
    }
    
    *value = (slice[3] << 0) + (slice[2] << 8) + (slice[1] << 16) + (slice[0] << 24);
}

void yc_res_dat_private_load_string(yc_res_platform_reader_t* reader, const void* input,
                                    unsigned long offset, char** value, unsigned long* read) {
    unsigned char length = 0;
    
    reader(input, offset, 1, &length);
    offset++;
    
    
    if (NULL != read) {
        *read = length + 1;
    }
    
    *value = malloc(length + 1);
    
    if (NULL == *value)
        return; // TODO: Handle errors.
    
    reader(input, offset, length, (unsigned char*)*value);
    (*value)[length] = '\0';
}

void yc_res_dat_private_append_marked_dirs(yc_res_dat_directory_t* node, yc_res_dat_directory_t** flat,
                                           unsigned long* appended) {
    if (node->has_content_block == 1) {
        flat[*appended] = node;
        (*appended)++;
    }
    
    unsigned long i;
    for (i = 0; i< node->directories_count; ++i) {
        yc_res_dat_private_append_marked_dirs(&node->directories[i], flat, appended);
    }
}
