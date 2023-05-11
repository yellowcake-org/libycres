#include <ycimsg.h>
#include <stdlib.h>

static arg_lit_t *help;
static arg_file_t *input;
static arg_end_t *end;

void *ycimsg_io_fopen(const char *filename, const char *mode);
int ycimsg_io_fclose(void *stream);

int ycimsg_io_fseek(void *stream, long offset, int whence);
size_t ycimsg_io_fread(void *dest, size_t len, size_t cnt, void *str);

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
                .fopen = &ycimsg_io_fopen,
                .fclose = &ycimsg_io_fclose,
                .fseek = &ycimsg_io_fseek,
                .fread = &ycimsg_io_fread,
        };

        yc_res_msg_parse_result_t result = {NULL};
        if (YC_RES_MSG_STATUS_OK != yc_res_msg_parse(filename, &io_api, &result)) {
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

void *ycimsg_io_fopen(const char *filename, const char *mode) { return fopen(filename, mode); }
int ycimsg_io_fclose(void *stream) { return fclose(stream); }

int ycimsg_io_fseek(void *stream, long offset, int whence) { return fseek(stream, offset, whence); }
size_t ycimsg_io_fread(void *dest, size_t len, size_t cnt, void *str) {
    return fread(dest, len, cnt, str);
}
