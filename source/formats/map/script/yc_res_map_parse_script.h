#ifndef SOURCE_FORMATS_MAP_SCRIPT_H
#define SOURCE_FORMATS_MAP_SCRIPT_H

yc_res_map_status_t yc_res_map_parse_script(
        void *file, const yc_res_io_fs_api_t *api, yc_res_map_script_t *into, yc_res_pro_script_type_t type
);

#endif //SOURCE_FORMATS_MAP_SCRIPT_H
