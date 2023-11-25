#ifndef LIBYCRES_INCLUDE_PRO_METHODS_H
#define LIBYCRES_INCLUDE_PRO_METHODS_H

bool yc_res_pro_is_valid_id(uint32_t any_id);

uint16_t yc_res_pro_index_from_object_id(uint32_t any_id);
uint16_t yc_res_pro_index_from_script_id(uint32_t script_id);
uint16_t yc_res_pro_index_from_sprite_id(uint32_t sprite_id);

yc_res_pro_object_type_t yc_res_pro_object_type_from_pid(uint32_t proto_id);
yc_res_pro_object_type_t yc_res_pro_object_type_from_fid(uint32_t sprite_id);
yc_res_pro_script_type_t yc_res_pro_script_type_from_sid(uint32_t script_id);

uint32_t yc_res_pro_fid_from(uint16_t sprite_idx, yc_res_pro_object_type_t type);

typedef struct yc_res_pro_parse_result {
    yc_res_pro_object_t *object;
} yc_res_pro_parse_result_t;

yc_res_pro_status_t yc_res_pro_parse(
        const char *filename,
        const yc_res_io_fs_api_t *api,
        yc_res_pro_parse_result_t *result
);

void yc_res_pro_object_invalidate(yc_res_pro_object_t *object);

#endif //LIBYCRES_INCLUDE_PRO_METHODS_H
