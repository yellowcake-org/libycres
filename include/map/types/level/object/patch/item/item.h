#ifndef LIB_YCRES_MAP_TYPES_LEVEL_OBJECT_PATCH_ITEM_H
#define LIB_YCRES_MAP_TYPES_LEVEL_OBJECT_PATCH_ITEM_H

#include "weapon/weapon.h"
#include "ammo/ammo.h"
#include "misc/misc.h"
#include "key/key.h"

typedef union yc_res_map_level_object_patch_item_data {
    yc_res_map_level_object_patch_item_weapon_t *weapon;
    yc_res_map_level_object_patch_item_ammo_t *ammo;
    yc_res_map_level_object_patch_item_misc_t *misc;
    yc_res_map_level_object_patch_item_key_t *key;
} yc_res_map_level_object_patch_item_data_t;

typedef struct yc_res_map_level_object_patch_item {
    yc_res_pro_object_item_type_t type;
    yc_res_map_level_object_patch_item_data_t data;
} yc_res_map_level_object_patch_item_t;

#endif //LIB_YCRES_MAP_TYPES_LEVEL_OBJECT_PATCH_ITEM_H
