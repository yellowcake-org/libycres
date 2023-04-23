#include <libycres.h>

bool yc_res_pro_is_valid_id(uint32_t any_id) {
    return any_id != 0xFFFFFFFF;
}

uint16_t yc_res_pro_index_from_id(uint32_t any_id) {
    return any_id & 0x00FFFFFF;
}
