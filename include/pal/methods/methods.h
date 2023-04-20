#ifndef LIB_YCRES_PAL_METHODS_H
#define LIB_YCRES_PAL_METHODS_H

#include "pal/statuses/statuses.h"
#include "pal/callbacks/callbacks.h"

yc_res_pal_status_t yc_res_pal_parse(
        const char *filename,
        const yc_res_io_fs_api_t *io,
        yc_res_pal_colors_cb_t *callback
);

bool yc_res_pal_color_is_transparent(yc_res_pal_color_t *color);

#endif //LIB_YCRES_PAL_METHODS_H
