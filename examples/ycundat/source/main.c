#include <ycundat.h>
#include <memory.h>

struct arg_lit *help;
struct arg_file *input, *output;
struct arg_end *end;

void ycundat_cb_dat_parse(yc_res_dat_directory_t *list, uint32_t count);

int main(int argc, char *argv[]) {
    void *arg_table[] = {
            help = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
            input = arg_filen("i", "input", "<file.dat>", 1, 1, "input file"),
            output = arg_filen("o", "output", "<directory>", 1, 1, "output directory"),
            end = arg_end(2),
    };

    int exit_code = 0;
    char program_name[] = "ycundat";

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

        if (YC_RES_DAT_STATUS_OK != yc_res_dat_parse(filename, &ycundat_cb_dat_parse)) {
            exit_code = 2;
        }
    }

    exit:
    arg_freetable(arg_table, sizeof(arg_table) / sizeof(arg_table[0]));

    return exit_code;
}

void ycundat_cb_dat_parse(yc_res_dat_directory_t *list, uint32_t count) {
    for (uint32_t dir_idx = 0; dir_idx < count; ++dir_idx) {
        printf("%s\n", list[dir_idx].path);
    }
}
