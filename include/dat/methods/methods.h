#ifndef LIB_YCRES_DAT_METHODS_H
#define LIB_YCRES_DAT_METHODS_H

#include "dat/statuses/statuses.h"
#include "dat/callbacks/callbacks.h"

yc_res_dat_status_t yc_res_dat_parse(
        const char *filename,
        const yc_res_io_fs_api_t *io,
        yc_res_dat_list_cb_t *callback
);

yc_res_dat_status_t yc_res_dat_extract(
        const char *filename,
        const yc_res_io_fs_api_t *io,
        yc_res_dat_file_t *file,
        yc_res_dat_extract_cb_t *callback
);

void yc_res_dat_invalidate_file(yc_res_dat_file_t *file);
void yc_res_dat_invalidate_directory(yc_res_dat_directory_t *directory);

#endif //LIB_YCRES_DAT_METHODS_H
