#ifndef LIB_YCRES_PRO_TYPES_OBJECT_ITEM_H
#define LIB_YCRES_PRO_TYPES_OBJECT_ITEM_H

#include "flags/flags.h"
#include "attacks/attacks.h"

#include "armor/armor.h"
#include "container/container.h"
#include "drug/drug.h"
#include "weapon/weapon.h"
#include "ammo/ammo.h"
#include "misc/misc.h"
#include "key/key.h"

typedef enum yc_res_pro_object_item_type {
    YC_RES_PRO_OBJECT_ITEM_TYPE_ARMOR = 0,
    YC_RES_PRO_OBJECT_ITEM_TYPE_CONTAINER,
    YC_RES_PRO_OBJECT_ITEM_TYPE_DRUG,
    YC_RES_PRO_OBJECT_ITEM_TYPE_WEAPON,
    YC_RES_PRO_OBJECT_ITEM_TYPE_AMMO,
    YC_RES_PRO_OBJECT_ITEM_TYPE_MISC,
    YC_RES_PRO_OBJECT_ITEM_TYPE_KEY,
} yc_res_pro_object_item_type_t;

typedef union yc_res_pro_object_item_data {
    yc_res_pro_object_item_armor_t *armor;
    yc_res_pro_object_item_container_t *container;
    yc_res_pro_object_item_drug_t *drug;
    yc_res_pro_object_item_weapon_t *weapon;
    yc_res_pro_object_item_ammo_t *ammo;
    yc_res_pro_object_item_misc_t *misc;
} yc_res_pro_object_item_data_t;

typedef struct yc_res_pro_object_item {
    uint32_t sprite_id, script_id;
    unsigned char sound_id;

    uint32_t volume, weight, cost;
    yc_res_pro_material_t material;

    yc_res_pro_item_flags_t flags;
    yc_res_pro_object_item_attack_t primary, secondary;

    yc_res_pro_object_item_type_t type;
    yc_res_pro_object_item_data_t data;
} yc_res_pro_object_item_t;

#endif //LIB_YCRES_PRO_TYPES_OBJECT_ITEM_H
