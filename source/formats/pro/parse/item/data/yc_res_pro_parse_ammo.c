#include <libycres.h>
#include <private.h>

#include <stdlib.h>

void yc_res_pro_ammo_parse_cleanup(yc_res_pro_object_item_ammo_t *ammo);

yc_res_pro_status_t yc_res_pro_object_item_ammo_parse(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_pro_object_item_t *into
) {
    yc_res_pro_object_item_ammo_t *ammo = malloc(sizeof(yc_res_pro_object_item_ammo_t));

    if (NULL == ammo) {
        yc_res_pro_ammo_parse_cleanup(ammo);
        return YC_RES_PRO_STATUS_MEM;
    }

    if (0 == io->fread(&ammo->caliber, sizeof(yc_res_pro_caliber_t), 1, file)) {
        yc_res_pro_ammo_parse_cleanup(ammo);
        return YC_RES_PRO_STATUS_IO;
    }
    ammo->caliber = yc_res_byteorder_int32(ammo->caliber);

    if (0 == io->fread(&ammo->count, sizeof(uint32_t), 1, file)) {
        yc_res_pro_ammo_parse_cleanup(ammo);
        return YC_RES_PRO_STATUS_IO;
    }
    ammo->count = yc_res_byteorder_uint32(ammo->count);

    if (0 == io->fread(&ammo->armor_class, sizeof(int32_t), 1, file)) {
        yc_res_pro_ammo_parse_cleanup(ammo);
        return YC_RES_PRO_STATUS_IO;
    }
    ammo->armor_class = yc_res_byteorder_int32(ammo->armor_class);

    if (0 == io->fread(&ammo->damage_resistance, sizeof(int32_t), 1, file)) {
        yc_res_pro_ammo_parse_cleanup(ammo);
        return YC_RES_PRO_STATUS_IO;
    }
    ammo->damage_resistance = yc_res_byteorder_int32(ammo->damage_resistance);

    if (0 == io->fread(&ammo->damage_multiplier, sizeof(uint32_t), 1, file)) {
        yc_res_pro_ammo_parse_cleanup(ammo);
        return YC_RES_PRO_STATUS_IO;
    }
    ammo->damage_multiplier = yc_res_byteorder_uint32(ammo->damage_multiplier);

    if (0 == io->fread(&ammo->damage_divider, sizeof(uint32_t), 1, file)) {
        yc_res_pro_ammo_parse_cleanup(ammo);
        return YC_RES_PRO_STATUS_IO;
    }
    ammo->damage_divider = yc_res_byteorder_uint32(ammo->damage_divider);

    into->data.ammo = ammo;
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_ammo_parse_cleanup(yc_res_pro_object_item_ammo_t *ammo) {
    if (NULL != ammo) {
        free(ammo);
    }
}
