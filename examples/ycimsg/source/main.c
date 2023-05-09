#include <ycimsg.h>

#include <stdlib.h>
#include <memory.h>

#include <sys/stat.h>

arg_lit_t *help;
arg_file_t *input;
arg_end_t *end;

int main(int argc, char *argv[]) {
    void *arg_table[] = {
            help = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
            input = arg_filen("i", "input", "<texts.msg>", 1, 1, "input messages file"),
            end = arg_end(1),
    };

    int exit_code = 0;
    char program_name[] = "ycimsg";

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

        yc_res_msg_parse_result_t result = {NULL};
        if (YC_RES_PAL_STATUS_OK != yc_res_msg_parse(filename, &io_api, &result)) {
            exit_code = 2;
            goto exit;
        }

        for (size_t entry_idx = 0; entry_idx < result.entries->count; ++entry_idx) {
            yc_res_msg_entry_t *entry = &result.entries->pointers[entry_idx];

            printf(
                    "[%lu] Index: %d, Audio: %s, Description: %s\n",
                    entry_idx,
                    entry->index,
                    entry->audio,
                    entry->description
            );

            yc_res_msg_invalidate(entry);
        }
        
        result.entries->pointers = NULL;
        result.entries->count = 0;
        
        free(result.entries);
        result.entries = NULL;
    }

    exit:

    arg_freetable(arg_table, sizeof(arg_table) / sizeof(arg_table[0]));
    if (0 != exit_code) { printf("Error occurred, code: %d\n", exit_code); }

    return exit_code;
}
