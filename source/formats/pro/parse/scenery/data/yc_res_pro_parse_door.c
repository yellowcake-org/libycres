#include <libycres.h>
#include <stdlib.h>

void yc_res_pro_door_parse_cleanup(yc_res_pro_object_scenery_door_t *door);

yc_res_pro_status_t yc_res_pro_object_scenery_door_parse(
        void *file,
        const yc_res_io_fs_api_t *api,
        yc_res_pro_object_scenery_t *into
) {
    yc_res_pro_object_scenery_door_t *door = malloc(sizeof(yc_res_pro_object_scenery_door_t));

    if (NULL == door) {
        yc_res_pro_door_parse_cleanup(door);
        return YC_RES_PRO_STATUS_MEM;
    }

    unsigned char flags_bytes[4] = {0, 0, 0, 0};
    if (0 == api->fread(&flags_bytes, sizeof(flags_bytes), 1, file)) {
        yc_res_pro_door_parse_cleanup(door);
        return YC_RES_PRO_STATUS_IO;
    }
    door->is_passable = (flags_bytes[0] & 0x0F) == 0x0F;

    yc_res_pro_door_parse_cleanup(NULL);

    into->data.door = door;
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_door_parse_cleanup(yc_res_pro_object_scenery_door_t *door) {
    if (NULL != door) { free(door); }
}
