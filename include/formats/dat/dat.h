#ifndef DAT_H
#define DAT_H

#include "../../platform/platform.h"

typedef struct {
    char* name;
    
    unsigned long start; /// Position in archive.
    unsigned long size; /// Byte length in archive.
    
    unsigned long original_size; /// Needs decompression when > 0.
} yc_res_dat_file_t;

typedef struct {
    char* name;
    
    unsigned long count;
    yc_res_dat_file_t *files;
} yc_res_dat_directory_t;

/// Writes to output number of directories stored within source.
void yc_res_dat_count(yc_res_platform_reader_t* reader, const void* input, unsigned long* count);

/// Allocates memory for inner fields within provided dirs list and fills them up.
void yc_res_dat_directories(yc_res_platform_reader_t* reader, const void* input,
                            unsigned long count, yc_res_dat_directory_t* directories);

/// Frees inner memory.
void yc_res_dat_free_directory(yc_res_dat_directory_t* directory);

/// Frees inner memory.
void yc_res_dat_free_file(yc_res_dat_file_t* file);

#endif /* DAT_H */
