#include <libycres.h>

#include <stdlib.h>
#include <memory.h>

bool yc_res_msg_is_valid_index(uint32_t index) { return index != 0xFFFFFFFF; }

void yc_res_msg_parse_cleanup(
        void *filename, const yc_res_io_fs_api_t *api, yc_res_msg_entries_t *entries, char *values[3]
);

yc_res_msg_status_t yc_res_msg_parse(const char *filename, const yc_res_io_fs_api_t *api, yc_res_msg_parse_result_t *result) {
    void *file = api->fopen(filename, "rb");

    if (NULL == file) {
        yc_res_msg_parse_cleanup(file, api, NULL, NULL);
        return YC_RES_MSG_STATUS_IO;
    }

    yc_res_msg_entries_t *entries = malloc(sizeof(yc_res_msg_entries_t));
    if (NULL == entries) {
        yc_res_msg_parse_cleanup(file, api, NULL, NULL);
        return YC_RES_MSG_STATUS_MEM;
    }

    entries->count = 0;
    entries->pointers = NULL;

    uint8_t value_idx = 0;
    char *values[3] = {NULL, NULL, NULL};

    char buffer = 0;
    bool is_copying = false;

    while (true) {
        size_t consumed = api->fread(&buffer, 1, 1, file);

        if ((false == is_copying && '\n' == buffer) || 0 == consumed) {
            value_idx = 0;

            if (NULL != values[0] || NULL != values[1] || NULL != values[2]) {
                entries->count++;
                size_t entries_size = sizeof(yc_res_msg_entry_t) * entries->count;

                yc_res_msg_entry_t *pointers =
                        NULL == entries->pointers ?
                        malloc(entries_size) : realloc(entries->pointers, entries_size);

                if (NULL == pointers) {
                    yc_res_msg_parse_cleanup(file, api, entries, values);
                    return YC_RES_MSG_STATUS_MEM;
                }

                entries->pointers = pointers;
                yc_res_msg_entry_t *current = &entries->pointers[entries->count - 1];

                current->description = values[2];
                values[2] = NULL;

                current->audio = values[1];
                values[1] = NULL;

                current->index = NULL == values[0] ? 0xFFFFFFFF : (uint32_t) strtoul(values[0], NULL, 10);
                free(values[0]);
                values[0] = NULL;
            }

            if (0 == consumed) { break; }
            continue;
        }

        if (false == is_copying) {
            if ('{' == buffer) {
                is_copying = true;
                continue;
            }
        } else {
            if ('\n' == buffer || '\r' == buffer) { continue; }

            if ('}' == buffer) {
                value_idx++;
                is_copying = false;
                continue;
            }

            if (value_idx >= sizeof(values) / sizeof(values[0])) { continue; }
            char **string = &values[value_idx];

            if (NULL == *string) {
                *string = malloc(1);
                if (NULL == *string) {
                    yc_res_msg_parse_cleanup(file, api, entries, values);
                    return YC_RES_MSG_STATUS_MEM;
                }

                **string = '\0';
            }

            size_t length = strlen(*string);
            char *grown = realloc(*string, length + 1 + 1);

            if (NULL == grown) {
                yc_res_msg_parse_cleanup(file, api, entries, values);
                return YC_RES_MSG_STATUS_MEM;
            }

            grown[length++] = buffer;
            grown[length] = '\0';

            *string = grown;
        }
    }

    yc_res_msg_parse_cleanup(file, api, NULL, values);

    result->entries = entries;
    return YC_RES_MSG_STATUS_OK;
}

void yc_res_msg_parse_cleanup(
        void *file, const yc_res_io_fs_api_t *api, yc_res_msg_entries_t *entries, char *values[3]
) {
    if (NULL != file) { api->fclose(file); }

    if (NULL != values) {
        for (size_t value_idx = 0; value_idx < 3; ++value_idx) {
            char *value = values[value_idx];
            if (NULL != value) { free(value); }
        }
    }

    if (NULL != entries) {
        if (NULL != entries->pointers) {
            for (size_t entry_idx = 0; entry_idx < entries->count; ++entry_idx) {
                yc_res_msg_invalidate(&entries->pointers[entry_idx]);
            }

            free(entries->pointers);
            entries->pointers = NULL;
        }

        entries->count = 0;
        free(entries);
    }
}
