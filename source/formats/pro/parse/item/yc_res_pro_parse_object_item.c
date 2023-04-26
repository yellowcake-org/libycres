#include <libycres.h>
#include <private.h>

#include "yc_res_pro_parse_object_item.h"

#include <stdlib.h>

void yc_res_pro_parse_item_flags(const unsigned char bytes[3], yc_res_pro_object_item_t *into);

void yc_res_pro_parse_item_attack_modes(unsigned char modes, yc_res_pro_object_item_t *into);

yc_res_pro_object_item_data_parser_t *yc_res_pro_parse_item_data_parser(yc_res_pro_object_item_t *from);

void yc_res_pro_item_parse_cleanup(yc_res_pro_object_item_t *item);

yc_res_pro_status_t yc_res_pro_object_item_parse(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_pro_object_t *into
) {
    yc_res_pro_object_item_t *item = malloc(sizeof(yc_res_pro_object_item_t));

    if (NULL == item) {
        yc_res_pro_item_parse_cleanup(item);
        return YC_RES_PRO_STATUS_MEM;
    }

    unsigned char flags_bytes[3] = {0, 0, 0};
    if (0 == io->fread(&flags_bytes[0], 3, 1, file)) {
        yc_res_pro_item_parse_cleanup(item);
        return YC_RES_PRO_STATUS_IO;
    }
    yc_res_pro_parse_item_flags(flags_bytes, item);

    unsigned char attack_modes;
    if (0 == io->fread(&attack_modes, 1, 1, file)) {
        yc_res_pro_item_parse_cleanup(item);
        return YC_RES_PRO_STATUS_IO;
    }
    yc_res_pro_parse_item_attack_modes(attack_modes, item);

    if (0 == io->fread(&item->script_id, sizeof(uint32_t), 1, file)) {
        yc_res_pro_item_parse_cleanup(item);
        return YC_RES_PRO_STATUS_IO;
    }
    item->script_id = yc_res_byteorder_uint32(item->script_id);

    uint32_t type = 0;
    if (0 == io->fread(&type, sizeof(uint32_t), 1, file)) {
        yc_res_pro_item_parse_cleanup(item);
        return YC_RES_PRO_STATUS_IO;
    }
    type = yc_res_byteorder_uint32(type);
    item->type = type;

    uint32_t material = 0;
    if (0 == io->fread(&material, sizeof(uint32_t), 1, file)) {
        yc_res_pro_item_parse_cleanup(item);
        return YC_RES_PRO_STATUS_IO;
    }
    material = yc_res_byteorder_uint32(material);
    item->material = material;

    uint32_t volume = 0;
    if (0 == io->fread(&volume, sizeof(uint32_t), 1, file)) {
        yc_res_pro_item_parse_cleanup(item);
        return YC_RES_PRO_STATUS_IO;
    }
    volume = yc_res_byteorder_uint32(volume);
    item->volume = volume;

    uint32_t weight = 0;
    if (0 == io->fread(&weight, sizeof(uint32_t), 1, file)) {
        yc_res_pro_item_parse_cleanup(item);
        return YC_RES_PRO_STATUS_IO;
    }
    weight = yc_res_byteorder_uint32(weight);
    item->weight = weight;

    uint32_t cost = 0;
    if (0 == io->fread(&cost, sizeof(uint32_t), 1, file)) {
        yc_res_pro_item_parse_cleanup(item);
        return YC_RES_PRO_STATUS_IO;
    }
    cost = yc_res_byteorder_uint32(cost);
    item->cost = cost;

    if (0 == io->fread(&item->sprite_id, sizeof(uint32_t), 1, file)) {
        yc_res_pro_item_parse_cleanup(item);
        return YC_RES_PRO_STATUS_IO;
    }
    item->sprite_id = yc_res_byteorder_uint32(item->sprite_id);

    if (0 == io->fread(&item->sound_id, sizeof(char), 1, file)) {
        yc_res_pro_item_parse_cleanup(item);
        return YC_RES_PRO_STATUS_IO;
    }

    yc_res_pro_object_item_data_parser_t *parser = yc_res_pro_parse_item_data_parser(item);
    yc_res_pro_status_t status = parser(file, io, item);

    if (YC_RES_PRO_STATUS_OK != status) {
        yc_res_pro_item_parse_cleanup(item);
        return status;
    }

    yc_res_pro_item_parse_cleanup(NULL);

    into->data.item = item;
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_parse_item_flags(const unsigned char bytes[3], yc_res_pro_object_item_t *into) {
    into->flags.is_hidden = (bytes[0] & 0x08) == 0x08;
}

void yc_res_pro_parse_item_attack_modes(unsigned char modes, yc_res_pro_object_item_t *into) {
    yc_res_pro_object_item_attack_t first = modes & 0xF;
    yc_res_pro_object_item_attack_t second = (modes >> 4) & 0xF;

    into->primary = first;
    into->secondary = second;
}

yc_res_pro_object_item_data_parser_t *yc_res_pro_parse_item_data_parser(yc_res_pro_object_item_t *from) {
    switch (from->type) {
        case YC_RES_PRO_OBJECT_ITEM_TYPE_ARMOR:
            return &yc_res_pro_object_item_armor_parse;
        case YC_RES_PRO_OBJECT_ITEM_TYPE_CONTAINER:
            return &yc_res_pro_object_item_container_parse;
        case YC_RES_PRO_OBJECT_ITEM_TYPE_DRUG:
            return &yc_res_pro_object_item_drug_parse;
        case YC_RES_PRO_OBJECT_ITEM_TYPE_WEAPON:
            return &yc_res_pro_object_item_weapon_parse;
        case YC_RES_PRO_OBJECT_ITEM_TYPE_AMMO:
            return &yc_res_pro_object_item_ammo_parse;
        case YC_RES_PRO_OBJECT_ITEM_TYPE_MISC:
        case YC_RES_PRO_OBJECT_ITEM_TYPE_KEY:
        default:
            return NULL;
    }
}

void yc_res_pro_item_parse_cleanup(yc_res_pro_object_item_t *item) {
    if (NULL != item) { free(item); }
}
