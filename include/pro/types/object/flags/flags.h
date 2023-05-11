#ifndef INCLUDE_PRO_TYPES_OBJECT_FLAGS_H
#define INCLUDE_PRO_TYPES_OBJECT_FLAGS_H

typedef enum yc_res_pro_object_transparency {
    YC_RES_PRO_TRANS_NONE,
    YC_RES_PRO_TRANS_RED,
    YC_RES_PRO_TRANS_WALL,
    YC_RES_PRO_TRANS_GLASS,
    YC_RES_PRO_TRANS_STEAM,
    YC_RES_PRO_TRANS_ENERGY,
    YC_RES_PRO_TRANS_WALL_END,
} yc_res_pro_object_transparency_t;

typedef struct yc_res_pro_object_flags {
    bool is_flat: 1, multi_hex: 1;

    bool no_block: 1, no_border: 1;
    bool light_through: 1, shoot_through: 1;

    yc_res_pro_object_transparency_t transparency;
} yc_res_pro_object_flags_t;

#endif //INCLUDE_PRO_TYPES_OBJECT_FLAGS_H
