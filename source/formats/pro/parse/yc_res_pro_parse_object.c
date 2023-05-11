#include <libycres.h>
#include <private.h>

#include "yc_res_pro_parse_object.h"

#include <stdlib.h>

void yc_res_pro_parse_cleanup(void *file, const yc_res_io_fs_api_t *api, yc_res_pro_object_t *object);

yc_res_pro_status_t yc_res_pro_parse(
        const char *filename,
        const yc_res_io_fs_api_t *api,
        yc_res_pro_parse_result_t *result
) {
    void *file = api->fopen(filename, "rb");

    if (NULL == file) {
        yc_res_pro_parse_cleanup(file, api, NULL);
        return YC_RES_PRO_STATUS_IO;
    }

    yc_res_pro_object_t *object = malloc(sizeof(yc_res_pro_object_t));

    if (NULL == object) {
        yc_res_pro_parse_cleanup(file, api, object);
        return YC_RES_PRO_STATUS_MEM;
    }

    if (0 == api->fread(&object->proto_id, sizeof(uint32_t), 1, file)) {
        yc_res_pro_parse_cleanup(file, api, object);
        return YC_RES_PRO_STATUS_IO;
    }
    object->proto_id = yc_res_byteorder_uint32(object->proto_id);

    if (0 == api->fread(&object->text_id, sizeof(uint32_t), 1, file)) {
        yc_res_pro_parse_cleanup(file, api, object);
        return YC_RES_PRO_STATUS_IO;
    }
    object->text_id = yc_res_byteorder_uint32(object->text_id);

    if (0 == api->fread(&object->sprite_id, sizeof(uint32_t), 1, file)) {
        yc_res_pro_parse_cleanup(file, api, object);
        return YC_RES_PRO_STATUS_IO;
    }
    object->sprite_id = yc_res_byteorder_uint32(object->sprite_id);

    uint32_t light_radius = 0;
    if (0 == api->fread(&light_radius, sizeof(uint32_t), 1, file)) {
        yc_res_pro_parse_cleanup(file, api, object);
        return YC_RES_PRO_STATUS_IO;
    }

    light_radius = yc_res_byteorder_uint32(light_radius);
    object->lighting.radius = (u_int8_t) light_radius;

    uint32_t light_level = 0;
    if (0 == api->fread(&light_level, sizeof(uint32_t), 1, file)) {
        yc_res_pro_parse_cleanup(file, api, object);
        return YC_RES_PRO_STATUS_IO;
    }

    light_level = yc_res_byteorder_uint32(light_level);
    object->lighting.level = (u_int16_t) light_level;

    uint32_t flags = 0;
    if (0 == api->fread(&flags, sizeof(uint32_t), 1, file)) {
        yc_res_pro_parse_cleanup(file, api, object);
        return YC_RES_PRO_STATUS_IO;
    }

    flags = yc_res_byteorder_uint32(flags);
    object->flags = yc_res_pro_parse_flags(flags);

    yc_res_pro_object_data_parser_t *all[] = {
            &yc_res_pro_object_item_parse,
            &yc_res_pro_object_critter_parse,
            &yc_res_pro_object_scenery_parse,
            &yc_res_pro_object_wall_parse,
            &yc_res_pro_object_tile_parse,
            &yc_res_pro_object_misc_parse
    };

    yc_res_pro_object_data_parser_t *fitting = all[yc_res_pro_object_type_from_pid(object->proto_id)];
    yc_res_pro_status_t status = fitting(file, api, object);

    if (YC_RES_PRO_STATUS_OK != status) {
        yc_res_pro_parse_cleanup(file, api, object);
        return status;
    }

    yc_res_pro_parse_cleanup(file, api, NULL);

    result->object = object;
    return YC_RES_PRO_STATUS_OK;
}

yc_res_pro_object_flags_t yc_res_pro_parse_flags(uint32_t flags) {
    yc_res_pro_object_flags_t result;

    result.is_flat = 0x00000008 == (flags & 0x00000008);
    result.multi_hex = 0x00000800 == (flags & 0x00000800);

    result.no_block = 0x00000010 == (flags & 0x00000010);
    result.no_border = 0x00000020 == (flags & 0x00000020);

    result.light_through = 0x20000000 == (flags & 0x20000000);
    result.shoot_through = 0x80000000 == (flags & 0x80000000);

    result.transparency = YC_RES_PRO_TRANS_NONE;

    if (0x00004000 == (flags & 0x00004000)) { result.transparency = YC_RES_PRO_TRANS_RED; }
    if (0x00008000 == (flags & 0x00008000)) { result.transparency = YC_RES_PRO_TRANS_NONE; }
    if (0x00010000 == (flags & 0x00010000)) { result.transparency = YC_RES_PRO_TRANS_WALL; }
    if (0x00020000 == (flags & 0x00020000)) { result.transparency = YC_RES_PRO_TRANS_GLASS; }
    if (0x00040000 == (flags & 0x00040000)) { result.transparency = YC_RES_PRO_TRANS_STEAM; }
    if (0x00080000 == (flags & 0x00080000)) { result.transparency = YC_RES_PRO_TRANS_ENERGY; }
    if (0x10000000 == (flags & 0x10000000)) { result.transparency = YC_RES_PRO_TRANS_WALL_END; }

    return result;
}

void yc_res_pro_parse_cleanup(void *file, const yc_res_io_fs_api_t *api, yc_res_pro_object_t *object) {
    if (NULL != file) { api->fclose(file); }

    if (NULL != object) {
        yc_res_pro_object_invalidate(object);
        free(object);
    }
}
