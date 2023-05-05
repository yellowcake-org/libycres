#ifndef LIB_YCRES_MAP_METHODS_H
#define LIB_YCRES_MAP_METHODS_H

typedef struct yc_res_map_parse_result {
    yc_res_map_t *map;
} yc_res_map_parse_result_t;

yc_res_map_status_t yc_res_map_parse(
        const char *filename,
        const yc_res_io_fs_api_t *io,
        yc_res_map_parse_result_t *result
);

void yc_res_map_invalidate(yc_res_map_t *map);

#endif //LIB_YCRES_MAP_METHODS_H
