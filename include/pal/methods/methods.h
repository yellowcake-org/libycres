#ifndef INCLUDE_PAL_METHODS_H
#define INCLUDE_PAL_METHODS_H

bool yc_res_pal_color_is_transparent(yc_res_pal_color_t *color);

typedef struct yc_res_pal_parse_result {
    size_t count;
    yc_res_pal_color_t *colors;
} yc_res_pal_parse_result_t;


yc_res_pal_status_t yc_res_pal_parse(
        const char *filename,
        const yc_res_io_fs_api_t *api,
        yc_res_pal_parse_result_t *result
);

#endif //INCLUDE_PAL_METHODS_H
