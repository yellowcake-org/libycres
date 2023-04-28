#include <libycres.h>

#include <stdlib.h>

void yc_res_pro_object_invalidate_data(yc_res_pro_object_t *object);

void yc_res_pro_object_item_invalidate(yc_res_pro_object_item_t *item);
void yc_res_pro_object_critter_invalidate(yc_res_pro_object_critter_t *critter);

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
}

void yc_res_pro_object_invalidate_data(yc_res_pro_object_t *object) {
    switch (yc_res_pro_object_type_from_pid(object->proto_id)) {
        case YC_RES_PRO_OBJECT_TYPE_ITEM:
            if (NULL != object->data.item) { yc_res_pro_object_item_invalidate(object->data.item); }
            break;
        case YC_RES_PRO_OBJECT_TYPE_CRITTER:
            if (NULL != object->data.critter) { yc_res_pro_object_critter_invalidate(object->data.critter); }
            break;
        default:
            break;
    }
}

void yc_res_pro_object_item_invalidate(yc_res_pro_object_item_t *item) {
    switch (item->type) {
        case YC_RES_PRO_OBJECT_ITEM_TYPE_DRUG: {
            if (NULL != item->data.drug) {
                item->data.drug->count = 0;

                free(item->data.drug->effects);
                item->data.drug->effects = NULL;
            }
        }
            break;
        default:
            break;
    }

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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void yc_res_pro_object_critter_invalidate(yc_res_pro_object_critter_t *critter) {
    //
}
#pragma clang diagnostic pop
