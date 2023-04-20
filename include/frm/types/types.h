#ifndef LIB_YCRES_FRM_TYPES_H
#define LIB_YCRES_FRM_TYPES_H

typedef struct yc_res_frm_dimensions {
    uint16_t horizontal, vertical;
} yc_res_frm_dimensions_t;

typedef struct yc_res_frm_shift {
    uint16_t horizontal, vertical;
} yc_res_frm_shift_t;

typedef struct yc_res_frm_texture {
    yc_res_frm_shift_t shift; /// Pixel shift for this particular texture

    unsigned char *indexes;
    yc_res_frm_dimensions_t dimensions;
} yc_res_frm_texture_t;

typedef struct yc_res_frm_animation {
    yc_res_frm_shift_t shift; /// Common shift for all frames within this animation

    size_t count;
    yc_res_frm_texture_t *frames;
} yc_res_frm_animation_t;

typedef struct yc_res_frm_sprite {
    uint16_t fps, keyframe_idx;

    size_t count; /// Unique animations count
    yc_res_frm_animation_t *animations; /// Unique animations list

    size_t orientations[6]; /// Index table: orientation -> animation
} yc_res_frm_sprite_t;

#endif //LIB_YCRES_FRM_TYPES_H
