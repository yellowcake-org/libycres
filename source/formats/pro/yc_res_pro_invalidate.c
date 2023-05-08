#include <libycres.h>

#include <stdlib.h>

void yc_res_pro_object_invalidate_data(yc_res_pro_object_t *object);

void yc_res_pro_object_item_invalidate(yc_res_pro_object_item_t *item);

void yc_res_pro_object_scenery_invalidate(yc_res_pro_object_scenery_t *scenery);

void yc_res_pro_object_invalidate(yc_res_pro_object_t *object) {
    yc_res_pro_object_invalidate_data(object);

    if (YC_RES_PRO_OBJECT_TYPE_ITEM == yc_res_pro_object_type_from_pid(object->proto_id) && NULL != object->data.item) {
        free(object->data.item);
        object->data.item = NULL;
    }

    if (YC_RES_PRO_OBJECT_TYPE_CRITTER == yc_res_pro_object_type_from_pid(object->proto_id) &&
        NULL != object->data.critter) {
        free(object->data.critter);
        object->data.critter = NULL;
    }

    if (YC_RES_PRO_OBJECT_TYPE_SCENERY == yc_res_pro_object_type_from_pid(object->proto_id) &&
        NULL != object->data.scenery) {
        free(object->data.scenery);
        object->data.scenery = NULL;
    }

    if (YC_RES_PRO_OBJECT_TYPE_WALL == yc_res_pro_object_type_from_pid(object->proto_id) && NULL != object->data.wall) {
        free(object->data.wall);
        object->data.wall = NULL;
    }

    if (YC_RES_PRO_OBJECT_TYPE_TILE == yc_res_pro_object_type_from_pid(object->proto_id) && NULL != object->data.tile) {
        free(object->data.tile);
        object->data.tile = NULL;
    }

    if (YC_RES_PRO_OBJECT_TYPE_MISC == yc_res_pro_object_type_from_pid(object->proto_id) && NULL != object->data.misc) {
        free(object->data.misc);
        object->data.misc = NULL;
    }
}

void yc_res_pro_object_invalidate_data(yc_res_pro_object_t *object) {
    switch (yc_res_pro_object_type_from_pid(object->proto_id)) {
        case YC_RES_PRO_OBJECT_TYPE_ITEM:
            if (NULL != object->data.item) { yc_res_pro_object_item_invalidate(object->data.item); }
            break;
        case YC_RES_PRO_OBJECT_TYPE_SCENERY:
            if (NULL != object->data.scenery) { yc_res_pro_object_scenery_invalidate(object->data.scenery); }
        default:
            break;
    }
}

void yc_res_pro_object_item_invalidate(yc_res_pro_object_item_t *item) {
    if (YC_RES_PRO_OBJECT_ITEM_TYPE_ARMOR == item->type && NULL != item->data.armor) {
        free(item->data.armor);
        item->data.armor = NULL;
    }

    if (YC_RES_PRO_OBJECT_ITEM_TYPE_CONTAINER == item->type && NULL != item->data.container) {
        free(item->data.container);
        item->data.container = NULL;
    }

    if (YC_RES_PRO_OBJECT_ITEM_TYPE_DRUG == item->type && NULL != item->data.drug) {
        free(item->data.drug);
        item->data.drug = NULL;
    }

    if (YC_RES_PRO_OBJECT_ITEM_TYPE_WEAPON == item->type && NULL != item->data.weapon) {
        free(item->data.weapon);
        item->data.weapon = NULL;
    }

    if (YC_RES_PRO_OBJECT_ITEM_TYPE_AMMO == item->type && NULL != item->data.ammo) {
        free(item->data.ammo);
        item->data.ammo = NULL;
    }

    if (YC_RES_PRO_OBJECT_ITEM_TYPE_MISC == item->type && NULL != item->data.misc) {
        free(item->data.misc);
        item->data.misc = NULL;
    }

    if (YC_RES_PRO_OBJECT_ITEM_TYPE_KEY == item->type && NULL != item->data.key) {
        free(item->data.key);
        item->data.key = NULL;
    }
}

void yc_res_pro_object_scenery_invalidate(yc_res_pro_object_scenery_t *scenery) {
    if (YC_RES_PRO_OBJECT_SCENERY_TYPE_DOOR == scenery->type && NULL != scenery->data.door) {
        free(scenery->data.door);
        scenery->data.door = NULL;
    }

    if (YC_RES_PRO_OBJECT_SCENERY_TYPE_STAIRS == scenery->type && NULL != scenery->data.stairs) {
        free(scenery->data.stairs);
        scenery->data.stairs = NULL;
    }

    if (YC_RES_PRO_OBJECT_SCENERY_TYPE_ELEVATOR == scenery->type && NULL != scenery->data.elevator) {
        free(scenery->data.elevator);
        scenery->data.elevator = NULL;
    }

    if ((YC_RES_PRO_OBJECT_SCENERY_TYPE_LADDER_BOTTOM == scenery->type
         || YC_RES_PRO_OBJECT_SCENERY_TYPE_LADDER_TOP == scenery->type) && NULL != scenery->data.ladder) {
        free(scenery->data.ladder);
        scenery->data.ladder = NULL;
    }

    if (YC_RES_PRO_OBJECT_SCENERY_TYPE_GENERIC == scenery->type && NULL != scenery->data.generic) {
        free(scenery->data.generic);
        scenery->data.generic = NULL;
    }
}

