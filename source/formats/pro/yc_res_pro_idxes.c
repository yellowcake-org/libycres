#include <libycres.h>

uint32_t yc_res_pro_fid_from(uint16_t sprite_idx, yc_res_pro_object_type_t type) {
    return (type << 24) | sprite_idx;
}
