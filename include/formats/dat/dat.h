#ifndef DAT_H
#define DAT_H

#include "../../platform/platform.h"

/// Writes to output number of directories stored within source.
void yc_res_dat_count(yc_res_platform_reader_t* reader, const void* input, unsigned long* output);

#endif /* DAT_H */
