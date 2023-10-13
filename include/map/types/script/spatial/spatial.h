#ifndef LIBYCRES_INCLUDE_MAP_TYPES_SCRIPT_SPATIAL_H
#define LIBYCRES_INCLUDE_MAP_TYPES_SCRIPT_SPATIAL_H

typedef struct yc_res_map_script_spatial {
    uint32_t hex_idx, distance;
    yc_res_map_elevation_t elevation;
} yc_res_map_script_spatial_t;

#endif //LIBYCRES_INCLUDE_MAP_TYPES_SCRIPT_SPATIAL_H
