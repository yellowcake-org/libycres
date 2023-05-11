#ifndef INCLUDE_DAT_TYPES_H
#define INCLUDE_DAT_TYPES_H

#include <stdint.h>
#include <stdbool.h>

typedef struct yc_res_dat_file {
    char *name;
    bool is_compressed;

    uint32_t offset;
    uint32_t count_plain;
    uint32_t count_compressed;
} yc_res_dat_file_t;

typedef struct yc_res_dat_directory {
    char *path;

    uint32_t count;
    yc_res_dat_file_t *files;
} yc_res_dat_directory_t;

#endif //INCLUDE_DAT_TYPES_H
