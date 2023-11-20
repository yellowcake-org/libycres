#ifndef LIBYCRES_INCLUDE_FRM_TYPES_H
#define LIBYCRES_INCLUDE_FRM_TYPES_H

typedef struct yc_res_frm_dimensions {
    uint16_t horizontal, vertical;
} yc_res_frm_dimensions_t;

typedef struct yc_res_frm_shift {
    int16_t horizontal, vertical;
} yc_res_frm_shift_t;

typedef struct yc_res_frm_texture {
    unsigned char *pixels; /// Color index table: pixel -> palette value

    yc_res_frm_shift_t shift; /// Pixel shift for this particular texture
    yc_res_frm_dimensions_t dimensions; /// Linear dimensions
} yc_res_frm_texture_t;

typedef struct yc_res_frm_animation {
    uint16_t fps, keyframe_idx;
    yc_res_frm_shift_t shift; /// Common shift for all frames within this animation

    size_t count;
    yc_res_frm_texture_t *frames;
} yc_res_frm_animation_t;

typedef struct yc_res_frm_sprite {
    size_t count; /// Unique animations count
    yc_res_frm_animation_t *animations; /// Unique animations list

    size_t orientations[YC_RES_MATH_ORIENTATION_COUNT]; /// Index table: orientation -> animation number
} yc_res_frm_sprite_t;

#endif //LIBYCRES_INCLUDE_FRM_TYPES_H
