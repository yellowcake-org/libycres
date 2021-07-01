#ifndef LIBYCRES_PRIVATE_DAT_LOAD_H
#define LIBYCRES_PRIVATE_DAT_LOAD_H

#include <libycres.h>

typedef enum {
    YC_RES_DAT_PRIVATE_LOAD_STATUS_OK = 0,
    YC_RES_DAT_PRIVATE_LOAD_STATUS_MALLOC,
    YC_RES_DAT_PRIVATE_LOAD_STATUS_CORRUPTED
} yc_res_dat_private_load_status_t;

yc_res_dat_private_load_status_t
yc_res_dat_private_load_count(yc_res_platform_reader_t* reader, void* input,
                              unsigned long offset, unsigned long* value, unsigned long* read);

yc_res_dat_private_load_status_t
yc_res_dat_private_load_string(yc_res_platform_reader_t* reader, void* input,
                               unsigned long offset,char** value, unsigned long *length,
                               unsigned long* read);

#endif /* LIBYCRES_PRIVATE_DAT_LOAD_H */
