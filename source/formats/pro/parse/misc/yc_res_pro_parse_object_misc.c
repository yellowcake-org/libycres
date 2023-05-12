#include <libycres.h>
#include <private.h>

#include <stdlib.h>

void yc_res_pro_misc_parse_cleanup(yc_res_pro_object_misc_t *misc);

yc_res_pro_status_t yc_res_pro_object_misc_parse(
        void *file,
        const yc_res_io_fs_api_t *api,
        yc_res_pro_object_t *into
) {
    yc_res_pro_object_misc_t *misc = malloc(sizeof(yc_res_pro_object_misc_t));

    if (NULL == misc) {
        yc_res_pro_misc_parse_cleanup(misc);
        return YC_RES_PRO_STATUS_MEM;
    }

    if (0 == api->fread(&misc->unknown, sizeof(uint32_t), 1, file)) {
        yc_res_pro_misc_parse_cleanup(misc);
        return YC_RES_PRO_STATUS_IO;
    }
    misc->unknown = yc_res_byteorder_uint32(misc->unknown);

    into->data.misc = misc;
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_misc_parse_cleanup(yc_res_pro_object_misc_t *misc) {
    if (NULL != misc) { free(misc); }
}
