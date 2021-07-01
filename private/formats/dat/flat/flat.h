#ifndef LIBYCRES_PRIVATE_DAT_FLAT_H
#define LIBYCRES_PRIVATE_DAT_FLAT_H

#include <libycres.h>

void yc_res_dat_private_flatten_marked_dirs(yc_res_dat_directory_t* node,
                                            yc_res_dat_directory_t** flat, unsigned long* appended);

#endif /* LIBYCRES_PRIVATE_DAT_FLAT_H */
