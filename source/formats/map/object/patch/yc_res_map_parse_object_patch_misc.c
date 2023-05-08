#include <libycres.h>
#include <private.h>

#include "yc_res_map_parse_object_patch.h"

#include <stdlib.h>

yc_res_map_status_t yc_res_map_parse_object_patch_misc(
        void *file,
        const yc_res_io_fs_api_t *io,
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

    if (0 == io->fread(&into->misc->exit.map_id, sizeof(int32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    into->misc->exit.map_id = yc_res_byteorder_int32(into->misc->exit.map_id);

    yc_res_math_location_t *entrance = &into->misc->exit.entrance;
    if (0 == io->fread(&entrance->grid_idx, sizeof(int32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    entrance->grid_idx = yc_res_byteorder_int32(entrance->grid_idx);

    if (0 == io->fread(&entrance->elevation_idx, sizeof(int32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    entrance->elevation_idx = yc_res_byteorder_int32(entrance->elevation_idx);

    if (0 == io->fread(&entrance->orientation_idx, sizeof(int32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    entrance->orientation_idx = yc_res_byteorder_int32(entrance->orientation_idx);

    return YC_RES_MAP_STATUS_OK;
}
