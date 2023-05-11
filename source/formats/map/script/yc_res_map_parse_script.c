#include <libycres.h>
#include <private.h>

#include "yc_res_map_parse_script.h"

#include <stdlib.h>
#include <assert.h>

void yc_res_map_parse_script_cleanup(yc_res_map_script_t *into);

yc_res_map_status_t yc_res_map_parse_script(
        void *file, const yc_res_io_fs_api_t *api, yc_res_map_script_t *into, yc_res_pro_script_type_t type
) {
    if (0 == api->fread(&into->program_id, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_script_cleanup(into);
        return YC_RES_MAP_STATUS_IO;
    }
    into->program_id = yc_res_byteorder_uint32(into->program_id);

    if (0 != api->fseek(file, sizeof(uint32_t), SEEK_CUR)) {
        yc_res_map_parse_script_cleanup(into);
        return YC_RES_MAP_STATUS_IO;
    }

    switch (type) {
        case YC_RES_PRO_SCRIPT_TYPE_SPATIAL: {
            into->data.spatial = malloc(sizeof(yc_res_map_script_spatial_t));

            if (NULL == into->data.spatial) {
                yc_res_map_parse_script_cleanup(into);
                return YC_RES_MAP_STATUS_MEM;
            }

            unsigned char elevation_bytes[2];
            if (0 == api->fread(&elevation_bytes, sizeof(unsigned char), 2, file)) {
                yc_res_map_parse_script_cleanup(into);
                return YC_RES_MAP_STATUS_IO;
            }
            into->data.spatial->elevation = ((elevation_bytes[0] >> 4) + 1) >> 1;

            uint16_t position = 0;
            if (0 == api->fread(&position, sizeof(uint16_t), 1, file)) {
                yc_res_map_parse_script_cleanup(into);
                return YC_RES_MAP_STATUS_IO;
            }
            position = yc_res_byteorder_uint16(position);
            into->data.spatial->hex_idx = position;

            if (0 == api->fread(&into->data.spatial->distance, sizeof(uint32_t), 1, file)) {
                yc_res_map_parse_script_cleanup(into);
                return YC_RES_MAP_STATUS_IO;
            }
            into->data.spatial->distance = yc_res_byteorder_uint32(into->data.spatial->distance);
        }
            break;
        case YC_RES_PRO_SCRIPT_TYPE_TIMED: {
            into->data.timed = malloc(sizeof(yc_res_map_script_timed_t));

            if (NULL == into->data.timed) {
                yc_res_map_parse_script_cleanup(into);
                return YC_RES_MAP_STATUS_MEM;
            }

            if (0 == api->fread(&into->data.timed->duration, sizeof(uint32_t), 1, file)) {
                yc_res_map_parse_script_cleanup(into);
                return YC_RES_MAP_STATUS_IO;
            }
            into->data.timed->duration = yc_res_byteorder_uint32(into->data.timed->duration);
        }
            break;
        default:
            break;
    }

    uint32_t _flags = 0;
    if (0 == api->fread(&_flags, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_script_cleanup(into);
        return YC_RES_MAP_STATUS_IO;
    }
    _flags = yc_res_byteorder_uint16(_flags);

    uint32_t script_idx = 0;
    if (0 == api->fread(&script_idx, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_script_cleanup(into);
        return YC_RES_MAP_STATUS_IO;
    }
    script_idx = yc_res_byteorder_uint32(script_idx);

    into->script_id = script_idx | (type << 24);

    assert(type == yc_res_pro_script_type_from_sid(into->script_id));
    assert(script_idx == yc_res_pro_index_from_script_id(into->script_id));

    if (0 != api->fseek(file, sizeof(uint32_t), SEEK_CUR)) {
        yc_res_map_parse_script_cleanup(into);
        return YC_RES_MAP_STATUS_IO;
    }

    if (0 == api->fread(&into->object_id, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_script_cleanup(into);
        return YC_RES_MAP_STATUS_IO;
    }
    into->object_id = yc_res_byteorder_uint32(into->object_id);

    int32_t lv_count, lv_offset = 0;
    if (0 == api->fread(&lv_offset, sizeof(int32_t), 1, file)) {
        yc_res_map_parse_script_cleanup(into);
        return YC_RES_MAP_STATUS_IO;
    }
    lv_offset = yc_res_byteorder_int32(lv_offset);

    if (0 == api->fread(&lv_count, sizeof(int32_t), 1, file)) {
        yc_res_map_parse_script_cleanup(into);
        return YC_RES_MAP_STATUS_IO;
    }
    lv_count = yc_res_byteorder_int32(lv_count);

    bool has_vars = lv_offset > -1 && lv_count > 0;

    into->variables_count = has_vars ? lv_count : 0;
    into->variables_offset = has_vars ? lv_offset: 0;

    if (0 != api->fseek(file, sizeof(uint32_t) * 8, SEEK_CUR)) {
        yc_res_map_parse_script_cleanup(into);
        return YC_RES_MAP_STATUS_IO;
    }

    return YC_RES_MAP_STATUS_OK;
}

void yc_res_map_parse_script_cleanup(yc_res_map_script_t *into) {
    if (NULL != into->data.timed) {
        free(into->data.timed);
        into->data.timed = NULL;
    }

    if (NULL != into->data.spatial) {
        free(into->data.spatial);
        into->data.spatial = NULL;
    }
}
