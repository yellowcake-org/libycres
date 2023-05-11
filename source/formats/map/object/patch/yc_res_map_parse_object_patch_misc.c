#include <libycres.h>
#include <private.h>

#include "yc_res_map_parse_object_patch.h"

#include <stdlib.h>

yc_res_map_status_t yc_res_map_parse_object_patch_misc(
        void *file,
        const yc_res_io_fs_api_t *api,
        const yc_res_map_parse_db_api_t *db,
        uint32_t pid,
        yc_res_map_level_object_patch_t *into
) {
    (void) (db);
    (void) (pid);

    into->misc = malloc(sizeof(yc_res_map_level_object_patch_misc_t));
    if (NULL == into->misc) { return YC_RES_MAP_STATUS_MEM; }

    into->misc->is_exit = pid >= 0x5000010 && pid <= 0x5000017;
    if (false == into->misc->is_exit) { return YC_RES_MAP_STATUS_OK; }

    if (0 == api->fread(&into->misc->exit.map_id, sizeof(int32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    into->misc->exit.map_id = yc_res_byteorder_int32(into->misc->exit.map_id);

    yc_res_math_location_t *entrance = &into->misc->exit.entrance;
    if (0 == api->fread(&entrance->grid_idx, sizeof(int32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    entrance->grid_idx = yc_res_byteorder_int32(entrance->grid_idx);

    uint32_t elevation_idx = 0;
    if (0 == api->fread(&elevation_idx, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    elevation_idx = yc_res_byteorder_uint32(elevation_idx);
    entrance->elevation_idx = elevation_idx;

    uint32_t orientation_idx = 0;
    if (0 == api->fread(&orientation_idx, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    orientation_idx = yc_res_byteorder_uint32(orientation_idx);
    entrance->orientation_idx = orientation_idx;

    return YC_RES_MAP_STATUS_OK;
}
