#include <undat.h>
#include <stdio.h>

yc_res_dat_platform_reader_status_t
undat_platform_file_reader(void* input, unsigned long offset, unsigned long length, unsigned char* output) {
    FILE* handle = (FILE*)input;
    
    if (0 != fseeko(handle, offset, SEEK_SET))
        return YC_RES_DAT_PLATFORM_READ_STATUS_ERROR;
        
    if (length > fread(output, sizeof(*output), length, handle))
        return YC_RES_DAT_PLATFORM_READ_STATUS_ERROR;
        
    return YC_RES_DAT_PLATFORM_READ_STATUS_OK;
}

yc_res_dat_platform_writer_status_t
undat_platform_file_write(unsigned char* bytes, unsigned long length, void* output) {
    FILE* handle = (FILE*)output;
    
    if (0 != fseeko(handle, 0, SEEK_END))
        return YC_RES_DAT_PLATFORM_WRITE_STATUS_ERROR;
        
    if (length > fwrite(bytes, sizeof(*bytes), length, output))
        return YC_RES_DAT_PLATFORM_WRITE_STATUS_ERROR;
        
    return YC_RES_DAT_PLATFORM_WRITE_STATUS_OK;
}
