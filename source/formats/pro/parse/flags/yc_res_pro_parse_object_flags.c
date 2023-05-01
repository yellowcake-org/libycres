#include <libycres.h>

void yc_res_pro_parse_action_flags(unsigned char flags, yc_res_pro_action_flags_t *into) {
    into->knees_down = (flags & 0x01) == 0x01;
    into->can_be_used = (flags & 0x08) == 0x08;
    into->can_be_used_on = (flags & 0x10) == 0x10;
    into->can_look = (flags & 0x20) == 0x20;
    into->can_talk = (flags & 0x40) == 0x40;
    into->can_pickup = (flags & 0x80) == 0x80;
}

void yc_res_pro_parse_weapon_flags(unsigned char flags, yc_res_pro_weapon_flags_t *into) {
    into->big_gun = (flags & 0x01) == 0x01;
    into->two_handed = (flags & 0x02) == 0x02;
}

void yc_res_pro_parse_light_passage(unsigned char flags, yc_res_pro_light_passage_t *into) {
    into->vertical = (flags & 0x00) == 0x00;
    into->horizontal = (flags & 0x08) == 0x08;
    into->north = (flags & 0x10) == 0x10;
    into->south = (flags & 0x20) == 0x20;
    into->east = (flags & 0x40) == 0x40;
    into->west = (flags & 0x80) == 0x80;
}
