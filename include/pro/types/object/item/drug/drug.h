#ifndef INCLUDE_PRO_TYPES_OBJECT_ITEM_DRUG_H
#define INCLUDE_PRO_TYPES_OBJECT_ITEM_DRUG_H

typedef struct yc_res_pro_object_item_drug_stat_modifier_impact {
    int32_t from, to;
} yc_res_pro_object_item_drug_stat_modifier_impact_t;

typedef struct yc_res_pro_object_item_drug_stat_modifier {
    uint32_t delay;
    yc_res_pro_object_item_drug_stat_modifier_impact_t impact;
} yc_res_pro_object_item_drug_stat_modifier_t;

typedef struct yc_res_pro_object_item_drug_effect {
    yc_res_pro_stat_t stat;
    yc_res_pro_object_item_drug_stat_modifier_t modifiers[3];
} yc_res_pro_object_item_drug_effect_t;

typedef struct yc_res_pro_object_item_drug {
    size_t count;
    yc_res_pro_object_item_drug_effect_t *effects;

    uint32_t withdrawal_delay;
    yc_res_pro_perk_t withdrawal_perk;
    unsigned char addiction_chance: 7; // 0..100 percent
} yc_res_pro_object_item_drug_t;

#endif //INCLUDE_PRO_TYPES_OBJECT_ITEM_DRUG_H
