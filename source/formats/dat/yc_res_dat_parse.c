#include <libycres.h>
#include <private.h>

#include <stddef.h>
#include <stdlib.h>

yc_res_dat_status_t
yc_res_dat_parse_file(void *archive, const yc_res_io_fs_api_t *api, yc_res_dat_file_t *into);

yc_res_dat_status_t
yc_res_dat_parse_directory(void *archive, const yc_res_io_fs_api_t *api, yc_res_dat_directory_t *into);

void
yc_res_dat_parse_cleanup(void *archive, const yc_res_io_fs_api_t *api, yc_res_dat_directory_t *list);

yc_res_dat_status_t yc_res_dat_parse(
        const char *filename,
        const yc_res_io_fs_api_t *api,
        yc_res_dat_parse_result_t *result
) {
    void *archive = api->fopen(filename, "rb");

    if (NULL == archive) {
        yc_res_dat_parse_cleanup(archive, api, NULL);
        return YC_RES_DAT_STATUS_IO;
    }

    uint32_t dir_count = 0;
    if (0 == api->fread(&dir_count, sizeof(dir_count), 1, archive)) {
        yc_res_dat_parse_cleanup(archive, api, NULL);
        return YC_RES_DAT_STATUS_IO;
    }

    dir_count = yc_res_byteorder_uint32(dir_count);
    yc_res_dat_directory_t *list = calloc(dir_count, sizeof(yc_res_dat_directory_t));

    if (NULL == list) {
        yc_res_dat_parse_cleanup(archive, api, list);
        return YC_RES_DAT_STATUS_MEM;
    }

    if (0 != api->fseek(archive, (long) (4 * 3), SEEK_CUR)) {
        yc_res_dat_parse_cleanup(archive, api, list);
        return YC_RES_DAT_STATUS_IO;
    }

    for (u_int32_t dir_num = 0; dir_num < dir_count; ++dir_num) {
        yc_res_dat_directory_t *current_dir = &list[dir_num];

        size_t length = 0;
        if (0 == api->fread(&length, 1, 1, archive)) {
            yc_res_dat_parse_cleanup(archive, api, list);
            return YC_RES_DAT_STATUS_IO;
        }

        char **path = &current_dir->path;
        *path = malloc(length + 1);

        if (NULL == *path) {
            yc_res_dat_parse_cleanup(archive, api, list);
            return YC_RES_DAT_STATUS_MEM;
        }

        if (0 == api->fread(*path, length, 1, archive)) {
            yc_res_dat_parse_cleanup(archive, api, list);
            return YC_RES_DAT_STATUS_IO;
        }

        (*path)[length] = 0;
    }

    for (u_int32_t dir_num = 0; dir_num < dir_count; dir_num++) {
        yc_res_dat_status_t status = yc_res_dat_parse_directory(archive, api, &list[dir_num]);

        if (YC_RES_DAT_STATUS_OK != status) {
            yc_res_dat_parse_cleanup(archive, api, list);
            return status;
        }
    }

    yc_res_dat_parse_cleanup(archive, api, NULL);

    result->list = list;
    result->count = dir_count;

    return YC_RES_DAT_STATUS_OK;
}

yc_res_dat_status_t yc_res_dat_parse_directory(
        void *archive,
        const yc_res_io_fs_api_t *api,
        yc_res_dat_directory_t *into
) {
    uint32_t files_count = 0;
    if (0 == api->fread(&files_count, sizeof(files_count), 1, archive)) { return YC_RES_DAT_STATUS_IO; }

    files_count = yc_res_byteorder_uint32(files_count);
    into->count = files_count;

    if (0 != api->fseek(archive, (long) (4 * 3), SEEK_CUR)) { return YC_RES_DAT_STATUS_IO; }

    into->files = malloc(sizeof(yc_res_dat_file_t) * into->count);
    if (NULL == into->files) { return YC_RES_DAT_STATUS_MEM; }

    for (size_t file_num = 0; file_num < into->count; file_num++) {
        yc_res_dat_status_t status = yc_res_dat_parse_file(archive, api, &into->files[file_num]);
        if (YC_RES_DAT_STATUS_OK != status) { return status; }
    }

    return YC_RES_DAT_STATUS_OK;
}

yc_res_dat_status_t yc_res_dat_parse_file(
        void *archive,
        const yc_res_io_fs_api_t *api,
        yc_res_dat_file_t *into
) {
    size_t length = 0;
    if (0 == api->fread(&length, 1, 1, archive)) { return YC_RES_DAT_STATUS_IO; }

    char **name = &into->name;
    *name = malloc(length + 1);

    if (NULL == *name) { return YC_RES_DAT_STATUS_MEM; }

    if (0 == api->fread(*name, length, 1, archive)) { return YC_RES_DAT_STATUS_IO; }

    (*name)[length] = 0;

    uint32_t attributes = 0, offset = 0, size = 0, size_packed = 0;

    if (0 == api->fread(&attributes, sizeof(attributes), 1, archive)) { return YC_RES_DAT_STATUS_IO; }
    if (0 == api->fread(&offset, sizeof(offset), 1, archive)) { return YC_RES_DAT_STATUS_IO; }
    if (0 == api->fread(&size, sizeof(size), 1, archive)) { return YC_RES_DAT_STATUS_IO; }
    if (0 == api->fread(&size_packed, sizeof(size_packed), 1, archive)) { return YC_RES_DAT_STATUS_IO; }

    into->is_compressed = yc_res_byteorder_uint32(attributes) == 0x40;

    into->offset = yc_res_byteorder_uint32(offset);
    into->count_plain = yc_res_byteorder_uint32(size);
    into->count_compressed = yc_res_byteorder_uint32(size_packed);

    return YC_RES_DAT_STATUS_OK;
}

void yc_res_dat_parse_cleanup(
        void *archive,
        const yc_res_io_fs_api_t *api,
        yc_res_dat_directory_t *list
) {
    if (NULL != archive) {
        api->fclose(archive);
    }

    if (NULL != list) {
        for (uint32_t dir_idx = 0; dir_idx < list->count; ++dir_idx) {
            yc_res_dat_directory_invalidate(&list[dir_idx]);
        }

        free(list);
    }
}
