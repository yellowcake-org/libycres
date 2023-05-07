#ifndef LIB_YCRES_MAP_TYPES_LEVEL_OBJECT_PATCH_H
#define LIB_YCRES_MAP_TYPES_LEVEL_OBJECT_PATCH_H

#include "item/item.h"
#include "critter/critter.h"
#include "scenery/scenery.h"
#include "misc/misc.h"

typedef union yc_res_map_level_object_patch {
    yc_res_map_level_object_patch_item_t *item;
    yc_res_map_level_object_patch_critter_t *critter;
    yc_res_map_level_object_patch_scenery_t *scenery;
    yc_res_map_level_object_patch_misc_t *misc;
} yc_res_map_level_object_patch_t;

#endif //LIB_YCRES_MAP_TYPES_LEVEL_OBJECT_PATCH_H
