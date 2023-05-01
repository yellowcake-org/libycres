#ifndef LIB_YCRES_PRO_TYPES_COMMON_LIGHTING_H
#define LIB_YCRES_PRO_TYPES_COMMON_LIGHTING_H

typedef struct yc_res_pro_light_passage {
    bool vertical: 1, horizontal: 1, north: 1, south: 1, east: 1, west: 1;
} yc_res_pro_light_passage_t;

typedef struct yc_res_pro_light_source {
    uint8_t radius: 3; // 0..8 hexes
    uint16_t level;
} yc_res_pro_light_source_t;

#endif //LIB_YCRES_PRO_TYPES_COMMON_LIGHTING_H
