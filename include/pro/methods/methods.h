#ifndef LIB_YCRES_PRO_METHODS_H
#define LIB_YCRES_PRO_METHODS_H

bool yc_res_pro_is_valid_object_id(uint32_t any_id);
uint16_t yc_res_pro_index_from_object_id(uint32_t any_id);

yc_res_pro_object_type_t yc_res_pro_object_type_from_pid(uint32_t proto_id);
yc_res_pro_object_type_t yc_res_pro_object_type_from_fid(uint32_t sprite_id);
yc_res_pro_object_type_t yc_res_pro_object_type_from_sid(uint32_t script_id);

yc_res_pro_status_t yc_res_pro_object_parse(
        const char *filename,
        const yc_res_io_fs_api_t *io,
        yc_res_frm_object_cb_t *callback
);

void yc_res_pro_object_invalidate(yc_res_pro_object_t *object);
void yc_res_pro_object_item_invalidate(yc_res_pro_object_item_t *item);

#endif //LIB_YCRES_PRO_METHODS_H
