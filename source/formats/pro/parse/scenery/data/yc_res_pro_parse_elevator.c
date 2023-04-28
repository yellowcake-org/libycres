#include <libycres.h>
#include <private.h>

#include <stdlib.h>

void yc_res_pro_elevator_parse_cleanup(yc_res_pro_object_scenery_elevator_t *elevator);

yc_res_pro_status_t yc_res_pro_object_scenery_elevator_parse(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_pro_object_scenery_t *into
) {
    yc_res_pro_object_scenery_elevator_t *elevator = malloc(sizeof(yc_res_pro_object_scenery_elevator_t));

    if (NULL == elevator) {
        yc_res_pro_elevator_parse_cleanup(elevator);
        return YC_RES_PRO_STATUS_MEM;
    }


    if (0 == io->fread(&elevator->type, sizeof(uint32_t), 1, file)) {
        yc_res_pro_elevator_parse_cleanup(elevator);
        return YC_RES_PRO_STATUS_IO;
    }
    elevator->type = yc_res_byteorder_uint32(elevator->type);

    uint32_t elevation_raw = 0;
    if (0 == io->fread(&elevation_raw, sizeof(uint32_t), 1, file)) {
        yc_res_pro_elevator_parse_cleanup(elevator);
        return YC_RES_PRO_STATUS_IO;
    }
    elevation_raw = yc_res_byteorder_uint32(elevation_raw);
    elevator->elevation = elevation_raw;

    yc_res_pro_elevator_parse_cleanup(NULL);

    into->data.elevator = elevator;
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_elevator_parse_cleanup(yc_res_pro_object_scenery_elevator_t *elevator) {
    if (NULL != elevator) { free(elevator); }
}
