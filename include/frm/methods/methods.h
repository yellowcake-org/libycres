#ifndef LIBYCRES_INCLUDE_FRM_METHODS_H
#define LIBYCRES_INCLUDE_FRM_METHODS_H

typedef struct yc_res_frm_parse_result {
    yc_res_frm_sprite_t *sprite;
} yc_res_frm_parse_result_t;

yc_res_frm_status_t yc_res_frm_parse(
        const char *filename,
        const yc_res_io_fs_api_t *api,
        yc_res_frm_parse_result_t *result
);

yc_res_frm_status_t yc_res_frm_merge(yc_res_frm_sprite_t **sprites_ptr, size_t count);

void yc_res_frm_sprite_invalidate(yc_res_frm_sprite_t *sprite);

#endif //LIBYCRES_INCLUDE_FRM_METHODS_H
