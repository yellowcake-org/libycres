#include <ycilst.h>

#include <stdlib.h>

static arg_lit_t *help; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
static arg_file_t *input; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
static arg_end_t *end; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

void *ycilst_io_fopen(const char *filename, const char *mode);
int ycilst_io_fclose(void *stream);

int ycilst_io_fseek(void *stream, long offset, int whence);
size_t ycilst_io_fread(void *dest, size_t len, size_t cnt, void *str);

int main(int argc, char *argv[]) {
    void *arg_table[] = {
            help = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
            input = arg_filen("i", "input", "<index.lst>", 1, 1, "input index file"),
            end = arg_end(1),
    };

    int exit_code = 0;
    char program_name[] = "ycilst";

    int errors_count = arg_parse(argc, argv, arg_table);

    if (help->count > 0) {
        printf("Usage: %s", program_name);
        arg_print_syntax(stdout, arg_table, "\n");
        arg_print_glossary(stdout, arg_table, "  %-25s %s\n");

        exit_code = 0;
        goto exit;
    }

    if (errors_count > 0) {
        arg_print_errors(stdout, end, program_name);
        printf("Try '%s --help' for more information.\n", program_name);

        exit_code = 1;
        goto exit;
    }

    if (input->count == 1) {
        const char *filename = input->filename[0];
        yc_res_io_fs_api_t io_api = {
                .fopen = &ycilst_io_fopen,
                .fclose = &ycilst_io_fclose,
                .fseek = &ycilst_io_fseek,
                .fread = &ycilst_io_fread,
        };

        yc_res_lst_entries_t entries = { .count = 0, .pointers = NULL };
        if (YC_RES_LST_STATUS_OK != yc_res_lst_parse(filename, &io_api, &entries)) {
            exit_code = 2;
            goto exit;
        }

        for (size_t entry_idx = 0; entry_idx < entries.count; ++entry_idx) {
            yc_res_lst_entry_t *entry = &entries.pointers[entry_idx];

            printf("[%lu] Value: %s", entry_idx, entry->value);

            if (yc_res_lst_is_valid_index(entry->index)) {
                printf(", index: %d", entry->index);
            }

            printf("\n");
            yc_res_lst_invalidate(entry);
        }

        entries.count = 0;
        free(entries.pointers);
        entries.pointers = NULL;
    }

    exit:

    arg_freetable(arg_table, sizeof(arg_table) / sizeof(arg_table[0]));
    if (0 != exit_code) { printf("Error occurred, code: %d\n", exit_code); }

    return exit_code;
}

void *ycilst_io_fopen(const char *filename, const char *mode) { return fopen(filename, mode); }
int ycilst_io_fclose(void *stream) { return fclose(stream); }

int ycilst_io_fseek(void *stream, long offset, int whence) { return fseek(stream, offset, whence); }
size_t ycilst_io_fread(void *dest, size_t len, size_t cnt, void *str) {
    return fread(dest, len, cnt, str);
}
