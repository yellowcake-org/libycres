#ifndef LIBYCRES_PRIVATE_DAT_H
#define LIBYCRES_PRIVATE_DAT_H

#include <libycres.h>

void yc_res_dat_private_load_count(yc_res_platform_reader_t* reader, void* input,
                                   unsigned long offset, unsigned long* value, unsigned long* read);

void yc_res_dat_private_load_string(yc_res_platform_reader_t* reader, void* input, unsigned long offset,
                                    char** value, unsigned long *length, unsigned long* read);

void yc_res_dat_private_flatten_marked_dirs(yc_res_dat_directory_t* node,
                                            yc_res_dat_directory_t** flat, unsigned long* appended);

#endif /* LIBYCRES_PRIVATE_DAT_H */
