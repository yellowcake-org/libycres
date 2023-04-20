#include <libycres.h>
#include <stdlib.h>

void yc_res_frm_sprite_invalidate(yc_res_frm_sprite_t *sprite) {
    if (NULL != sprite->animations) {
        for (size_t animations_idx = 0; animations_idx < sprite->count; ++animations_idx) {
            yc_res_frm_animation_t *animation = &sprite->animations[animations_idx];

            if (NULL != animation->frames) {
                for (size_t frames_idx = 0; frames_idx < animation->count; ++frames_idx) {
                    yc_res_frm_texture_t *frame = &animation->frames[frames_idx];

                    if (NULL != frame->pixels) {
                        free(frame->pixels);
                        frame->pixels = NULL;
                    }
                }

                free(animation->frames);
                animation->frames = NULL;
            }
        }

        free(sprite->animations);
        sprite->animations = NULL;
    }
}
