#include <libycres.h>
#include <private.h>
#include <stdlib.h>
#include <memory.h>

#include "yc_res_map_parse_object.h"
#include "../../pro/parse/yc_res_pro_parse_object.h"

yc_res_map_status_t yc_res_map_parse_object(
        void *file,
        const yc_res_io_fs_api_t *api,
        const yc_res_map_parse_db_api_t *fetchers,
        yc_res_map_level_object_t *into
) {
    uint32_t entry_id = 0;
    if (0 == api->fread(&entry_id, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    entry_id = yc_res_byteorder_uint32(entry_id);

    if (0 == api->fread(&into->location.grid_idx, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    into->location.grid_idx = yc_res_byteorder_uint32(into->location.grid_idx);

    if (0 == api->fread(&into->x, sizeof(int32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    into->x = yc_res_byteorder_int32(into->x);

    if (0 == api->fread(&into->y, sizeof(int32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    into->y = yc_res_byteorder_int32(into->y);

    if (0 == api->fread(&into->sx, sizeof(int32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    into->sx = yc_res_byteorder_int32(into->sx);

    if (0 == api->fread(&into->sy, sizeof(int32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    into->sy = yc_res_byteorder_int32(into->sy);

    if (0 == api->fread(&into->frame_idx, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    into->frame_idx = yc_res_byteorder_uint32(into->frame_idx);

    uint32_t orientation = 0;
    if (0 == api->fread(&orientation, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    orientation = yc_res_byteorder_uint32(orientation);
    into->location.orientation_idx = orientation;

    if (0 == api->fread(&into->sprite_id, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    into->sprite_id = yc_res_byteorder_uint32(into->sprite_id);

    uint32_t flags = 0;
    if (0 == api->fread(&flags, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    flags = yc_res_byteorder_uint32(flags);
    into->flags = yc_res_pro_parse_flags(flags);

    uint32_t elevation = 0;
    if (0 == api->fread(&elevation, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    elevation = yc_res_byteorder_uint32(elevation);
    into->location.elevation_idx = elevation;

    if (0 == api->fread(&into->proto_id, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    into->proto_id = yc_res_byteorder_uint32(into->proto_id);

    if (0 == api->fread(&into->critter_idx, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    into->critter_idx = yc_res_byteorder_uint32(into->critter_idx);

    uint32_t light_radius = 0;
    if (0 == api->fread(&light_radius, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    light_radius = yc_res_byteorder_uint32(light_radius);
    into->light.radius = light_radius;

    uint32_t light_level = 0;
    if (0 == api->fread(&light_level, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    light_level = yc_res_byteorder_uint32(light_level);
    into->light.level = light_level;

    if (0 == api->fread(&into->outline_color_idx, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    into->outline_color_idx = yc_res_byteorder_uint32(into->outline_color_idx);

    if (0 == api->fread(&into->program_id, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    into->program_id = yc_res_byteorder_uint32(into->program_id);

    if (0 == api->fread(&into->script_id, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    into->script_id = yc_res_byteorder_uint32(into->script_id);

    uint32_t inventory_records_count = 0;
    if (0 == api->fread(&inventory_records_count, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    inventory_records_count = yc_res_byteorder_uint32(inventory_records_count);

    uint32_t inventory_capacity = 0;
    if (0 == api->fread(&inventory_capacity, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    inventory_capacity = yc_res_byteorder_uint32(inventory_capacity);

    if (0 != api->fseek(file, sizeof(uint32_t), SEEK_CUR)) { return YC_RES_MAP_STATUS_IO; }

    uint32_t flags_ext = 0;
    if (0 == api->fread(&flags_ext, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    flags_ext = yc_res_byteorder_uint32(flags_ext);

    yc_res_map_parse_object_patch_parser_t yc_res_map_parse_object_patch_item;
    yc_res_map_parse_object_patch_parser_t yc_res_map_parse_object_patch_critter;
    yc_res_map_parse_object_patch_parser_t yc_res_map_parse_object_patch_scenery;
    yc_res_map_parse_object_patch_parser_t yc_res_map_parse_object_patch_misc;

    yc_res_map_parse_object_patch_parser_t *parsers[YC_RES_PRO_OBJECT_TYPE_COUNT] = {
            &yc_res_map_parse_object_patch_item,
            &yc_res_map_parse_object_patch_critter,
            &yc_res_map_parse_object_patch_scenery,
            NULL,
            NULL,
            &yc_res_map_parse_object_patch_misc,
            NULL,
            NULL,
            NULL,
            NULL
    };

    yc_res_pro_object_type_t type = yc_res_pro_object_type_from_pid(into->proto_id);
    yc_res_map_parse_object_patch_parser_t *fitting = parsers[type];

    if (NULL != fitting) {
        yc_res_map_status_t status = fitting(file, api, fetchers, into->proto_id, &into->patch);
        if (YC_RES_MAP_STATUS_OK != status) { return status; }
    }

    into->occupied = 0;
    into->capacity = inventory_capacity;

    into->inventory = calloc(into->capacity, sizeof(yc_res_map_level_object_t *));
    if (NULL == into->inventory) { return YC_RES_MAP_STATUS_MEM; }

    for (size_t record_idx = 0; record_idx < inventory_records_count; ++record_idx) {
        uint32_t item_idx = 0;
        if (0 == api->fread(&item_idx, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
        item_idx = yc_res_byteorder_uint32(item_idx);

        int32_t overhead = (int32_t) item_idx - ((int32_t) into->capacity - 1);
        if (0 < overhead) {
            into->capacity += overhead;
            yc_res_map_level_object_t **grown =
                    realloc(into->inventory, sizeof(yc_res_map_level_object_t *) * into->capacity);

            if (NULL == grown) { return YC_RES_MAP_STATUS_MEM; }
            into->inventory = grown;

            memset(&into->inventory[into->capacity - overhead], 0, overhead * sizeof(yc_res_map_level_object_t *));
        }

        into->inventory[item_idx] = malloc(sizeof(yc_res_map_level_object_t));
        if (NULL == into->inventory[item_idx]) { return YC_RES_MAP_STATUS_MEM; }

        yc_res_map_level_object_t *item = into->inventory[item_idx];
        yc_res_map_status_t status = yc_res_map_parse_object(file, api, fetchers, item);

        if (YC_RES_MAP_STATUS_OK != status) { return status; }
        into->occupied++;
    }

    return YC_RES_MAP_STATUS_OK;
}
