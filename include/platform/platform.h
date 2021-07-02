#ifndef LIBYCRES_PLATFORM_H
#define LIBYCRES_PLATFORM_H

typedef enum {
    YC_RES_DAT_PLATFORM_READ_STATUS_OK,
    YC_RES_DAT_PLATFORM_READ_STATUS_ERROR
} yc_res_dat_platform_reader_status_t;

typedef
yc_res_dat_platform_reader_status_t yc_res_platform_reader_t(void* input, unsigned long offset, unsigned long length,
                                                             unsigned char* output);

typedef enum {
    YC_RES_DAT_PLATFORM_WRITE_STATUS_OK,
    YC_RES_DAT_PLATFORM_WRITE_STATUS_ERROR
} yc_res_dat_platform_writer_status_t;

typedef
yc_res_dat_platform_writer_status_t yc_res_platform_writer_t(unsigned char* bytes, unsigned long length, void* output);

#endif /* LIBYCRES_PLATFORM_H */
