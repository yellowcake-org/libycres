#ifndef LIBYCRES_INCLUDE_MAP_TYPES_LEVEL_OBJECT_H
#define LIBYCRES_INCLUDE_MAP_TYPES_LEVEL_OBJECT_H

#include "patch/patch.h"

typedef struct yc_res_map_level_object {
    uint32_t proto_id, script_id, program_id;

    yc_res_pro_object_flags_t flags;
    yc_res_map_level_object_patch_t patch;

    uint32_t sprite_id, frame_idx;
    yc_res_math_location_t location;
    yc_res_pro_light_source_t light;

    int32_t screen_x, screen_y;
    uint32_t correction_x, correction_y;

    // for in-battle .SAV files
    uint32_t critter_idx, outline_color_idx;

    size_t capacity, occupied;
    struct yc_res_map_level_object **inventory;
} yc_res_map_level_object_t;

typedef struct yc_res_map_level_objects {
    size_t count;
    yc_res_map_level_object_t *pointers;
} yc_res_map_level_objects_t;

#endif //LIBYCRES_INCLUDE_MAP_TYPES_LEVEL_OBJECT_H
