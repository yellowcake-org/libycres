#ifndef LIB_YCRES_DAT_METHODS_H
#define LIB_YCRES_DAT_METHODS_H

typedef struct yc_res_dat_parse_result {
    uint32_t count;
    yc_res_dat_directory_t *list;
} yc_res_dat_parse_result_t;

yc_res_dat_status_t yc_res_dat_parse(
        const char *filename,
        const yc_res_io_fs_api_t *api,
        yc_res_dat_parse_result_t *result
);

typedef void (yc_res_dat_extract_cb_t)
        (unsigned char *bytes, size_t count, void *context);

typedef struct yc_res_dat_extract_result {
    yc_res_dat_extract_cb_t *callback;
    void *context;
} yc_res_dat_extract_result_t;

yc_res_dat_status_t yc_res_dat_extract(
        const char *filename,
        const yc_res_io_fs_api_t *api,
        yc_res_dat_file_t *file,
        yc_res_dat_extract_result_t *result
);

void yc_res_dat_file_invalidate(yc_res_dat_file_t *file);

void yc_res_dat_directory_invalidate(yc_res_dat_directory_t *directory);

#endif //LIB_YCRES_DAT_METHODS_H
