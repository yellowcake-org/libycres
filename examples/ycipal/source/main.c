#include <ycipal.h>

arg_lit_t *help;
arg_file_t *input;
arg_end_t *end;

void ycipal_parse_cb(yc_res_pal_color_t *colors, size_t count);

void *ycipal_io_fopen(const char *, const char *);
int ycipal_io_fclose(void *);

int ycipal_io_fseek(void *, long, int);
size_t ycipal_io_fread(void *, size_t, size_t, void *);

int main(int argc, char *argv[]) {
    void *arg_table[] = {
            help = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
            input = arg_filen("i", "input", "<color.pal>", 1, 1, "input palette file"),
            end = arg_end(1),
    };

    int exit_code = 0;
    char program_name[] = "ycipal";

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
                .fopen = &ycipal_io_fopen,
                .fclose = &ycipal_io_fclose,
                .fseek = &ycipal_io_fseek,
                .fread = &ycipal_io_fread,
        };

        yc_res_pal_parse_result_t result = {0, NULL};
        if (YC_RES_PAL_STATUS_OK != yc_res_pal_parse(filename, &io_api, &result)) {
            exit_code = 2;
            goto exit;
        }

        ycipal_parse_cb(result.colors, result.count);
    }

    exit:

    arg_freetable(arg_table, sizeof(arg_table) / sizeof(arg_table[0]));
    if (0 != exit_code) { printf("Error occurred, code: %d\n", exit_code); }

    return exit_code;
}

void ycipal_parse_cb(yc_res_pal_color_t *colors, size_t count) {
    size_t trans_count = 0;

    for (size_t idx = 0; idx < count; ++idx) {
        printf("[%zu]  r: %d, g: %d, b: %d\n", idx, colors[idx].r, colors[idx].g, colors[idx].b);
        if (yc_res_pal_color_is_transparent(&colors[idx])) { trans_count++; }
    }

    printf("\n");
    printf("PRESENT: %zu", count - trans_count);
    printf(", ");
    printf("EMPTY: %zu", trans_count);
    printf(", ");
    printf("TOTAL: %zu\n", count);
}

void *ycipal_io_fopen(const char *filename, const char *mode) { return fopen(filename, mode); }
int ycipal_io_fclose(void *stream) { return fclose(stream); }

int ycipal_io_fseek(void *stream, long offset, int whence) { return fseek(stream, offset, whence); }
size_t ycipal_io_fread(void *dest, size_t len, size_t cnt, void *str) {
    return fread(dest, len, cnt, str);
}
