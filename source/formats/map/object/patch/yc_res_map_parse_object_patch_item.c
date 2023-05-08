#include <libycres.h>
#include <private.h>

#include "yc_res_map_parse_object_patch.h"

#include <stdlib.h>

yc_res_map_status_t yc_res_map_parse_object_patch_item(
        void *file,
        const yc_res_io_fs_api_t *io,
        const yc_res_map_parse_db_api_t *db,
        uint32_t pid,
        yc_res_map_level_object_patch_t *into
) {
    into->item = malloc(sizeof(yc_res_map_level_object_patch_item_t));
    if (NULL == into->item) { return YC_RES_MAP_STATUS_MEM; }

    into->item->type = db->item_type_from_pid(pid, db->context);
    switch (into->item->type) {
        case YC_RES_PRO_OBJECT_ITEM_TYPE_WEAPON:
            into->item->data.weapon = malloc(sizeof(yc_res_map_level_object_patch_item_weapon_t));
            if (NULL == into->item->data.weapon) { return YC_RES_MAP_STATUS_MEM; }

            yc_res_map_level_object_patch_item_weapon_t *weapon = into->item->data.weapon;

            if (0 == io->fread(&weapon->capacity, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
            weapon->capacity = yc_res_byteorder_uint32(weapon->capacity);

            if (0 == io->fread(&weapon->ammo_pid, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
            weapon->ammo_pid = yc_res_byteorder_uint32(weapon->ammo_pid);

            break;
        case YC_RES_PRO_OBJECT_ITEM_TYPE_AMMO:
            into->item->data.ammo = malloc(sizeof(yc_res_map_level_object_patch_item_ammo_t));
            if (NULL == into->item->data.ammo) { return YC_RES_MAP_STATUS_MEM; }

            yc_res_map_level_object_patch_item_ammo_t *ammo = into->item->data.ammo;

            if (0 == io->fread(&ammo->count, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
            ammo->count = yc_res_byteorder_uint32(ammo->count);

            break;
        case YC_RES_PRO_OBJECT_ITEM_TYPE_MISC:
            into->item->data.misc = malloc(sizeof(yc_res_map_level_object_patch_item_misc_t));
            if (NULL == into->item->data.misc) { return YC_RES_MAP_STATUS_MEM; }

            yc_res_map_level_object_patch_item_misc_t *misc = into->item->data.misc;

            if (0 == io->fread(&misc->count, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
            misc->count = yc_res_byteorder_uint32(misc->count);

            break;
        case YC_RES_PRO_OBJECT_ITEM_TYPE_KEY:
            into->item->data.key = malloc(sizeof(yc_res_map_level_object_patch_item_key_t));
            if (NULL == into->item->data.key) { return YC_RES_MAP_STATUS_MEM; }

            yc_res_map_level_object_patch_item_key_t *key = into->item->data.key;

            if (0 == io->fread(&key->code, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
            key->code = yc_res_byteorder_uint32(key->code);

            break;
        case YC_RES_PRO_OBJECT_ITEM_TYPE_ARMOR:
        case YC_RES_PRO_OBJECT_ITEM_TYPE_CONTAINER:
        case YC_RES_PRO_OBJECT_ITEM_TYPE_DRUG:
            return YC_RES_MAP_STATUS_OK;
        default:
            return YC_RES_MAP_STATUS_CORR;
    }

    return YC_RES_MAP_STATUS_OK;
}
