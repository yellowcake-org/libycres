#include <libycres.h>

#include <stdlib.h>
#include <memory.h>

yc_res_frm_status_t yc_res_frm_sprites_merge(yc_res_frm_sprite_t **list_ptr, size_t count) {
    // orientations count is 6, so...
    if (1 > count) { return YC_RES_FRM_STATUS_IN; } // min 2 sprites
    if (6 != count) { return YC_RES_FRM_STATUS_IN; } // max 6 sprites

    yc_res_frm_sprite_t *list = *list_ptr;

    list[0].count += count - 1;
    yc_res_frm_animation_t *grown = realloc(
            list[0].animations,
            list[0].count * sizeof(yc_res_frm_animation_t)
    );

    if (NULL == grown) { return YC_RES_FRM_STATUS_MEM; }
    list[0].animations = grown;

    for (size_t sprite_idx = 1; sprite_idx < count; ++sprite_idx) {
        yc_res_frm_sprite_t current = list[sprite_idx];

        if (1 != current.count) { return YC_RES_FRM_STATUS_IN; }

        list[0].orientations[sprite_idx] = sprite_idx;
        list[0].animations[sprite_idx] = current.animations[0];

        current.animations = NULL;
        yc_res_frm_sprite_invalidate(&current);
    }

    yc_res_frm_sprite_t *shrunk = realloc(*list_ptr, sizeof(yc_res_frm_sprite_t));
    if (NULL == shrunk) { return YC_RES_FRM_STATUS_MEM; }

    *list_ptr = shrunk;
    return YC_RES_FRM_STATUS_OK;
}
