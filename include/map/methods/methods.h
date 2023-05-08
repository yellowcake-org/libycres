#ifndef LIB_YCRES_MAP_METHODS_H
#define LIB_YCRES_MAP_METHODS_H

typedef yc_res_pro_object_item_type_t yc_res_map_parse_db_api_item_fetcher_t(uint32_t pid);
typedef yc_res_pro_object_scenery_type_t yc_res_map_parse_db_api_scenery_fetcher_t(uint32_t pid);

typedef struct yc_res_map_parse_db_api {
    yc_res_map_parse_db_api_item_fetcher_t *item_type_from_pid;
    yc_res_map_parse_db_api_scenery_fetcher_t *scenery_type_from_pid;
} yc_res_map_parse_db_api_t;

typedef struct yc_res_map_parse_result {
    yc_res_map_t *map;
} yc_res_map_parse_result_t;

yc_res_map_status_t yc_res_map_parse(
        const char *filename,
        const yc_res_io_fs_api_t *io,
        const yc_res_map_parse_db_api_t *db,
        yc_res_map_parse_result_t *result
);

void yc_res_map_invalidate(yc_res_map_t *map);

#endif //LIB_YCRES_MAP_METHODS_H
