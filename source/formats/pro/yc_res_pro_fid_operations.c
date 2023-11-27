#include <libycres.h>
#include <stdlib.h>

bool yc_rec_pro_is_sprite_split(uint32_t sprite_id) {
    yc_res_pro_object_type_t type = yc_res_pro_object_type_from_fid(sprite_id);
    return YC_RES_PRO_OBJECT_TYPE_CRITTER == type && ((sprite_id >> 28) & 0x7) > 0;
}

yc_res_pro_object_item_weapon_animation_t yc_rec_pro_critter_weapon_animation_from(uint32_t sprite_id) {
    return (sprite_id >> 12) & 0xF;
}

yc_res_pro_object_critter_animation_t yc_rec_pro_critter_animation_from(uint32_t sprite_id) {
    return sprite_id >> 16;
}

unsigned char weapon_to_code(unsigned char weapon) {
    return weapon == 0 ? 'a' : 'c' + weapon;
}

unsigned char animation_to_code(unsigned char current, unsigned char base, unsigned char start) {
    return (start + current - base);
}

yc_res_pro_status_t yc_res_pro_critter_sprite_suffix_from(uint32_t sprite_id, unsigned char **result) {
    if (NULL == result) { return YC_RES_PRO_STATUS_IN; }

    unsigned char *suffix = malloc(sizeof(unsigned char) * 2 + 1);
    if (NULL == suffix) { return YC_RES_PRO_STATUS_MEM; }
    suffix[2] = 0;

    unsigned char critter_animation = yc_rec_pro_critter_animation_from(sprite_id);
    unsigned char weapon_animation = yc_rec_pro_critter_weapon_animation_from(sprite_id);

    if (36 == critter_animation) {
        suffix[0] = 'c';
        suffix[1] = 'h';
    } else if (37 == critter_animation) {
        suffix[0] = 'c';
        suffix[1] = 'j';
    } else if ((38 <= critter_animation) && (47 >= critter_animation)) {
        if (0 == weapon_animation) {
            free(suffix);
            *result = NULL;
            return YC_RES_PRO_STATUS_OK;
        }

        suffix[0] = weapon_to_code(weapon_animation);
        suffix[1] = animation_to_code(critter_animation, 38, 'c');
    } else if (64 == critter_animation) {
        suffix[0] = 'n';
        suffix[1] = 'a';
    } else if (48 <= critter_animation) {
        suffix[0] = 'r';
        suffix[1] = animation_to_code(critter_animation, 48, 'a');
    } else if (20 <= critter_animation) {
        suffix[0] = 'b';
        suffix[1] = animation_to_code(critter_animation, 20, 'a');
    } else if (18 == critter_animation) {
        if (1 == weapon_animation || 4 == weapon_animation) {
            suffix[0] = weapon_to_code(weapon_animation);
            suffix[1] = 'm';
        } else {
            suffix[0] = weapon_to_code(0);
            suffix[1] = 's';
        }
    } else if (13 != critter_animation) {
        if (0 == critter_animation || 1 == critter_animation) {
            suffix[0] = weapon_to_code(weapon_animation);
        } else {
            suffix[0] = weapon_to_code(0);
        }

        suffix[1] = animation_to_code(critter_animation, 0, 'a');
    } else {
        if (0 == weapon_animation) { suffix[1] = 'n'; } else { suffix[1] = 'e'; }
        suffix[0] = weapon_to_code(weapon_animation);
    }

    *result = suffix;
    return YC_RES_PRO_STATUS_OK;
}
