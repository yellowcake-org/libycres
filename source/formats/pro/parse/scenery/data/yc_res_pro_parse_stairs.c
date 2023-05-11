#include <libycres.h>
#include <private.h>

#include "../../destination/yc_res_pro_parse_object_destination.h"

#include <stdlib.h>

void yc_res_pro_stairs_parse_cleanup(yc_res_pro_object_scenery_stairs_t *stairs);

yc_res_pro_status_t yc_res_pro_object_scenery_stairs_parse(
        void *file,
        const yc_res_io_fs_api_t *api,
        yc_res_pro_object_scenery_t *into
) {
    yc_res_pro_object_scenery_stairs_t *stairs = malloc(sizeof(yc_res_pro_object_scenery_stairs_t));

    if (NULL == stairs) {
        yc_res_pro_stairs_parse_cleanup(stairs);
        return YC_RES_PRO_STATUS_MEM;
    }

    uint32_t destination_raw = 0;
    if (0 == api->fread(&destination_raw, sizeof(uint32_t), 1, file)) {
        yc_res_pro_stairs_parse_cleanup(stairs);
        return YC_RES_PRO_STATUS_IO;
    }
    destination_raw = yc_res_byteorder_uint32(destination_raw);
    yc_res_pro_parse_object_destination(destination_raw, &stairs->destination);

    if (0 == api->fread(&stairs->map_id, sizeof(int32_t), 1, file)) {
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
