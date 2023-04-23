#ifndef LIB_YCRES_PRO_TYPES_OBJECT_ITEM_H
#define LIB_YCRES_PRO_TYPES_OBJECT_ITEM_H

#include "../../material/material.h"

#include "flags/flags.h"
#include "attacks/attacks.h"

typedef enum yc_res_pro_object_item_type {
    YC_RES_PRO_OBJECT_ITEM_ARMOR = 0,
    YC_RES_PRO_OBJECT_ITEM_CONTAINER,
    YC_RES_PRO_OBJECT_ITEM_DRUG,
    YC_RES_PRO_OBJECT_ITEM_AMMO,
    YC_RES_PRO_OBJECT_ITEM_MISC,
    YC_RES_PRO_OBJECT_ITEM_KEY,
} yc_res_pro_object_item_type_t;

typedef struct yc_res_pro_object_item {
    uint32_t sprite_id, script_id;
    unsigned char sound_id;

    uint32_t volume, weight, cost;
    yc_res_pro_material_t material;

    yc_res_pro_item_flags_t flags;
    yc_res_pro_item_attack_t primary, secondary;

    yc_res_pro_object_item_type_t type;
    // TODO: union w/ data
} yc_res_pro_object_item_t;

#endif //LIB_YCRES_PRO_TYPES_OBJECT_ITEM_H
