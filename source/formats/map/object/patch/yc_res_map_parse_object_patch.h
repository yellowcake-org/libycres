#ifndef SOURCE_FORMATS_MAP_OBJECT_PATCH_H
#define SOURCE_FORMATS_MAP_OBJECT_PATCH_H

typedef yc_res_map_status_t (yc_res_map_parse_object_patch_parser_t)(
        void *file,
        const yc_res_io_fs_api_t *api,
        const yc_res_map_parse_db_api_t *fetchers,
        uint32_t pid,
        yc_res_map_level_object_patch_t *into
);

#endif //SOURCE_FORMATS_MAP_OBJECT_PATCH_H
