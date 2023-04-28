#include <libycres.h>
#include <private.h>

#include "yc_res_pro_parse_object_scenery.h"
#include "../flags/yc_res_pro_parse_object_flags.h"

#include <stdlib.h>

yc_res_pro_object_scenery_data_parser_t *yc_res_pro_parse_scenery_data_parser(yc_res_pro_object_scenery_t *from);
void yc_res_pro_scenery_parse_cleanup(yc_res_pro_object_scenery_t *scenery);

yc_res_pro_status_t yc_res_pro_object_scenery_parse(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_pro_object_t *into
) {
    yc_res_pro_object_scenery_t *scenery = malloc(sizeof(yc_res_pro_object_scenery_t));

    if (NULL == scenery) {
        yc_res_pro_scenery_parse_cleanup(scenery);
        return YC_RES_PRO_STATUS_MEM;
    }

    unsigned char flags_bytes[4] = {0, 0, 0, 0};
    if (0 == io->fread(&flags_bytes, sizeof(flags_bytes), 1, file)) {
        yc_res_pro_scenery_parse_cleanup(scenery);
        return YC_RES_PRO_STATUS_IO;
    }

    yc_res_pro_parse_action_flags(flags_bytes[0], &scenery->flags);
    yc_res_pro_parse_light_passage(flags_bytes[3], &scenery->light);

    if (0 == io->fread(&scenery->script_id, sizeof(uint32_t), 1, file)) {
        yc_res_pro_scenery_parse_cleanup(scenery);
        return YC_RES_PRO_STATUS_IO;
    }
    scenery->script_id = yc_res_byteorder_uint32(scenery->script_id);

    if (0 == io->fread(&scenery->type, sizeof(uint32_t), 1, file)) {
        yc_res_pro_scenery_parse_cleanup(scenery);
        return YC_RES_PRO_STATUS_IO;
    }
    scenery->type = yc_res_byteorder_uint32(scenery->type);

    if (0 == io->fread(&scenery->material, sizeof(uint32_t), 1, file)) {
        yc_res_pro_scenery_parse_cleanup(scenery);
        return YC_RES_PRO_STATUS_IO;
    }
    scenery->material = yc_res_byteorder_uint32(scenery->material);

    if (0 == io->fread(&scenery->sound_id, sizeof(unsigned char), 1, file)) {
        yc_res_pro_scenery_parse_cleanup(scenery);
        return YC_RES_PRO_STATUS_IO;
    }

    yc_res_pro_object_scenery_data_parser_t *parser = yc_res_pro_parse_scenery_data_parser(scenery);
    yc_res_pro_status_t status = parser(file, io, scenery);

    if (YC_RES_PRO_STATUS_OK != status) {
        yc_res_pro_scenery_parse_cleanup(scenery);
        return status;
    }

    yc_res_pro_scenery_parse_cleanup(NULL);

    into->data.scenery = scenery;
    return YC_RES_PRO_STATUS_OK;
}

yc_res_pro_object_scenery_data_parser_t *yc_res_pro_parse_scenery_data_parser(yc_res_pro_object_scenery_t *from) {
    switch (from->type) {
        case YC_RES_PRO_OBJECT_SCENERY_TYPE_DOOR:
            return &yc_res_pro_object_scenery_door_parse;
        case YC_RES_PRO_OBJECT_SCENERY_TYPE_STAIRS:
        case YC_RES_PRO_OBJECT_SCENERY_TYPE_ELEVATOR:
        case YC_RES_PRO_OBJECT_SCENERY_TYPE_LADDER_BOTTOM:
        case YC_RES_PRO_OBJECT_SCENERY_TYPE_LADDER_TOP:
        case YC_RES_PRO_OBJECT_SCENERY_TYPE_GENERIC:
        default:
            return NULL;
    }
}

void yc_res_pro_scenery_parse_cleanup(yc_res_pro_object_scenery_t *scenery) {
    if (NULL != scenery) { free(scenery); }
}
