#include <libycres.h>

bool yc_res_pro_is_valid_destination(yc_res_pro_destination_t destination) {
    bool is_elevation_valid = destination.elevation_idx >= 0 && destination.elevation_idx < 3;
    bool is_orientation_valid = destination.orientation >= 0 && destination.orientation <= 6;

    return is_elevation_valid && is_orientation_valid;
}

yc_res_pro_object_type_t yc_res_pro_object_type_from_pid(uint32_t proto_id) {
    return proto_id >> 24;
}

yc_res_pro_object_type_t yc_res_pro_object_type_from_fid(uint32_t sprite_id) {
    return (sprite_id & 0xF000000) >> 24;
}

yc_res_pro_script_type_t yc_res_pro_script_type_from_sid(uint32_t script_id) {
    return script_id >> 24;
}
