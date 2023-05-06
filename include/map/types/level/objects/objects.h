#ifndef LIB_YCRES_MAP_TYPES_LEVEL_OBJECTS_H
#define LIB_YCRES_MAP_TYPES_LEVEL_OBJECTS_H

typedef struct yc_res_map_level_object {
    uint32_t hex_idx;
} yc_res_map_level_object_t;

typedef struct yc_res_map_level_objects {
    size_t count;
    yc_res_map_level_object_t *pointers;
} yc_res_map_level_objects_t;

#endif //LIB_YCRES_MAP_TYPES_LEVEL_OBJECTS_H
