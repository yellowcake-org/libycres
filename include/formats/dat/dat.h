#ifndef DAT_H
#define DAT_H

#include "../../platform/platform.h"

typedef struct {
    char* name;
} yc_res_dat_directory_t;

/// Writes to output number of directories stored within source.
void yc_res_dat_count(yc_res_platform_reader_t* reader, const void* input, unsigned long* count);

/// Allocates memory for inner fields within provided dirs list and fills them up.
void yc_res_dat_directories(yc_res_platform_reader_t* reader, const void* input,
                            unsigned long count, yc_res_dat_directory_t* directories);

/// Frees inner fields' memory.
void yc_res_dat_directory_free(yc_res_dat_directory_t *directory);

#endif /* DAT_H */
