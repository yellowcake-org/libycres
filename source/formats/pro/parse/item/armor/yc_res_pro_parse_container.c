#include <libycres.h>
#include <private.h>

#include <stdlib.h>

void yc_res_pro_container_parse_cleanup(yc_res_pro_object_item_container_t *container);

yc_res_pro_status_t yc_res_pro_object_item_container_parse(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_pro_object_item_t *into
) {
    yc_res_pro_object_item_container_t *container = malloc(sizeof(yc_res_pro_object_item_container_t));

    if (NULL == container) {
        yc_res_pro_container_parse_cleanup(container);
        return YC_RES_PRO_STATUS_MEM;
    }

    if (0 == io->fread(&container->capacity, sizeof(uint32_t), 1, file)) {
        yc_res_pro_container_parse_cleanup(container);
        return YC_RES_PRO_STATUS_IO;
    }
    container->capacity = yc_res_byteorder_uint32(container->capacity);

    uint32_t flags = 0;
    if (0 == io->fread(&flags, sizeof(uint32_t), 1, file)) {
        yc_res_pro_container_parse_cleanup(container);
        return YC_RES_PRO_STATUS_IO;
    }
    flags = yc_res_byteorder_uint32(flags);

    container->flags.no_pickup = 0x01 == (flags & 0x01);
    container->flags.is_grounded = 0x08 == (flags & 0x08);

    into->data.container = container;
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_container_parse_cleanup(yc_res_pro_object_item_container_t *container) {
    if (NULL != container) {
        free(container);
    }
}
