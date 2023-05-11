#include <libycres.h>
#include <private.h>

#include <stdlib.h>

void yc_res_pro_weapon_parse_cleanup(yc_res_pro_object_item_weapon_t *weapon);

yc_res_pro_status_t yc_res_pro_object_item_weapon_parse(
        void *file,
        const yc_res_io_fs_api_t *api,
        yc_res_pro_object_item_t *into
) {
    yc_res_pro_object_item_weapon_t *weapon = malloc(sizeof(yc_res_pro_object_item_weapon_t));

    if (NULL == weapon) {
        yc_res_pro_weapon_parse_cleanup(weapon);
        return YC_RES_PRO_STATUS_MEM;
    }

    uint32_t anim_code = 0;
    if (0 == api->fread(&anim_code, sizeof(uint32_t), 1, file)) {
        yc_res_pro_weapon_parse_cleanup(weapon);
        return YC_RES_PRO_STATUS_IO;
    }
    anim_code = yc_res_byteorder_uint32(anim_code);
    weapon->animation = anim_code;

    if (0 == api->fread(&weapon->damage_min, sizeof(uint32_t), 1, file)) {
        yc_res_pro_weapon_parse_cleanup(weapon);
        return YC_RES_PRO_STATUS_IO;
    }
    weapon->damage_min = yc_res_byteorder_uint32(weapon->damage_min);

    if (0 == api->fread(&weapon->damage_max, sizeof(uint32_t), 1, file)) {
        yc_res_pro_weapon_parse_cleanup(weapon);
        return YC_RES_PRO_STATUS_IO;
    }
    weapon->damage_max = yc_res_byteorder_uint32(weapon->damage_max);

    if (0 == api->fread(&weapon->damage_type, sizeof(uint32_t), 1, file)) {
        yc_res_pro_weapon_parse_cleanup(weapon);
        return YC_RES_PRO_STATUS_IO;
    }
    weapon->damage_type = yc_res_byteorder_uint32(weapon->damage_type);

    if (0 == api->fread(&weapon->fire_primary.max_range, sizeof(uint32_t), 1, file)) {
        yc_res_pro_weapon_parse_cleanup(weapon);
        return YC_RES_PRO_STATUS_IO;
    }
    weapon->fire_primary.max_range = yc_res_byteorder_uint32(weapon->fire_primary.max_range);

    if (0 == api->fread(&weapon->fire_secondary.max_range, sizeof(uint32_t), 1, file)) {
        yc_res_pro_weapon_parse_cleanup(weapon);
        return YC_RES_PRO_STATUS_IO;
    }
    weapon->fire_secondary.max_range = yc_res_byteorder_uint32(weapon->fire_secondary.max_range);

    if (0 == api->fread(&weapon->projectile_pid, sizeof(uint32_t), 1, file)) {
        yc_res_pro_weapon_parse_cleanup(weapon);
        return YC_RES_PRO_STATUS_IO;
    }
    weapon->projectile_pid = yc_res_byteorder_uint32(weapon->projectile_pid);

    if (0 == api->fread(&weapon->requirement.value, sizeof(uint32_t), 1, file)) {
        yc_res_pro_weapon_parse_cleanup(weapon);
        return YC_RES_PRO_STATUS_IO;
    }
    weapon->requirement.value = yc_res_byteorder_uint32(weapon->requirement.value);
    weapon->requirement.stat = YC_RES_PRO_STAT_STRENGTH;

    if (0 == api->fread(&weapon->fire_primary.cost, sizeof(uint32_t), 1, file)) {
        yc_res_pro_weapon_parse_cleanup(weapon);
        return YC_RES_PRO_STATUS_IO;
    }
    weapon->fire_primary.cost = yc_res_byteorder_uint32(weapon->fire_primary.cost);

    if (0 == api->fread(&weapon->fire_secondary.cost, sizeof(uint32_t), 1, file)) {
        yc_res_pro_weapon_parse_cleanup(weapon);
        return YC_RES_PRO_STATUS_IO;
    }
    weapon->fire_secondary.cost = yc_res_byteorder_uint32(weapon->fire_secondary.cost);

    int32_t critical_failure_idx = -1;
    if (0 == api->fread(&critical_failure_idx, sizeof(int32_t), 1, file)) {
        yc_res_pro_weapon_parse_cleanup(weapon);
        return YC_RES_PRO_STATUS_IO;
    }
    critical_failure_idx = yc_res_byteorder_int32(critical_failure_idx);
    weapon->critical_failure_idx = (char) critical_failure_idx;

    if (0 == api->fread(&weapon->perk, sizeof(yc_res_pro_perk_t), 1, file)) {
        yc_res_pro_weapon_parse_cleanup(weapon);
        return YC_RES_PRO_STATUS_IO;
    }
    weapon->perk = yc_res_byteorder_int32(weapon->perk);

    if (0 == api->fread(&weapon->burst, sizeof(uint32_t), 1, file)) {
        yc_res_pro_weapon_parse_cleanup(weapon);
        return YC_RES_PRO_STATUS_IO;
    }
    weapon->burst = yc_res_byteorder_uint32(weapon->burst);

    if (0 == api->fread(&weapon->caliber, sizeof(yc_res_pro_caliber_t), 1, file)) {
        yc_res_pro_weapon_parse_cleanup(weapon);
        return YC_RES_PRO_STATUS_IO;
    }
    weapon->caliber = yc_res_byteorder_int32(weapon->caliber);

    if (0 == api->fread(&weapon->ammo_pid, sizeof(uint32_t), 1, file)) {
        yc_res_pro_weapon_parse_cleanup(weapon);
        return YC_RES_PRO_STATUS_IO;
    }
    weapon->ammo_pid = yc_res_byteorder_uint32(weapon->ammo_pid);

    if (0 == api->fread(&weapon->capacity, sizeof(uint32_t), 1, file)) {
        yc_res_pro_weapon_parse_cleanup(weapon);
        return YC_RES_PRO_STATUS_IO;
    }
    weapon->capacity = yc_res_byteorder_uint32(weapon->capacity);

    if (0 == api->fread(&weapon->sound_id, sizeof(unsigned  char ), 1, file)) {
        yc_res_pro_weapon_parse_cleanup(weapon);
        return YC_RES_PRO_STATUS_IO;
    }

    into->data.weapon = weapon;
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_weapon_parse_cleanup(yc_res_pro_object_item_weapon_t *weapon) {
    if (NULL != weapon) {
        free(weapon);
    }
}
