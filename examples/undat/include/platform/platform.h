#ifndef UNDAT_PLATFORM_H
#define UNDAT_PLATFORM_H

#include <libycres.h>

yc_res_dat_platform_reader_status_t
undat_platform_file_reader(void* input, unsigned long offset, unsigned long length, unsigned char* output);

#endif /* UNDAT_PLATFORM_H */
