#include <libycres.h>

#include <stdlib.h>

void yc_res_dat_file_invalidate(yc_res_dat_file_t *file) {
    free(file->name);
    file->name = NULL;
}

void yc_res_dat_directory_invalidate(yc_res_dat_directory_t *directory) {
    for (uint32_t file_idx = 0; file_idx < directory->count; ++file_idx) {
        yc_res_dat_file_invalidate(&directory->files[file_idx]);
    }

    free(directory->path);
    directory->path = NULL;

    free(directory->files);
    directory->files = NULL;
}
