#ifndef YC_RES_MAP_PARSE_OBJECT_H
#define YC_RES_MAP_PARSE_OBJECT_H

#include "patch/yc_res_map_parse_object_patch.h"

yc_res_map_status_t yc_res_map_parse_object(
        void *file,
        const yc_res_io_fs_api_t *io,
        const yc_res_map_parse_db_api_t *db,
        yc_res_map_level_object_t *into
);

#endif //YC_RES_MAP_PARSE_OBJECT_H
