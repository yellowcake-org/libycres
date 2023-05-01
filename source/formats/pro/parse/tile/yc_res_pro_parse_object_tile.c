#include <libycres.h>
#include <private.h>

#include <stdlib.h>

void yc_res_pro_tile_parse_cleanup(yc_res_pro_object_tile_t *tile);

yc_res_pro_status_t yc_res_pro_object_tile_parse(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_pro_object_t *into
) {
    yc_res_pro_object_tile_t *tile = malloc(sizeof(yc_res_pro_object_tile_t));

    if (NULL == tile) {
        yc_res_pro_tile_parse_cleanup(tile);
        return YC_RES_PRO_STATUS_MEM;
    }

    if (0 == io->fread(&tile->material, sizeof(uint32_t), 1, file)) {
        yc_res_pro_tile_parse_cleanup(tile);
        return YC_RES_PRO_STATUS_IO;
    }
    tile->material = yc_res_byteorder_uint32(tile->material);

    into->data.tile = tile;
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_tile_parse_cleanup(yc_res_pro_object_tile_t *tile) {
    if (NULL != tile) { free(tile); }
}
