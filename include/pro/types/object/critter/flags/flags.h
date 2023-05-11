#ifndef INCLUDE_PRO_TYPES_OBJECT_CRITTER_FLAGS_H
#define INCLUDE_PRO_TYPES_OBJECT_CRITTER_FLAGS_H

typedef struct yc_res_pro_object_critter_flags {
    bool is_barter_available;

    bool no_steal, no_drop, no_age;
    bool no_limbs, no_heal, no_corpse, no_damage, no_knock_back;

    bool has_special_death;
    bool has_long_limbs;
} yc_res_pro_object_critter_flags_t;

#endif //INCLUDE_PRO_TYPES_OBJECT_CRITTER_FLAGS_H
