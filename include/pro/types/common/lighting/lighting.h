#ifndef LIB_YCRES_PRO_TYPES_COMMON_LIGHTING_H
#define LIB_YCRES_PRO_TYPES_COMMON_LIGHTING_H

typedef struct yc_res_pro_lighting {
    uint8_t radius: 3; // 0..8 hexes
    uint16_t level;
} yc_res_pro_lighting_t;

#endif //LIB_YCRES_PRO_TYPES_COMMON_LIGHTING_H
