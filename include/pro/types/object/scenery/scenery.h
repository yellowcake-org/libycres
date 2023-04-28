#ifndef LIB_YCRES_PRO_TYPES_OBJECT_SCENERY_H
#define LIB_YCRES_PRO_TYPES_OBJECT_SCENERY_H

typedef enum yc_res_pro_object_scenery_type {
    YC_RES_PRO_OBJECT_SCENERY_TYPE_DOOR,
    YC_RES_PRO_OBJECT_SCENERY_TYPE_STAIRS,
    YC_RES_PRO_OBJECT_SCENERY_TYPE_ELEVATOR,
    YC_RES_PRO_OBJECT_SCENERY_TYPE_LADDER_BOTTOM,
    YC_RES_PRO_OBJECT_SCENERY_TYPE_LADDER_TOP,
    YC_RES_PRO_OBJECT_SCENERY_TYPE_GENERIC,
} yc_res_pro_object_scenery_type_t;

//typedef union yc_res_pro_object_scenery_data {
//
//} yc_res_pro_object_scenery_data_t;

uint8_t yc_res_pro_object_scenery_rotation_from_id(uint32_t destination_id);
uint8_t yc_res_pro_object_scenery_elevation_from_id(uint32_t destination_id);
uint16_t yc_res_pro_object_scenery_tile_from_id(uint32_t destination_id);

typedef struct yc_res_pro_object_scenery {
    uint64_t script_id;
    unsigned char sound_id;

    yc_res_pro_material_t material;
    yc_res_pro_action_flags_t flags;
    yc_res_pro_light_passage_t light;

    yc_res_pro_object_scenery_type_t type;
    // data
} yc_res_pro_object_scenery_t;

#endif //LIB_YCRES_PRO_TYPES_OBJECT_SCENERY_H
