#include <libycres.h>
#include <private.h>

#include "yc_res_pro_parse_object_scenery.h"
#include "../flags/yc_res_pro_parse_object_flags.h"

#include <stdlib.h>

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


    yc_res_pro_object_scenery_data_parser_t *all[] = {
            &yc_res_pro_object_scenery_door_parse,
            &yc_res_pro_object_scenery_stairs_parse,
            &yc_res_pro_object_scenery_elevator_parse,
            &yc_res_pro_object_scenery_ladder_parse,
            &yc_res_pro_object_scenery_ladder_parse,
            &yc_res_pro_object_scenery_generic_parse,
    };

    yc_res_pro_object_scenery_data_parser_t *fitting = all[scenery->type];
    yc_res_pro_status_t status = fitting(file, io, scenery);

    if (YC_RES_PRO_STATUS_OK != status) {
        yc_res_pro_scenery_parse_cleanup(scenery);
        return status;
    }

    yc_res_pro_scenery_parse_cleanup(NULL);

    into->data.scenery = scenery;
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_scenery_parse_cleanup(yc_res_pro_object_scenery_t *scenery) {
    if (NULL != scenery) { free(scenery); }
}
