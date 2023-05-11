#include <libycres.h>
#include <private.h>

#include <stdlib.h>

void yc_res_pro_armor_parse_cleanup(yc_res_pro_object_item_armor_t *armor);

yc_res_pro_status_t yc_res_pro_object_item_armor_parse(
        void *file,
        const yc_res_io_fs_api_t *api,
        yc_res_pro_object_item_t *into
) {
    yc_res_pro_object_item_armor_t *armor = malloc(sizeof(yc_res_pro_object_item_armor_t));

    if (NULL == armor) {
        yc_res_pro_armor_parse_cleanup(armor);
        return YC_RES_PRO_STATUS_MEM;
    }

    if (0 == api->fread(&armor->class, sizeof(uint32_t), 1, file)) {
        yc_res_pro_armor_parse_cleanup(armor);
        return YC_RES_PRO_STATUS_IO;
    }
    armor->class = yc_res_byteorder_uint32(armor->class);

    if (0 == api->fread(&armor->resistances, YC_RES_PRO_DAMAGE_TYPE_COUNT * sizeof(uint32_t), 1, file)) {
        yc_res_pro_armor_parse_cleanup(armor);
        return YC_RES_PRO_STATUS_IO;
    }

    if (0 == api->fread(&armor->thresholds, sizeof(uint32_t), YC_RES_PRO_DAMAGE_TYPE_COUNT, file)) {
        yc_res_pro_armor_parse_cleanup(armor);
        return YC_RES_PRO_STATUS_IO;
    }

    for (size_t dmg_idx = 0; dmg_idx < YC_RES_PRO_DAMAGE_TYPE_COUNT; ++dmg_idx) {
        armor->thresholds[dmg_idx] = yc_res_byteorder_uint32(armor->thresholds[dmg_idx]);
        armor->resistances[dmg_idx] = yc_res_byteorder_uint32(armor->resistances[dmg_idx]);
    }

    if (0 == api->fread(&armor->perk, sizeof(yc_res_pro_perk_t), 1, file)) {
        yc_res_pro_armor_parse_cleanup(armor);
        return YC_RES_PRO_STATUS_IO;
    }
    armor->perk = yc_res_byteorder_int32(armor->perk);

    if (0 == api->fread(&armor->sprite_ids, sizeof(uint32_t), YC_RES_PRO_TYPES_GENDER_COUNT, file)) {
        yc_res_pro_armor_parse_cleanup(armor);
        return YC_RES_PRO_STATUS_IO;
    }

    for (size_t gender_idx = 0; gender_idx < YC_RES_PRO_TYPES_GENDER_COUNT; ++gender_idx) {
        armor->sprite_ids[gender_idx] = yc_res_byteorder_uint32(armor->sprite_ids[gender_idx]);
    }

    into->data.armor = armor;
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_armor_parse_cleanup(yc_res_pro_object_item_armor_t *armor) {
    if (NULL != armor) {
        free(armor);
    }
}
