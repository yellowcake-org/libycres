#include <libycres.h>
#include <private.h>

#include "../../destination/yc_res_pro_parse_object_destination.h"

#include <stdlib.h>

void yc_res_pro_ladder_parse_cleanup(yc_res_pro_object_scenery_ladder_t *ladder);

yc_res_pro_status_t yc_res_pro_object_scenery_ladder_parse(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_pro_object_scenery_t *into
) {
    into->data.ladder = malloc(sizeof(yc_res_pro_object_scenery_ladder_t));

    if (NULL == into->data.ladder) {
        yc_res_pro_ladder_parse_cleanup(into->data.ladder);
        return YC_RES_PRO_STATUS_MEM;
    }

    uint32_t destination_raw = 0;
    if (0 == io->fread(&destination_raw, sizeof(uint32_t), 1, file)) {
        yc_res_pro_ladder_parse_cleanup(into->data.ladder);
        return YC_RES_PRO_STATUS_IO;
    }

    destination_raw = yc_res_byteorder_uint32(destination_raw);
    yc_res_pro_parse_object_destination(destination_raw, &into->data.ladder->destination);

    yc_res_pro_ladder_parse_cleanup(NULL);
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_ladder_parse_cleanup(yc_res_pro_object_scenery_ladder_t *ladder) {
    if (NULL != ladder) { free(ladder); }
}
