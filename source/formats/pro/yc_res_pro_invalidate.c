#include <libycres.h>

#include <stdlib.h>

void yc_res_pro_object_invalidate_data(yc_res_pro_object_t *object);

void yc_res_pro_object_item_invalidate(yc_res_pro_object_item_t *item);
void yc_res_pro_object_scenery_invalidate(yc_res_pro_object_scenery_t *scenery);

void yc_res_pro_object_invalidate(yc_res_pro_object_t *object) {
    yc_res_pro_object_invalidate_data(object);

    if (NULL != object->data.item) {
        free(object->data.item);
        object->data.item = NULL;
    }

    if (NULL != object->data.critter) {
        free(object->data.critter);
        object->data.critter = NULL;
    }

    if (NULL != object->data.scenery) {
        free(object->data.scenery);
        object->data.scenery = NULL;
    }

    if (NULL != object->data.wall) {
        free(object->data.wall);
        object->data.wall = NULL;
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
    if (NULL != item->data.armor) {
        free(item->data.armor);
        item->data.armor = NULL;
    }

    if (NULL != item->data.container) {
        free(item->data.container);
        item->data.container = NULL;
    }

    if (NULL != item->data.drug) {
        free(item->data.drug);
        item->data.drug = NULL;
    }

    if (NULL != item->data.weapon) {
        free(item->data.weapon);
        item->data.weapon = NULL;
    }

    if (NULL != item->data.ammo) {
        free(item->data.ammo);
        item->data.ammo = NULL;
    }

    if (NULL != item->data.misc) {
        free(item->data.misc);
        item->data.misc = NULL;
    }

    if (NULL != item->data.key) {
        free(item->data.key);
        item->data.key = NULL;
    }
}

void yc_res_pro_object_scenery_invalidate(yc_res_pro_object_scenery_t *scenery) {
    if (NULL != scenery->data.door) {
        free(scenery->data.door);
        scenery->data.door = NULL;
    }

    if (NULL != scenery->data.stairs) {
        free(scenery->data.stairs);
        scenery->data.stairs = NULL;
    }

    if (NULL != scenery->data.elevator) {
        free(scenery->data.elevator);
        scenery->data.elevator = NULL;
    }

    if (NULL != scenery->data.ladder_bottom) {
        free(scenery->data.ladder_bottom);
        scenery->data.ladder_bottom = NULL;
    }

    if (NULL != scenery->data.ladder_top) {
        free(scenery->data.ladder_top);
        scenery->data.ladder_top = NULL;
    }

    if (NULL != scenery->data.generic) {
        free(scenery->data.generic);
        scenery->data.generic = NULL;
    }
}

