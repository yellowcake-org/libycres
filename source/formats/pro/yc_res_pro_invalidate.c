#include <libycres.h>

#include <stdlib.h>

void yc_res_pro_object_invalidate(yc_res_pro_object_t *object) {
    if (NULL != object->data.item) {
        if (yc_res_pro_type_from_pid(object->proto_id) == YC_RES_PRO_OBJECT_TYPE_ITEM) {
            yc_res_pro_object_item_invalidate(object->data.item);
        }

        free(object->data.item);
        object->data.item = NULL;
    }
}

void yc_res_pro_object_item_invalidate(yc_res_pro_object_item_t *item) {
    switch (item->type) {
        case YC_RES_PRO_OBJECT_ITEM_DRUG: {
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
}
