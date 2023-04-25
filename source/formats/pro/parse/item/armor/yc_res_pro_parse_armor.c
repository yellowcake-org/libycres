#include <libycres.h>
#include <private.h>

#include <stdlib.h>

void yc_res_pro_armor_parse_cleanup(yc_res_pro_object_item_armor_t *armor);

yc_res_pro_status_t yc_res_pro_object_item_armor_parse(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_pro_object_item_t *into
) {
    yc_res_pro_object_item_armor_t *armor = malloc(sizeof(yc_res_pro_object_item_armor_t));

    if (NULL == armor) {
        yc_res_pro_armor_parse_cleanup(armor);
        return YC_RES_PRO_STATUS_MEM;
    }

    if (0 == io->fread(&armor->class, sizeof(uint32_t), 1, file)) {
        yc_res_pro_armor_parse_cleanup(armor);
        return YC_RES_PRO_STATUS_IO;
    }
    armor->class = yc_res_byteorder_uint32(armor->class);

    into->data.armor = armor;
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_armor_parse_cleanup(yc_res_pro_object_item_armor_t *armor) {
    if (NULL != armor) {
        free(armor);
    }
}
