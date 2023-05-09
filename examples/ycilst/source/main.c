#include <ycilst.h>

#include <stdlib.h>
#include <memory.h>

#include <sys/stat.h>

arg_lit_t *help;
arg_file_t *input;
arg_end_t *end;

void ycipal_parse_cb(yc_res_pal_color_t *colors, size_t count);

int main(int argc, char *argv[]) {
    void *arg_table[] = {
            help = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
            input = arg_filen("i", "input", "<index.lst>", 1, 1, "input index file"),
            end = arg_end(1),
    };

    int exit_code = 0;
    char program_name[] = "ycilst";

    int errors_count;
    errors_count = arg_parse(argc, argv, arg_table);

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
                .fopen = (yc_res_io_fopen_t *) &fopen,
                .fclose = (yc_res_io_fclose_t *) &fclose,
                .fseek = (yc_res_io_fseek_t *) &fseek,
                .fread = (yc_res_io_fread_t *) &fread,
        };

        yc_res_lst_parse_result_t result = {NULL};
        if (YC_RES_PAL_STATUS_OK != yc_res_lst_parse(filename, &io_api, &result)) {
            exit_code = 2;
            goto exit;
        }

        for (size_t entry_idx = 0; entry_idx < result.entries->count; ++entry_idx) {
            yc_res_lst_entry_t *entry = &result.entries->pointers[entry_idx];

            printf("[%lu] Value: %s", entry_idx, entry->value);

            if (yc_res_lst_is_valid_index(entry->index)) {
                printf(", index: %d", entry->index);
            }

            printf("\n");
        }
    }

    exit:

    arg_freetable(arg_table, sizeof(arg_table) / sizeof(arg_table[0]));
    if (0 != exit_code) { printf("Error occurred, code: %d\n", exit_code); }

    return exit_code;
}
