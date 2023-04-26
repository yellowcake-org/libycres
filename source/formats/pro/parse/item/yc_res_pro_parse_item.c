#include <libycres.h>
#include <private.h>

#include "yc_res_pro_parse_object_item.h"

#include <stdlib.h>

void yc_res_pro_parse_item_flags(const unsigned char bytes[3], yc_res_pro_object_item_t *into);

void yc_res_pro_parse_item_attack_modes(unsigned char modes, yc_res_pro_object_item_t *into);

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

    yc_res_pro_parser_item_data_t *parser = NULL;
    switch (item->type) {
        case YC_RES_PRO_OBJECT_ITEM_ARMOR:
            parser = &yc_res_pro_object_item_armor_parse;
            break;
        case YC_RES_PRO_OBJECT_ITEM_CONTAINER:
            parser = &yc_res_pro_object_item_container_parse;
            break;
        case YC_RES_PRO_OBJECT_ITEM_DRUG:
            parser = &yc_res_pro_object_item_drug_parse;
            break;
        case YC_RES_PRO_OBJECT_ITEM_AMMO:
            break;
        case YC_RES_PRO_OBJECT_ITEM_MISC:
            break;
        case YC_RES_PRO_OBJECT_ITEM_KEY:
            break;
    }

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
    yc_res_pro_item_attack_t first = modes & 0xF;
    yc_res_pro_item_attack_t second = (modes >> 4) & 0xF;

    into->primary = first;
    into->secondary = second;
}

void yc_res_pro_item_parse_cleanup(yc_res_pro_object_item_t *item) {
    if (NULL != item) { free(item); }
}
