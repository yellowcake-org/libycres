#include <libycres.h>

#include <stdlib.h>

void yc_res_dat_erase_file(yc_res_dat_file_t *file) {
    free(file->name);
}

void yc_res_dat_erase_directory(yc_res_dat_directory_t *directory) {
    for (uint32_t file_idx = 0; file_idx < directory->count; ++file_idx) {
        yc_res_dat_erase_file(&directory->files[file_idx]);
    }

    free(directory->path);
    free(directory->files);
}
