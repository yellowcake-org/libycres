#include <libycres.h>
#include <stdlib.h>

void yc_res_frm_parse_cleanup(void *file, const yc_res_io_fs_api_t *io, yc_res_frm_sprite_t *sprite);

yc_res_frm_status_t yc_res_frm_sprite_parse(
        const char *filename,
        yc_res_io_fs_api_t *io,
        yc_res_frm_sprite_cb_t *callback
) {
    void *file = io->fopen(filename, "rb");

    if (NULL == file) {
        yc_res_frm_parse_cleanup(NULL, io, NULL);
        return YC_RES_FRM_STATUS_IO;
    }

    yc_res_frm_sprite_t *sprite = malloc(sizeof(yc_res_frm_sprite_t));

    if (NULL == sprite) {
        yc_res_frm_parse_cleanup(file, io, sprite);
        return YC_RES_FRM_STATUS_IO;
    }

    callback(sprite);

    yc_res_frm_parse_cleanup(file, io, NULL);
    return YC_RES_FRM_STATUS_OK;
}

void yc_res_frm_parse_cleanup(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_frm_sprite_t *sprite
) {
    if (NULL != file) {
        io->fclose(file);
    }

    if (NULL != sprite) {
        yc_res_frm_sprite_invalidate(sprite);
        free(sprite);
    }
}
