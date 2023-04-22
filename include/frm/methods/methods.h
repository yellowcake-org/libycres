#ifndef LIB_YCRES_FRM_METHODS_H
#define LIB_YCRES_FRM_METHODS_H

#include "../statuses/statuses.h"
#include "../callbacks/callbacks.h"

yc_res_frm_status_t yc_res_frm_sprite_parse(
        const char *filename,
        yc_res_io_fs_api_t *io,
        yc_res_frm_sprite_cb_t *callback
);

yc_res_frm_status_t yc_res_frm_sprites_merge(yc_res_frm_sprite_t **sprites_ptr, size_t count);

void yc_res_frm_sprite_invalidate(yc_res_frm_sprite_t *sprite);

#endif //LIB_YCRES_FRM_METHODS_H
