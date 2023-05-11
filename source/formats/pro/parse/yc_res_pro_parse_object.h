#ifndef SOURCE_FORMATS_PRO_PARSE_H
#define SOURCE_FORMATS_PRO_PARSE_H

#include <libycres.h>

yc_res_pro_object_flags_t yc_res_pro_parse_flags(uint32_t flags);
typedef yc_res_pro_status_t (yc_res_pro_object_data_parser_t)(
        void *file,
        const yc_res_io_fs_api_t *api,
        yc_res_pro_object_t *into
);

yc_res_pro_object_data_parser_t yc_res_pro_object_item_parse;
yc_res_pro_object_data_parser_t yc_res_pro_object_critter_parse;
yc_res_pro_object_data_parser_t yc_res_pro_object_scenery_parse;
yc_res_pro_object_data_parser_t yc_res_pro_object_wall_parse;
yc_res_pro_object_data_parser_t yc_res_pro_object_tile_parse;
yc_res_pro_object_data_parser_t yc_res_pro_object_misc_parse;

#endif //SOURCE_FORMATS_PRO_PARSE_H
