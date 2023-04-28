#ifndef LIB_YCRES_PRO_TYPES_OBJECT_H
#define LIB_YCRES_PRO_TYPES_OBJECT_H

#include "flags/flags.h"

#include "item/item.h"
#include "critter/critter.h"
#include "scenery/scenery.h"
#include "wall/wall.h"
#include "tile/tile.h"

typedef enum yc_res_pro_object_type {
    YC_RES_PRO_OBJECT_TYPE_ITEM = 0,
    YC_RES_PRO_OBJECT_TYPE_CRITTER,
    YC_RES_PRO_OBJECT_TYPE_SCENERY,
    YC_RES_PRO_OBJECT_TYPE_WALL,
    YC_RES_PRO_OBJECT_TYPE_TILE,
    YC_RES_PRO_OBJECT_TYPE_MISC,
    YC_RES_PRO_OBJECT_TYPE_INTERFACE,
    YC_RES_PRO_OBJECT_TYPE_INVENTORY,
    YC_RES_PRO_OBJECT_TYPE_HEAD,
    YC_RES_PRO_OBJECT_TYPE_BACKGROUND
} yc_res_pro_object_type_t;

typedef union yc_res_pro_object_data {
    yc_res_pro_object_item_t *item;
    yc_res_pro_object_critter_t *critter;
    yc_res_pro_object_scenery_t *scenery;
    yc_res_pro_object_wall_t *wall;
    yc_res_pro_object_tile_t *tile;
} yc_res_pro_object_data_t;

typedef struct yc_res_pro_object {
    uint32_t proto_id, text_id, sprite_id;
    yc_res_pro_object_data_t data;

    yc_res_pro_light_source_t lighting;
    yc_res_pro_object_flags_t flags;
} yc_res_pro_object_t;

#endif //LIB_YCRES_PRO_TYPES_OBJECT_H
