#include <private.h>
#include <stdlib.h>

yc_res_dat_private_load_status_t
yc_res_dat_private_load_count(yc_res_platform_reader_t* reader, void* input,unsigned long offset,
                              unsigned long* value, unsigned long* read) {
    const length = 4;
    unsigned char slice[4];
    
    switch (reader(input, offset, length, &slice[0])) {
        case YC_RES_DAT_PLATFORM_READ_STATUS_OK: break;
        case YC_RES_DAT_PLATFORM_READ_STATUS_ERROR: return YC_RES_DAT_PRIVATE_LOAD_STATUS_PLATFORM;
    }
    
    if (NULL != read)
        *read = length;
    
    *value = (slice[3] << 0) + (slice[2] << 8) + (slice[1] << 16) + (slice[0] << 24);
    return YC_RES_DAT_PRIVATE_LOAD_STATUS_OK;
}

yc_res_dat_private_load_status_t
yc_res_dat_private_load_string(yc_res_platform_reader_t* reader, void* input, unsigned long offset,
                               char** value, unsigned long *length, unsigned long* read) {
    unsigned long length_buf = 0;
    
    if (NULL != read)
        *read = 0;
    
    switch (reader(input, offset, 1, (unsigned char*)&length_buf)) {
        case YC_RES_DAT_PLATFORM_READ_STATUS_OK: break;
        case YC_RES_DAT_PLATFORM_READ_STATUS_ERROR: return YC_RES_DAT_PRIVATE_LOAD_STATUS_PLATFORM;
    }
    offset++;
    
    if (NULL != read)
        (*read)++;
    
    if (0 == length_buf)
        return YC_RES_DAT_PRIVATE_LOAD_STATUS_CORRUPTED;
    
    *value = malloc(length_buf + 1);
    
    if (NULL == *value)
        return YC_RES_DAT_PRIVATE_LOAD_STATUS_MALLOC;
    
    switch (reader(input, offset, length_buf, (unsigned char*)*value)) {
        case YC_RES_DAT_PLATFORM_READ_STATUS_OK: break;
        case YC_RES_DAT_PLATFORM_READ_STATUS_ERROR: return YC_RES_DAT_PRIVATE_LOAD_STATUS_PLATFORM;
    }
    (*value)[length_buf] = '\0';
    
    if (NULL != read)
        *read += length_buf;
    
    if (NULL != length)
        *length = length_buf;
    
    return YC_RES_DAT_PRIVATE_LOAD_STATUS_OK;
}
