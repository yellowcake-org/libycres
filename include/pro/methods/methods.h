#ifndef LIB_YCRES_PRO_METHODS_H
#define LIB_YCRES_PRO_METHODS_H

#include "../statuses/statuses.h"
#include "../callbacks/callbacks.h"

uint16_t yc_res_index_from_any_id(uint32_t any_id);

yc_res_entity_t yc_res_entity_from_pid(uint32_t proto_id);
yc_res_entity_t yc_res_entity_from_fid(uint32_t sprite_id);
yc_res_entity_t yc_res_entity_from_sid(uint32_t script_id);

yc_res_pro_status_t yc_res_pro_header_parse(
        const char *filename,
        const yc_res_io_fs_api_t *io,
        yc_res_frm_object_header_cb_t *callback
);

#endif //LIB_YCRES_PRO_METHODS_H
