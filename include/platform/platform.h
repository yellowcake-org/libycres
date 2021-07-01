#ifndef LIBYCRES_PLATFORM_H
#define LIBYCRES_PLATFORM_H

typedef enum {
    YC_RES_DAT_PLATFORM_READ_STATUS_OK,
    YC_RES_DAT_PLATFORM_READ_STATUS_ERROR
} yc_res_dat_platform_reader_status_t;

typedef
yc_res_dat_platform_reader_status_t yc_res_platform_reader_t(void* input, unsigned long offset, unsigned long length,
                                                             unsigned char* output);

#endif /* LIBYCRES_PLATFORM_H */
