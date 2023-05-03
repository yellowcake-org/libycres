#ifndef LIB_YCRES_PRO_TYPES_COMMON_DESTINATION_TYPE_H
#define LIB_YCRES_PRO_TYPES_COMMON_DESTINATION_TYPE_H

// TODO: Move out (geometry)
typedef struct yc_res_pro_destination {
    uint16_t tile_idx;

    uint8_t orientation;
    uint8_t elevation_idx;
} yc_res_pro_destination_t;

#endif //LIB_YCRES_PRO_TYPES_COMMON_DESTINATION_TYPE_H
