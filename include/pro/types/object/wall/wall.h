#ifndef INCLUDE_PRO_TYPES_OBJECT_WALL_H
#define INCLUDE_PRO_TYPES_OBJECT_WALL_H

typedef struct yc_res_pro_object_wall {
    uint32_t script_id;

    yc_res_pro_material_t material;
    yc_res_pro_action_flags_t flags;
    yc_res_pro_light_passage_t light;
} yc_res_pro_object_wall_t;

#endif //INCLUDE_PRO_TYPES_OBJECT_WALL_H
