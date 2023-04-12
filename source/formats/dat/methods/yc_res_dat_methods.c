#include <libycres.h>
#include <private.h>

#include <stdio.h>
#include <stdlib.h>

yc_res_dat_status_t yc_res_dat_parse(const char *filename, yc_res_dat_list_cb_t *callback) {
    FILE *archive = fopen(filename, "rb"); // TODO: fclose?

    if (NULL == archive) {
        return YC_RES_DAT_STATUS_IO;
    }

    uint32_t dir_count = 0;
    if (0 == fread(&dir_count, sizeof(dir_count), 1, archive)) {
        return YC_RES_DAT_STATUS_IO;
    }

    dir_count = yc_res_byteorder_uint32(dir_count);
    yc_res_dat_directory_t *list = malloc(sizeof(yc_res_dat_directory_t) * dir_count); // TODO: free?

    if (NULL == list) {
        return YC_RES_DAT_STATUS_MEM;
    }

    if (0 != fseek(archive, 4 * 3, SEEK_CUR)) {
        return YC_RES_DAT_STATUS_IO;
    }

    for (u_int32_t dir_num = 0; dir_num < dir_count; ++dir_num) {
        yc_res_dat_directory_t *current_dir = &list[dir_num];

        size_t length = 0;
        if (0 == fread(&length, 1, 1, archive)) {
            return YC_RES_DAT_STATUS_IO;
        }

        char **path = &current_dir->path;
        *path = malloc(length + 1); // TODO: free?

        if (NULL == *path) {
            return YC_RES_DAT_STATUS_MEM;
        }

        if (0 == fread(*path, length, 1, archive)) {
            return YC_RES_DAT_STATUS_IO;
        }

        (*path)[length] = 0;
    }

    for (u_int32_t dir_num = 0; dir_num < dir_count; ++dir_num) {
        yc_res_dat_directory_t *current_dir = &list[dir_num];

        uint32_t files_count = 0;

        if (0 == fread(&files_count, sizeof(files_count), 1, archive)) {
            return YC_RES_DAT_STATUS_IO;
        }

        files_count = yc_res_byteorder_uint32(files_count);
        current_dir->count = files_count;

        if (0 != fseek(archive, 4 * 3, SEEK_CUR)) {
            return YC_RES_DAT_STATUS_IO;
        }

        current_dir->files = malloc(sizeof(yc_res_dat_file_t) * current_dir->count); // TODO: free?

        if (NULL == current_dir->files) {
            return YC_RES_DAT_STATUS_MEM;
        }

        for (size_t file_num = 0; file_num < current_dir->count; ++file_num) {
            yc_res_dat_file_t *current_file = &current_dir->files[file_num];

            size_t length = 0;
            if (0 == fread(&length, 1, 1, archive)) {
                return YC_RES_DAT_STATUS_IO;
            }

            char **name = &current_file->name;
            *name = malloc(length + 1); // TODO: free?

            if (NULL == *name) {
                return YC_RES_DAT_STATUS_MEM;
            }

            fread(*name, length, 1, archive); // TODO: check result?
            (*name)[length] = 0;

            uint32_t attributes, offset, size, size_packed = 0;

            if (0 == fread(&attributes, sizeof(attributes), 1, archive)) { return YC_RES_DAT_STATUS_IO; }
            if (0 == fread(&offset, sizeof(offset), 1, archive)) { return YC_RES_DAT_STATUS_IO; }
            if (0 == fread(&size, sizeof(size), 1, archive)) { return YC_RES_DAT_STATUS_IO; };
            if (0 == fread(&size_packed, sizeof(size_packed), 1, archive)) { return YC_RES_DAT_STATUS_IO; }

            current_file->is_compressed = yc_res_byteorder_uint32(attributes) == 0x40;

            current_file->offset = yc_res_byteorder_uint32(offset);
            current_file->count_plain = yc_res_byteorder_uint32(size);
            current_file->count_compressed = yc_res_byteorder_uint32(size_packed);
        }
    }

    callback(list, dir_count);
    fclose(archive);

    return YC_RES_DAT_STATUS_OK;
}
