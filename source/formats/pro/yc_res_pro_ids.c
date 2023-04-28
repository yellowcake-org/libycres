#include <libycres.h>

bool yc_res_pro_is_valid_object_id(uint32_t any_id) {
    return any_id != 0xFFFFFFFF;
}

uint16_t yc_res_pro_index_from_object_id(uint32_t any_id) {
    return any_id & 0x00FFFFFF;
}

bool yc_res_pro_is_valid_script_id(uint32_t script_id) {
    return script_id != 0xFFFFFFFF;
}

uint16_t yc_res_pro_index_from_script_id(uint32_t script_id) {
    return script_id & 0x00FFFFFF;
}
