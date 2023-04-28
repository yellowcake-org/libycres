#include <libycres.h>

yc_res_pro_object_type_t yc_res_pro_object_type_from_pid(uint32_t proto_id) {
    return proto_id >> 24;
}

yc_res_pro_object_type_t yc_res_pro_object_type_from_fid(uint32_t sprite_id) {
    return (sprite_id & 0xF000000) >> 24;
}

yc_res_pro_script_type_t yc_res_pro_script_type_from_sid(uint32_t script_id) {
    return (script_id & 0xFF00000) >> 24;
}
