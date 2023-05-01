#ifndef LIB_YCRES_PRO_TYPES_OBJECT_ITEM_AMMO_H
#define LIB_YCRES_PRO_TYPES_OBJECT_ITEM_AMMO_H

typedef struct yc_res_pro_object_item_ammo {
    uint32_t count;
    yc_res_pro_caliber_t caliber;

    int32_t armor_class, damage_resistance;
    uint32_t damage_multiplier, damage_divider;
} yc_res_pro_object_item_ammo_t;

#endif //LIB_YCRES_PRO_TYPES_OBJECT_ITEM_AMMO_H
