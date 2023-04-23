#ifndef LIB_YCRES_PRO_TYPES_H
#define LIB_YCRES_PRO_TYPES_H

#include "object/flags.h"
#include "object/entity.h"
#include "object/lighting.h"

typedef struct yc_res_pro_object_header {
    uint32_t proto_id, text_id, sprite_id;

    yc_res_pro_flags_t flags;
    yc_res_pro_lighting_t lighting;
} yc_res_pro_object_header_t;

#endif //LIB_YCRES_PRO_TYPES_H
