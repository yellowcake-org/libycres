#ifndef DAT_PRIVATE_H
#define DAT_PRIVATE_H

#include "../../../include/platform/platform.h"
#include "../../../include/formats/dat/dat.h"

void yc_res_dat_private_load_count(yc_res_platform_reader_t* reader, const void* input,
                                   unsigned long offset, unsigned long* value, unsigned long* read);

void yc_res_dat_private_load_string(yc_res_platform_reader_t* reader, const void* input,
                                    unsigned long offset, char** value, unsigned long* read);

void yc_res_dat_private_flatten_marked_dirs(yc_res_dat_directory_t* node,
                                            yc_res_dat_directory_t** flat, unsigned long* appended);

#endif /* DAT_PRIVATE_H */
