#ifndef LIB_YCRES_PRO_TYPES_OBJECT_SCENERY_H
#define LIB_YCRES_PRO_TYPES_OBJECT_SCENERY_H

#include "door/door.h"
#include "stairs/stairs.h"
#include "elevator/elevator.h"
#include "ladder/ladder.h"

typedef enum yc_res_pro_object_scenery_type {
    YC_RES_PRO_OBJECT_SCENERY_TYPE_DOOR,
    YC_RES_PRO_OBJECT_SCENERY_TYPE_STAIRS,
    YC_RES_PRO_OBJECT_SCENERY_TYPE_ELEVATOR,
    YC_RES_PRO_OBJECT_SCENERY_TYPE_LADDER_BOTTOM,
    YC_RES_PRO_OBJECT_SCENERY_TYPE_LADDER_TOP,
    YC_RES_PRO_OBJECT_SCENERY_TYPE_GENERIC,
} yc_res_pro_object_scenery_type_t;

typedef union yc_res_pro_object_scenery_data {
    yc_res_pro_object_scenery_door_t *door;
    yc_res_pro_object_scenery_stairs_t *stairs;
    yc_res_pro_object_scenery_elevator_t *elevator;
    yc_res_pro_object_scenery_ladder_t *ladder_bottom, *ladder_top;
} yc_res_pro_object_scenery_data_t;

typedef struct yc_res_pro_object_scenery {
    uint64_t script_id;
    unsigned char sound_id;

    yc_res_pro_material_t material;
    yc_res_pro_action_flags_t flags;
    yc_res_pro_light_passage_t light;

    yc_res_pro_object_scenery_type_t type;
    yc_res_pro_object_scenery_data_t data;
} yc_res_pro_object_scenery_t;

#endif //LIB_YCRES_PRO_TYPES_OBJECT_SCENERY_H
