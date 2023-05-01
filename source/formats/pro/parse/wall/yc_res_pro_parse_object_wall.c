#include <libycres.h>
#include <private.h>

#include <stdlib.h>
#include "../flags/yc_res_pro_parse_object_flags.h"

void yc_res_pro_wall_parse_cleanup(yc_res_pro_object_wall_t *wall);

yc_res_pro_status_t yc_res_pro_object_wall_parse(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_pro_object_t *into
) {
    yc_res_pro_object_wall_t *wall = malloc(sizeof(yc_res_pro_object_wall_t));

    if (NULL == wall) {
        yc_res_pro_wall_parse_cleanup(wall);
        return YC_RES_PRO_STATUS_MEM;
    }

    unsigned char flags_bytes[4] = {0, 0, 0, 0};
    if (0 == io->fread(&flags_bytes, sizeof(flags_bytes), 1, file)) {
        yc_res_pro_wall_parse_cleanup(wall);
        return YC_RES_PRO_STATUS_IO;
    }

    yc_res_pro_parse_action_flags(flags_bytes[0], &wall->flags);
    yc_res_pro_parse_light_passage(flags_bytes[3], &wall->light);

    if (0 == io->fread(&wall->script_id, sizeof(uint32_t), 1, file)) {
        yc_res_pro_wall_parse_cleanup(wall);
        return YC_RES_PRO_STATUS_IO;
    }
    wall->script_id = yc_res_byteorder_uint32(wall->script_id);

    if (0 == io->fread(&wall->material, sizeof(uint32_t), 1, file)) {
        yc_res_pro_wall_parse_cleanup(wall);
        return YC_RES_PRO_STATUS_IO;
    }
    wall->material = yc_res_byteorder_uint32(wall->material);

    into->data.wall = wall;
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_wall_parse_cleanup(yc_res_pro_object_wall_t *wall) {
    if (NULL != wall) { free(wall); }
}
