#ifndef INCLUDE_MAP_TYPES_LEVEL_OBJECT_PATCH_SCENERY_H
#define INCLUDE_MAP_TYPES_LEVEL_OBJECT_PATCH_SCENERY_H

#include "door/door.h"
#include "stairs/stairs.h"
#include "elevator/elevator.h"
#include "ladder/ladder.h"

typedef union yc_res_map_level_object_patch_scenery_data {
    yc_res_map_level_object_patch_scenery_door_t *door;
    yc_res_map_level_object_patch_scenery_stairs_t *stairs;
    yc_res_map_level_object_patch_scenery_elevator_t *elevator;
    yc_res_map_level_object_patch_scenery_ladder_t *ladder;
} yc_res_map_level_object_patch_scenery_data_t;

typedef struct yc_res_map_level_object_patch_scenery {
    yc_res_pro_object_scenery_type_t type;
    yc_res_map_level_object_patch_scenery_data_t data;
} yc_res_map_level_object_patch_scenery_t;

#endif //INCLUDE_MAP_TYPES_LEVEL_OBJECT_PATCH_SCENERY_H
