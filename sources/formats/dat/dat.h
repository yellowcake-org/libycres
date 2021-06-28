#ifndef DAT_PRIVATE_H
#define DAT_PRIVATE_H

#include "platform/platform.h"

void yc_res_dat_private_load_uint(yc_res_platform_reader_t* reader, const void* input,
                                  unsigned int offset, unsigned int* count);

void yc_res_dat_private_load_string(yc_res_platform_reader_t* reader, const void* input,
                                    unsigned int offset, char** value, unsigned int* read);

#endif /* DAT_PRIVATE_H */
