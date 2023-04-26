#ifndef LIB_YCRES_PRO_PARSE_OBJECT_ITEM_H
#define LIB_YCRES_PRO_PARSE_OBJECT_ITEM_H

#include <libycres.h>

typedef yc_res_pro_status_t (_yc_res_pro_object_item_data_parser_t) (
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_pro_object_item_t *into
);

_yc_res_pro_object_item_data_parser_t yc_res_pro_object_item_armor_parse;
_yc_res_pro_object_item_data_parser_t yc_res_pro_object_item_container_parse;
_yc_res_pro_object_item_data_parser_t yc_res_pro_object_item_drug_parse;

#endif //LIB_YCRES_PRO_PARSE_OBJECT_ITEM_H
