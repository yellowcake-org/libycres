#ifndef LIB_YCRES_PRO_PARSE_OBJECT_SCENERY_H
#define LIB_YCRES_PRO_PARSE_OBJECT_SCENERY_H

#include <libycres.h>

typedef yc_res_pro_status_t (yc_res_pro_object_scenery_data_parser_t)(
        void *file,
        const yc_res_io_fs_api_t *api,
        yc_res_pro_object_scenery_t *into
);

yc_res_pro_object_scenery_data_parser_t yc_res_pro_object_scenery_door_parse;
yc_res_pro_object_scenery_data_parser_t yc_res_pro_object_scenery_stairs_parse;
yc_res_pro_object_scenery_data_parser_t yc_res_pro_object_scenery_elevator_parse;
yc_res_pro_object_scenery_data_parser_t yc_res_pro_object_scenery_ladder_parse;
yc_res_pro_object_scenery_data_parser_t yc_res_pro_object_scenery_generic_parse;

#endif //LIB_YCRES_PRO_PARSE_OBJECT_SCENERY_H
