#include <libycres.h>
#include <stdlib.h>
#include <memory.h>

bool yc_res_lst_is_valid_index(uint32_t fallback_idx) { return fallback_idx != 0xFFFFFFFF; }

void yc_res_lst_parse_cleanup(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_lst_entries_t *entries,
        char *value, char *index
);

yc_res_lst_status_t yc_res_lst_parse(const char *filename, const yc_res_io_fs_api_t *io, yc_res_lst_parse_result_t *result) {
    void *file = io->fopen(filename, "rb");

    if (NULL == file) {
        yc_res_lst_parse_cleanup(file, io, NULL, NULL, NULL);
        return YC_RES_LST_STATUS_IO;
    }

    yc_res_lst_entries_t *entries = malloc(sizeof(yc_res_lst_entries_t));
    if (NULL == entries) {
        yc_res_lst_parse_cleanup(file, io, NULL, NULL, NULL);
        return YC_RES_LST_STATUS_MEM;
    }

    entries->count = 0;
    entries->pointers = NULL;

    size_t line_idx = 0;
    bool has_reached_end = false, has_read_value = false;

    char buffer = 0;
    char *value = NULL, *index = NULL;

    while (true) {
        size_t consumed = io->fread(&buffer, 1, 1, file);

        if ('\n' == buffer || 0 == consumed) {
            has_read_value = false;
            has_reached_end = false;

            entries->count = ++line_idx;
            size_t entries_size = sizeof(yc_res_lst_entry_t) * entries->count;

            yc_res_lst_entry_t *pointers =
                    NULL == entries->pointers ?
                    malloc(entries_size) : realloc(entries->pointers, entries_size);

            if (NULL == pointers) {
                yc_res_lst_parse_cleanup(file, io, entries, value, index);
                return YC_RES_LST_STATUS_MEM;
            }

            entries->pointers = pointers;
            yc_res_lst_entry_t *current = &entries->pointers[entries->count - 1];

            current->value = value;
            value = NULL;

            current->index = NULL == index ? 0xFFFFFFFF : (uint32_t) strtoul(index, NULL, 10);

            free(index);
            index = NULL;

            if (0 == consumed) { break; } else { continue; }
        }

        if ('\r' == buffer || '\t' == buffer || ' ' == buffer || ';' == buffer) {
            has_reached_end = true;
            continue;
        }

        // still reading current entry
        if (false == has_reached_end) {
            if (',' == buffer) {
                has_read_value = true;
                continue;
            }

            char **string = false == has_read_value ? &value : &index;

            if (NULL == *string) {
                *string = malloc(1);
                if (NULL == *string) {
                    yc_res_lst_parse_cleanup(file, io, entries, value, index);
                    return YC_RES_LST_STATUS_MEM;
                }

                **string = '\0';
            }

            size_t length = strlen(*string);
            char *grown = realloc(*string, length + 1 + 1);

            if (NULL == grown) {
                yc_res_lst_parse_cleanup(file, io, entries, value, index);
                return YC_RES_LST_STATUS_MEM;
            }

            grown[length++] = buffer;
            grown[length] = '\0';

            *string = grown;
        }
    }

    yc_res_lst_parse_cleanup(file, io, NULL, value, index);

    result->entries = entries;
    return YC_RES_LST_STATUS_OK;
}

void yc_res_lst_parse_cleanup(
        void *file, const yc_res_io_fs_api_t *io, yc_res_lst_entries_t *entries,
        char *value, char *index
) {
    if (NULL != file) { io->fclose(file); }

    if (NULL != value) { free(value); }
    if (NULL != index) { free(index); }

    if (NULL != entries) {
        if (NULL != entries->pointers) {
            for (size_t entry_idx = 0; entry_idx < entries->count; ++entry_idx) {
                yc_res_lst_invalidate(&entries->pointers[entry_idx]);
            }

            free(entries->pointers);
            entries->pointers = NULL;
        }

        entries->count = 0;
    }
}
