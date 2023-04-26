#include <libycres.h>
#include <private.h>

#include <stdlib.h>

void yc_res_pro_misc_parse_cleanup(yc_res_pro_object_item_misc_t *misc);

yc_res_pro_status_t yc_res_pro_object_item_misc_parse(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_pro_object_item_t *into
) {
    yc_res_pro_object_item_misc_t *misc = malloc(sizeof(yc_res_pro_object_item_misc_t));

    if (NULL == misc) {
        yc_res_pro_misc_parse_cleanup(misc);
        return YC_RES_PRO_STATUS_MEM;
    }

    if (0 == io->fread(&misc->item_idx, sizeof(uint32_t), 1, file)) {
        yc_res_pro_misc_parse_cleanup(misc);
        return YC_RES_PRO_STATUS_IO;
    }
    misc->item_idx = yc_res_byteorder_uint32(misc->item_idx);

    if (0 == io->fread(&misc->caliber, sizeof(yc_res_pro_caliber_t), 1, file)) {
        yc_res_pro_misc_parse_cleanup(misc);
        return YC_RES_PRO_STATUS_IO;
    }
    misc->caliber = yc_res_byteorder_int32(misc->caliber);

    if (0 == io->fread(&misc->count, sizeof(uint32_t), 1, file)) {
        yc_res_pro_misc_parse_cleanup(misc);
        return YC_RES_PRO_STATUS_IO;
    }
    misc->count = yc_res_byteorder_uint32(misc->count);

    into->data.misc = misc;
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_misc_parse_cleanup(yc_res_pro_object_item_misc_t *misc) {
    if (NULL != misc) {
        free(misc);
    }
}
