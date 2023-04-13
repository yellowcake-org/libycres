#ifndef LIB_YCRES_DAT_CALLBACKS_H
#define LIB_YCRES_DAT_CALLBACKS_H

#include <stddef.h>

typedef void (yc_res_dat_list_cb_t)(yc_res_dat_directory_t *list, uint32_t count);
typedef void (yc_res_dat_extract_cb_t)(unsigned char *bytes, size_t count);

#endif //LIB_YCRES_DAT_CALLBACKS_H
