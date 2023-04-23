#ifndef LIB_YCRES_PRO_TYPES_FLAGS_H
#define LIB_YCRES_PRO_TYPES_FLAGS_H

typedef enum yc_res_pro_transparency {
    YC_RES_PRO_TRANS_NONE,
    YC_RES_PRO_TRANS_RED,
    YC_RES_PRO_TRANS_WALL,
    YC_RES_PRO_TRANS_GLASS,
    YC_RES_PRO_TRANS_STEAM,
    YC_RES_PRO_TRANS_ENERGY,
    YC_RES_PRO_TRANS_WALL_END,
} yc_res_pro_transparency_t;

typedef struct yc_res_pro_flags {
    bool multi_hex;

    bool no_block, no_border;
    bool light_through, shoot_through;

    bool is_flat, is_hidden;
    bool is_locked, is_jammed;

    yc_res_pro_transparency_t transparency;
} yc_res_pro_flags_t;

#endif //LIB_YCRES_PRO_TYPES_FLAGS_H
