#include <libycres.h>

#include <stdlib.h>

void yc_res_pro_object_invalidate_data(yc_res_pro_object_t *object);

void yc_res_pro_object_item_invalidate(yc_res_pro_object_item_t *item);

void yc_res_pro_object_scenery_invalidate(yc_res_pro_object_scenery_t *scenery);

void yc_res_pro_object_invalidate(yc_res_pro_object_t *object) {
    yc_res_pro_object_invalidate_data(object);

    if (NULL != object->data.item
        && YC_RES_PRO_OBJECT_TYPE_ITEM == yc_res_pro_object_type_from_pid(object->proto_id)) {
        free(object->data.item);
        object->data.item = NULL;
    }

    if (NULL != object->data.critter
        && YC_RES_PRO_OBJECT_TYPE_CRITTER == yc_res_pro_object_type_from_pid(object->proto_id)) {
        free(object->data.critter);
        object->data.critter = NULL;
    }

    if (NULL != object->data.scenery
        && YC_RES_PRO_OBJECT_TYPE_SCENERY == yc_res_pro_object_type_from_pid(object->proto_id)) {
        free(object->data.scenery);
        object->data.scenery = NULL;
    }

    if (NULL != object->data.wall
        && YC_RES_PRO_OBJECT_TYPE_WALL == yc_res_pro_object_type_from_pid(object->proto_id)) {
        free(object->data.wall);
        object->data.wall = NULL;
    }

    if (NULL != object->data.tile
        && YC_RES_PRO_OBJECT_TYPE_TILE == yc_res_pro_object_type_from_pid(object->proto_id)) {
        free(object->data.tile);
        object->data.tile = NULL;
    }

    if (NULL != object->data.misc
        && YC_RES_PRO_OBJECT_TYPE_MISC == yc_res_pro_object_type_from_pid(object->proto_id)) {
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
    if (NULL != item->data.armor && YC_RES_PRO_OBJECT_ITEM_TYPE_ARMOR == item->type) {
        free(item->data.armor);
        item->data.armor = NULL;
    }

    if (NULL != item->data.container && YC_RES_PRO_OBJECT_ITEM_TYPE_CONTAINER == item->type) {
        free(item->data.container);
        item->data.container = NULL;
    }

    if (NULL != item->data.drug && YC_RES_PRO_OBJECT_ITEM_TYPE_DRUG == item->type) {
        free(item->data.drug);
        item->data.drug = NULL;
    }

    if (NULL != item->data.weapon && YC_RES_PRO_OBJECT_ITEM_TYPE_WEAPON == item->type) {
        free(item->data.weapon);
        item->data.weapon = NULL;
    }

    if (NULL != item->data.ammo && YC_RES_PRO_OBJECT_ITEM_TYPE_AMMO == item->type) {
        free(item->data.ammo);
        item->data.ammo = NULL;
    }

    if (NULL != item->data.misc && YC_RES_PRO_OBJECT_ITEM_TYPE_MISC == item->type) {
        free(item->data.misc);
        item->data.misc = NULL;
    }

    if (NULL != item->data.key && YC_RES_PRO_OBJECT_ITEM_TYPE_KEY == item->type) {
        free(item->data.key);
        item->data.key = NULL;
    }
}

void yc_res_pro_object_scenery_invalidate(yc_res_pro_object_scenery_t *scenery) {
    if (NULL != scenery->data.door && YC_RES_PRO_OBJECT_SCENERY_TYPE_DOOR == scenery->type) {
        free(scenery->data.door);
        scenery->data.door = NULL;
    }

    if (NULL != scenery->data.stairs && YC_RES_PRO_OBJECT_SCENERY_TYPE_STAIRS == scenery->type) {
        free(scenery->data.stairs);
        scenery->data.stairs = NULL;
    }

    if (NULL != scenery->data.elevator && YC_RES_PRO_OBJECT_SCENERY_TYPE_ELEVATOR == scenery->type) {
        free(scenery->data.elevator);
        scenery->data.elevator = NULL;
    }

    if (NULL != scenery->data.ladder
        && (YC_RES_PRO_OBJECT_SCENERY_TYPE_LADDER_BOTTOM == scenery->type
            || YC_RES_PRO_OBJECT_SCENERY_TYPE_LADDER_TOP == scenery->type)) {
        free(scenery->data.ladder);
        scenery->data.ladder = NULL;
    }

    if (NULL != scenery->data.generic && YC_RES_PRO_OBJECT_SCENERY_TYPE_GENERIC == scenery->type) {
        free(scenery->data.generic);
        scenery->data.generic = NULL;
    }
}

