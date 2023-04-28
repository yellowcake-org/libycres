#include <libycres.h>
#include <private.h>

#include "../../destination/yc_res_pro_parse_object_destination.h"

#include <stdlib.h>

void yc_res_pro_ladder_parse_cleanup(yc_res_pro_object_scenery_ladder_t *ladder);

yc_res_pro_status_t yc_res_pro_object_scenery_ladder_parse(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_pro_object_scenery_ladder_t **result
) {
    *result = malloc(sizeof(yc_res_pro_object_scenery_ladder_t));

    if (NULL == (*result)) {
        yc_res_pro_ladder_parse_cleanup((*result));
        return YC_RES_PRO_STATUS_MEM;
    }

    uint32_t destination_raw = 0;
    if (0 == io->fread(&destination_raw, sizeof(uint32_t), 1, file)) {
        yc_res_pro_ladder_parse_cleanup((*result));
        return YC_RES_PRO_STATUS_IO;
    }

    destination_raw = yc_res_byteorder_uint32(destination_raw);
    yc_res_pro_parse_object_destination(destination_raw, &(*result)->destination);

    yc_res_pro_ladder_parse_cleanup(NULL);
    return YC_RES_PRO_STATUS_OK;
}

yc_res_pro_status_t yc_res_pro_object_scenery_ladder_bottom_parse(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_pro_object_scenery_t *into
) {
    return yc_res_pro_object_scenery_ladder_parse(file, io, &into->data.ladder_bottom);
}

yc_res_pro_status_t yc_res_pro_object_scenery_ladder_top_parse(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_pro_object_scenery_t *into
) {
    return yc_res_pro_object_scenery_ladder_parse(file, io, &into->data.ladder_top);
}

void yc_res_pro_ladder_parse_cleanup(yc_res_pro_object_scenery_ladder_t *ladder) {
    if (NULL != ladder) { free(ladder); }
}
