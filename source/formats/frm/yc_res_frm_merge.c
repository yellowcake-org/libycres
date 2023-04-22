#include <libycres.h>
#include <stdlib.h>
#include <memory.h>

yc_res_frm_status_t yc_res_frm_sprites_merge(yc_res_frm_sprite_t **list_ptr, size_t count) {
    // orientations count is 6, so...
    if (1 > count) { return YC_RES_FRM_STATUS_IN; } // min 2 sprites
    if (6 != count) { return YC_RES_FRM_STATUS_IN; } // max 6 sprites

    yc_res_frm_sprite_t *result = *list_ptr;

    result->count += count - 1;
    yc_res_frm_animation_t *grown = realloc(
            result->animations,
            result->count * sizeof(yc_res_frm_animation_t)
    );

    if (NULL == grown) { return YC_RES_FRM_STATUS_MEM; }
    result->animations = grown;

    for (size_t sprite_idx = 1; sprite_idx < count; ++sprite_idx) {
        yc_res_frm_sprite_t *current = list_ptr[sprite_idx];

        if (1 != current->count) { return YC_RES_FRM_STATUS_IN; }

        result->orientations[sprite_idx] = sprite_idx;
        result->animations[sprite_idx] = current->animations[0];

        current->animations = NULL;
        yc_res_frm_sprite_invalidate(current);
    }

    result = realloc(*list_ptr, sizeof(yc_res_frm_sprite_t));
    if (NULL == result) { return YC_RES_FRM_STATUS_MEM; }

    *list_ptr = result;
    return YC_RES_FRM_STATUS_OK;
}
