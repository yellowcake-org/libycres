#include <libycres.h>

#include <stdlib.h>

void yc_res_pro_object_invalidate(yc_res_pro_object_t *object) {
    if (NULL != object->data.item) {
        free(object->data.item);
    }
}

void yc_res_pro_object_item_invalidate(yc_res_pro_object_item_t *item) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-value"
    item; // TODO: free data
#pragma clang diagnostic pop
}
