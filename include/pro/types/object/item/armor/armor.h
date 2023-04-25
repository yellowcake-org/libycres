#ifndef LIB_YCRES_PRO_TYPES_OBJECT_ITEM_ARMOR_H
#define LIB_YCRES_PRO_TYPES_OBJECT_ITEM_ARMOR_H

typedef struct yc_res_pro_object_item_armor_stat {
    uint32_t value;
    yc_res_pro_types_damage_t damage;
} yc_res_pro_object_item_armor_stat_t;

typedef struct yc_res_pro_object_item_armor {
    uint32_t class;
    uint32_t sprite_ids[YC_RES_PRO_TYPES_GENDER_COUNT];

    yc_res_pro_perk_t perk;
    yc_res_pro_object_item_armor_stat_t threshold[YC_RES_PRO_DAMAGE_COUNT];
    yc_res_pro_object_item_armor_stat_t resistance[YC_RES_PRO_DAMAGE_COUNT];
} yc_res_pro_object_item_armor_t;

#endif //LIB_YCRES_PRO_TYPES_OBJECT_ITEM_ARMOR_H
