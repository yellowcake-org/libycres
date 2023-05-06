#ifndef LIB_YCRES_MAP_TYPES_LEVEL_OBJECTS_H
#define LIB_YCRES_MAP_TYPES_LEVEL_OBJECTS_H

typedef struct yc_res_map_level_object {
    uint32_t proto_id, script_id, program_id;
    yc_res_pro_object_flags_t flags;

    uint32_t sprite_id, frame_idx;
    yc_res_math_location_t location;
    yc_res_pro_light_source_t light;

    // screen coordinates and shifts
    int32_t x, y, sx, sy;

    // for in-battle .SAV files
    uint32_t critter_idx, outline_color_idx;

    size_t count;
    struct yc_res_map_level_object *inventory;
} yc_res_map_level_object_t;

typedef struct yc_res_map_level_objects {
    size_t count;
    yc_res_map_level_object_t *pointers;
} yc_res_map_level_objects_t;

#endif //LIB_YCRES_MAP_TYPES_LEVEL_OBJECTS_H
