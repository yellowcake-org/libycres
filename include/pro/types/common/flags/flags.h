#ifndef LIBYCRES_INCLUDE_PRO_TYPES_COMMON_FLAGS_H
#define LIBYCRES_INCLUDE_PRO_TYPES_COMMON_FLAGS_H

typedef struct yc_res_pro_action_flags {
    bool knees_down: 1;
    bool can_be_used: 1, can_be_used_on: 1;
    bool can_look: 1, can_talk: 1, can_pickup: 1;
} yc_res_pro_action_flags_t;

typedef struct yc_res_pro_weapon_flags {
    bool big_gun: 1, two_handed: 1;
} yc_res_pro_weapon_flags_t;

#endif //LIBYCRES_INCLUDE_PRO_TYPES_COMMON_FLAGS_H
