#ifndef LIBYCRES_INCLUDE_PRO_TYPES_OBJECT_ITEM_CONTAINER_H
#define LIBYCRES_INCLUDE_PRO_TYPES_OBJECT_ITEM_CONTAINER_H

#include "flags/flags.h"

typedef struct yc_res_pro_object_item_container {
    uint32_t capacity;
    yc_res_pro_object_item_container_flags_t flags;
} yc_res_pro_object_item_container_t;

#endif //LIBYCRES_INCLUDE_PRO_TYPES_OBJECT_ITEM_CONTAINER_H
