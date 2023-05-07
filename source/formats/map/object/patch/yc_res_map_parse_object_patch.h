#ifndef YC_RES_MAP_PARSE_OBJECT_PATCH_H
#define YC_RES_MAP_PARSE_OBJECT_PATCH_H

typedef yc_res_map_status_t (yc_res_map_parse_object_patch_parser_t)
        (void *file, const yc_res_io_fs_api_t *io, yc_res_map_level_object_patch_t *into);

yc_res_map_parse_object_patch_parser_t yc_res_map_parse_object_patch_misc;

#endif //YC_RES_MAP_PARSE_OBJECT_PATCH_H
