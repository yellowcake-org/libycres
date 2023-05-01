#ifndef LIB_YCRES_PRO_TYPES_OBJECT_ITEM_ARMOR_H
#define LIB_YCRES_PRO_TYPES_OBJECT_ITEM_ARMOR_H

typedef struct yc_res_pro_object_item_armor {
    uint32_t class;
    yc_res_pro_perk_t perk;

    uint32_t sprite_ids[YC_RES_PRO_TYPES_GENDER_COUNT];

    uint32_t thresholds[YC_RES_PRO_DAMAGE_TYPE_COUNT];
    uint32_t resistances[YC_RES_PRO_DAMAGE_TYPE_COUNT];
} yc_res_pro_object_item_armor_t;

#endif //LIB_YCRES_PRO_TYPES_OBJECT_ITEM_ARMOR_H
