#include <libycres.h>
#include <private.h>

#include <stdlib.h>

void yc_res_pro_key_parse_cleanup(yc_res_pro_object_item_key_t *key);

yc_res_pro_status_t yc_res_pro_object_item_key_parse(
        void *file,
        const yc_res_io_fs_api_t *api,
        yc_res_pro_object_item_t *into
) {
    yc_res_pro_object_item_key_t *key = malloc(sizeof(yc_res_pro_object_item_key_t));

    if (NULL == key) {
        yc_res_pro_key_parse_cleanup(key);
        return YC_RES_PRO_STATUS_MEM;
    }

    if (0 == api->fread(&key->code, sizeof(uint32_t), 1, file)) {
        yc_res_pro_key_parse_cleanup(key);
        return YC_RES_PRO_STATUS_IO;
    }
    key->code = yc_res_byteorder_uint32(key->code);

    into->data.key = key;
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_key_parse_cleanup(yc_res_pro_object_item_key_t *key) {
    if (NULL != key) {
        free(key);
    }
}
