#ifndef LIBYCRES_INCLUDE_PRO_TYPES_OBJECT_SCENERY_ELEVATOR_H
#define LIBYCRES_INCLUDE_PRO_TYPES_OBJECT_SCENERY_ELEVATOR_H

typedef enum yc_res_pro_object_scenery_elevator_type {
    YC_RES_PRO_OBJECT_SCENERY_ELEVATOR_TYPE_BROTHERHOOD_OF_STEEL_MAIN,
    YC_RES_PRO_OBJECT_SCENERY_ELEVATOR_TYPE_BROTHERHOOD_OF_STEEL_SURFACE,
    YC_RES_PRO_OBJECT_SCENERY_ELEVATOR_TYPE_MASTER_UPPER,
    YC_RES_PRO_OBJECT_SCENERY_ELEVATOR_TYPE_MASTER_LOWER,
    YC_RES_PRO_OBJECT_SCENERY_ELEVATOR_TYPE_MILITARY_BASE_UPPER,
    YC_RES_PRO_OBJECT_SCENERY_ELEVATOR_TYPE_MILITARY_BASE_LOWER,
    YC_RES_PRO_OBJECT_SCENERY_ELEVATOR_TYPE_GLOW_UPPER,
    YC_RES_PRO_OBJECT_SCENERY_ELEVATOR_TYPE_GLOW_LOWER,
    YC_RES_PRO_OBJECT_SCENERY_ELEVATOR_TYPE_VAULT_13,
    YC_RES_PRO_OBJECT_SCENERY_ELEVATOR_TYPE_NECROPOLIS,
    YC_RES_PRO_OBJECT_SCENERY_ELEVATOR_TYPE_SIERRA_1,
    YC_RES_PRO_OBJECT_SCENERY_ELEVATOR_TYPE_SIERRA_2,
} yc_res_pro_object_scenery_elevator_type_t;

typedef struct yc_res_pro_object_scenery_elevator {
    uint8_t elevation;
    yc_res_pro_object_scenery_elevator_type_t type;
} yc_res_pro_object_scenery_elevator_t;

#endif //LIBYCRES_INCLUDE_PRO_TYPES_OBJECT_SCENERY_ELEVATOR_H
