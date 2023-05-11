#include <libycres.h>
#include <private.h>

#include <stdlib.h>

void yc_res_pro_drug_parse_cleanup(yc_res_pro_object_item_drug_t *drug);

yc_res_pro_status_t yc_res_pro_object_item_drug_parse(
        void *file,
        const yc_res_io_fs_api_t *api,
        yc_res_pro_object_item_t *into
) {
    yc_res_pro_object_item_drug_t *drug = malloc(sizeof(yc_res_pro_object_item_drug_t));
    drug->effects = NULL;

    if (NULL == drug) {
        yc_res_pro_drug_parse_cleanup(drug);
        return YC_RES_PRO_STATUS_MEM;
    }

    typedef struct yc_res_pro_object_item_drug_raw {
        int32_t stat[3];

        int32_t amount0[3];

        uint32_t delay1;
        int32_t amount1[3];

        uint32_t delay2;
        int32_t amount2[3];
    } yc_res_pro_object_item_drug_raw_t;

    yc_res_pro_object_item_drug_raw_t raw_drug;
    if (0 == api->fread(&raw_drug, sizeof(yc_res_pro_object_item_drug_raw_t), 1, file)) {
        yc_res_pro_drug_parse_cleanup(drug);
        return YC_RES_PRO_STATUS_IO;
    }

    raw_drug.delay1 = yc_res_byteorder_uint32(raw_drug.delay1);
    raw_drug.delay2 = yc_res_byteorder_uint32(raw_drug.delay2);

    for (size_t i = 0; i < 3; ++i) {
        raw_drug.stat[i] = yc_res_byteorder_int32(raw_drug.stat[i]);
        raw_drug.amount0[i] = yc_res_byteorder_int32(raw_drug.amount0[i]);
        raw_drug.amount1[i] = yc_res_byteorder_int32(raw_drug.amount1[i]);
        raw_drug.amount2[i] = yc_res_byteorder_int32(raw_drug.amount2[i]);
    }

    size_t effects_count = 0;
    yc_res_pro_object_item_drug_effect_t *effects = NULL;

    uint32_t delays[3] = {0, raw_drug.delay1, raw_drug.delay2};
    int32_t *amounts[3] = {raw_drug.amount0, raw_drug.amount1, raw_drug.amount2};

    for (size_t stat_idx = 0; stat_idx < 3; ++stat_idx) {
        yc_res_pro_stat_t stat = raw_drug.stat[stat_idx];
        if (0 > stat) { continue; }

        bool is_ranged = false;
        if (stat_idx > 0) { is_ranged = raw_drug.stat[stat_idx - 1] == -2; }

        effects_count++;
        yc_res_pro_object_item_drug_effect_t *effects_tmp =
                effects_count > 1 ?
                realloc(effects, sizeof(yc_res_pro_object_item_drug_effect_t) * effects_count) :
                malloc(sizeof(yc_res_pro_object_item_drug_effect_t) * effects_count);

        if (NULL == effects_tmp) {
            if (NULL != effects) { free(effects); }
            yc_res_pro_drug_parse_cleanup(drug);

            return YC_RES_PRO_STATUS_MEM;
        }

        effects = effects_tmp;

        yc_res_pro_object_item_drug_effect_t *effect = &effects[effects_count - 1];
        effect->stat = stat;

        for (size_t modifier_idx = 0; modifier_idx < 3; ++modifier_idx) {
            yc_res_pro_object_item_drug_stat_modifier_t *modifier = &effect->modifiers[modifier_idx];
            modifier->delay = delays[modifier_idx];

            modifier->impact.to = amounts[modifier_idx][stat_idx];
            modifier->impact.from = is_ranged ? amounts[modifier_idx][stat_idx - 1] : 0;
        }
    }

    drug->count = effects_count;
    drug->effects = effects;

    uint32_t chance = 0;
    if (0 == api->fread(&chance, sizeof(uint32_t), 1, file)) {
        yc_res_pro_drug_parse_cleanup(drug);
        return YC_RES_PRO_STATUS_IO;
    }
    chance = yc_res_byteorder_uint32(chance);
    drug->addiction_chance = chance;

    if (0 == api->fread(&drug->withdrawal_perk, sizeof(yc_res_pro_perk_t), 1, file)) {
        yc_res_pro_drug_parse_cleanup(drug);
        return YC_RES_PRO_STATUS_IO;
    }
    drug->withdrawal_perk = yc_res_byteorder_int32(drug->withdrawal_perk);

    if (0 == api->fread(&drug->withdrawal_delay, sizeof(uint32_t), 1, file)) {
        yc_res_pro_drug_parse_cleanup(drug);
        return YC_RES_PRO_STATUS_IO;
    }
    drug->withdrawal_delay = yc_res_byteorder_uint32(drug->withdrawal_delay);

    into->data.drug = drug;
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_drug_parse_cleanup(yc_res_pro_object_item_drug_t *drug) {
    if (NULL != drug) {
        if (NULL != drug->effects) {
            free(drug->effects);
            drug->effects = NULL;

            drug->count = 0;
        }

        free(drug);
    }
}
