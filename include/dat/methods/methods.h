#ifndef LIB_YCRES_DAT_METHODS_H
#define LIB_YCRES_DAT_METHODS_H

#include "dat/statuses/statuses.h"
#include "dat/callbacks/callbacks.h"

yc_res_dat_status_t yc_res_dat_parse(const char *filename, yc_res_dat_list_cb_t *callback);

#endif //LIB_YCRES_DAT_METHODS_H
