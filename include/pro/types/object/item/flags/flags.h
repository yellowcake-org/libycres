#ifndef LIB_YCRES_PRO_TYPES_OBJECT_ITEM_FLAGS_H
#define LIB_YCRES_PRO_TYPES_OBJECT_ITEM_FLAGS_H

typedef struct yc_res_pro_item_flags {
    bool is_hidden;
    bool can_be_used, can_be_used_on, can_pickup;
    bool big_gun, two_handed, natural_weapon;
} yc_res_pro_item_flags_t;

#endif //LIB_YCRES_PRO_TYPES_OBJECT_ITEM_FLAGS_H
