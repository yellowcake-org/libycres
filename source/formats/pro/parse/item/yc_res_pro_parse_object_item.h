#ifndef SOURCE_FORMATS_PRO_PARSE_ITEM_H
#define SOURCE_FORMATS_PRO_PARSE_ITEM_H

#include <libycres.h>

typedef yc_res_pro_status_t (yc_res_pro_object_item_data_parser_t)(
        void *file,
        const yc_res_io_fs_api_t *api,
        yc_res_pro_object_item_t *into
);

yc_res_pro_object_item_data_parser_t yc_res_pro_object_item_armor_parse;
yc_res_pro_object_item_data_parser_t yc_res_pro_object_item_container_parse;
yc_res_pro_object_item_data_parser_t yc_res_pro_object_item_drug_parse;
yc_res_pro_object_item_data_parser_t yc_res_pro_object_item_weapon_parse;
yc_res_pro_object_item_data_parser_t yc_res_pro_object_item_ammo_parse;
yc_res_pro_object_item_data_parser_t yc_res_pro_object_item_misc_parse;
yc_res_pro_object_item_data_parser_t yc_res_pro_object_item_key_parse;

#endif //SOURCE_FORMATS_PRO_PARSE_ITEM_H
