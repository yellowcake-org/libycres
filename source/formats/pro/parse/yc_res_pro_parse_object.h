#ifndef LIB_YCRES_PRO_PARSE_OBJECT_H
#define LIB_YCRES_PRO_PARSE_OBJECT_H

#include <libycres.h>

yc_res_pro_status_t yc_res_pro_object_item_parse(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_pro_object_t *into
);

#endif //LIB_YCRES_PRO_PARSE_OBJECT_H
