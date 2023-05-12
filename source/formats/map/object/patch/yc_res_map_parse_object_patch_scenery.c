#include <libycres.h>
#include <private.h>

#include "../../../pro/parse/destination/yc_res_pro_parse_object_destination.h"

#include <stdlib.h>

yc_res_map_status_t yc_res_map_parse_object_patch_scenery(
        void *file,
        const yc_res_io_fs_api_t *api,
        const yc_res_map_parse_db_api_t *fetchers,
        uint32_t pid,
        yc_res_map_level_object_patch_t *into
) {
    into->scenery = malloc(sizeof(yc_res_map_level_object_patch_scenery_t));
    if (NULL == into->scenery) { return YC_RES_MAP_STATUS_MEM; }

    into->scenery->type = fetchers->scenery_type_from_pid(pid, fetchers->context);
    switch (into->scenery->type) {
        case YC_RES_PRO_OBJECT_SCENERY_TYPE_DOOR:
            into->scenery->data.door = malloc(sizeof(yc_res_map_level_object_patch_scenery_door_t));
            if (NULL == into->scenery->data.door) { return YC_RES_MAP_STATUS_MEM; }

            yc_res_map_level_object_patch_scenery_door_t *door = into->scenery->data.door;

            unsigned char flags_bytes[4] = {0, 0, 0, 0};
            if (0 == api->fread(&flags_bytes, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
            door->is_passable = (flags_bytes[0] & 0x0F) == 0x0F;

            break;
        case YC_RES_PRO_OBJECT_SCENERY_TYPE_STAIRS: {

            into->scenery->data.stairs = malloc(sizeof(yc_res_map_level_object_patch_scenery_stairs_t));
            if (NULL == into->scenery->data.stairs) { return YC_RES_MAP_STATUS_MEM; }

            yc_res_map_level_object_patch_scenery_stairs_t *stairs = into->scenery->data.stairs;

            uint32_t destination_raw = 0;
            if (0 == api->fread(&destination_raw, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
            destination_raw = yc_res_byteorder_uint32(destination_raw);
            yc_res_pro_parse_object_destination(destination_raw, &stairs->destination);

            if (0 == api->fread(&stairs->map_id, sizeof(int32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
            stairs->map_id = yc_res_byteorder_int32(stairs->map_id);
        }
            break;
        case YC_RES_PRO_OBJECT_SCENERY_TYPE_ELEVATOR:
            into->scenery->data.elevator = malloc(sizeof(yc_res_map_level_object_patch_scenery_elevator_t));
            if (NULL == into->scenery->data.elevator) { return YC_RES_MAP_STATUS_MEM; }

            yc_res_map_level_object_patch_scenery_elevator_t *elevator = into->scenery->data.elevator;

            if (0 == api->fread(&elevator->type, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
            elevator->type = yc_res_byteorder_uint32(elevator->type);

            if (0 == api->fread(&elevator->elevation, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
            elevator->elevation = yc_res_byteorder_uint32(elevator->elevation);

            break;
        case YC_RES_PRO_OBJECT_SCENERY_TYPE_LADDER_BOTTOM:
        case YC_RES_PRO_OBJECT_SCENERY_TYPE_LADDER_TOP: {
            into->scenery->data.ladder = malloc(sizeof(yc_res_map_level_object_patch_scenery_ladder_t));
            if (NULL == into->scenery->data.ladder) { return YC_RES_MAP_STATUS_MEM; }

            yc_res_map_level_object_patch_scenery_ladder_t *ladder = into->scenery->data.ladder;

            uint32_t destination_raw = 0;
            if (0 == api->fread(&destination_raw, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
            destination_raw = yc_res_byteorder_uint32(destination_raw);
            yc_res_pro_parse_object_destination(destination_raw, &ladder->destination);
        }
            break;
        case YC_RES_PRO_OBJECT_SCENERY_TYPE_GENERIC:
            return YC_RES_MAP_STATUS_OK;
        default:
            return YC_RES_MAP_STATUS_CORR;
    }

    return YC_RES_MAP_STATUS_OK;
}
