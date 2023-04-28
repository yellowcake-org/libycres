#include <libycres.h>
#include <private.h>

#include <stdlib.h>

void yc_res_pro_stairs_parse_cleanup(yc_res_pro_object_scenery_stairs_t *stairs);

yc_res_pro_status_t yc_res_pro_object_scenery_stairs_parse(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_pro_object_scenery_t *into
) {
    yc_res_pro_object_scenery_stairs_t *stairs = malloc(sizeof(yc_res_pro_object_scenery_stairs_t));

    if (NULL == stairs) {
        yc_res_pro_stairs_parse_cleanup(stairs);
        return YC_RES_PRO_STATUS_MEM;
    }

    uint32_t destination_raw = 0;
    if (0 == io->fread(&destination_raw, sizeof(uint32_t), 1, file)) {
        yc_res_pro_stairs_parse_cleanup(stairs);
        return YC_RES_PRO_STATUS_IO;
    }
    destination_raw = yc_res_byteorder_uint32(destination_raw);

    const uint32_t BUILT_TILE_TILE_MASK = 0x3FFFFFF;
    const uint32_t BUILT_TILE_ELEVATION_MASK = 0xE0000000;
    const uint32_t BUILT_TILE_ELEVATION_SHIFT = 29;
    const uint32_t BUILT_TILE_ROTATION_MASK = 0x1C000000;
    const uint32_t BUILT_TILE_ROTATION_SHIFT = 26;

    stairs->destination.tile_idx = destination_raw & BUILT_TILE_TILE_MASK;
    stairs->destination.elevation_idx = (destination_raw & BUILT_TILE_ELEVATION_MASK) >> BUILT_TILE_ELEVATION_SHIFT;
    stairs->destination.orientation = (destination_raw & BUILT_TILE_ROTATION_MASK) >> BUILT_TILE_ROTATION_SHIFT;

    if (0 == io->fread(&stairs->map_id, sizeof(uint32_t), 1, file)) {
        yc_res_pro_stairs_parse_cleanup(stairs);
        return YC_RES_PRO_STATUS_IO;
    }
    stairs->map_id = yc_res_byteorder_int32(stairs->map_id);

    yc_res_pro_stairs_parse_cleanup(NULL);

    into->data.stairs = stairs;
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_stairs_parse_cleanup(yc_res_pro_object_scenery_stairs_t *stairs) {
    if (NULL != stairs) { free(stairs); }
}
