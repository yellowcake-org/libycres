#ifndef SOURCE_FORMATS_MAP_OBJECT_H
#define SOURCE_FORMATS_MAP_OBJECT_H

#include "patch/yc_res_map_parse_object_patch.h"

yc_res_map_status_t yc_res_map_parse_object(
        void *file,
        const yc_res_io_fs_api_t *api,
        const yc_res_map_parse_db_api_t *fetchers,
        yc_res_map_level_object_t *into
);

#endif //SOURCE_FORMATS_MAP_OBJECT_H
