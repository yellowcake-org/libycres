#include <libycres.h>

bool yc_res_pro_is_valid_id(uint32_t any_id) {
    return any_id != 0xFFFFFFFF;
}

uint16_t yc_res_pro_index_from_object_id(uint32_t any_id) {
    return (any_id & 0x00FFFFFF) & 0xFFF;
}

uint16_t yc_res_pro_index_from_script_id(uint32_t script_id) {
    return (script_id & 0x00FFFFFF) & 0xFFF;
}

uint16_t yc_res_pro_index_from_sprite_id(uint32_t sprite_id) {
    return (sprite_id & 0x00FFFFFF) & 0xFFF;
}
