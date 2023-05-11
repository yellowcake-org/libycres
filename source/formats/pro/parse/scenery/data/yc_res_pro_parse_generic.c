#include <libycres.h>
#include <private.h>

#include <stdlib.h>

void yc_res_pro_generic_parse_cleanup(yc_res_pro_object_scenery_generic_t *generic);

yc_res_pro_status_t yc_res_pro_object_scenery_generic_parse(
        void *file,
        const yc_res_io_fs_api_t *api,
        yc_res_pro_object_scenery_t *into
) {
    yc_res_pro_object_scenery_generic_t *generic = malloc(sizeof(yc_res_pro_object_scenery_generic_t));

    if (NULL == generic) {
        yc_res_pro_generic_parse_cleanup(generic);
        return YC_RES_PRO_STATUS_MEM;
    }

    if (0 == api->fread(&generic->_unknown, sizeof(uint32_t), 1, file)) {
        yc_res_pro_generic_parse_cleanup(generic);
        return YC_RES_PRO_STATUS_IO;
    }
    generic->_unknown = yc_res_byteorder_uint32(generic->_unknown);

    yc_res_pro_generic_parse_cleanup(NULL);

    into->data.generic = generic;
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_generic_parse_cleanup(yc_res_pro_object_scenery_generic_t *generic) {
    if (NULL != generic) { free(generic); }
}
