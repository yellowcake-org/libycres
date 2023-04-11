#include <ycundat.h>

#include <memory.h>
#include <stdlib.h>

/* global arg_xxx structs */
struct arg_lit *help;
struct arg_file *input, *output;
struct arg_end *end;

int list(const char *filename);

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
        exit_code = list(input->filename[0]);
    }

    exit:
    arg_freetable(arg_table, sizeof(arg_table) / sizeof(arg_table[0]));

    return exit_code;
}

int list(const char *filename) {
    FILE *archive = fopen(filename, "rb");
    if (NULL == archive) { return 2; }

    unsigned char dir_count_bytes[4];
    fread(&dir_count_bytes, 4, 1, archive);

    // TODO: Move out BE/LE conversions.
    u_int32_t dir_count = 0;
    memcpy((char *) &dir_count + 0, &dir_count_bytes[3], 1);
    memcpy((char *) &dir_count + 1, &dir_count_bytes[2], 1);
    memcpy((char *) &dir_count + 2, &dir_count_bytes[1], 1);
    memcpy((char *) &dir_count + 3, &dir_count_bytes[0], 1);

    printf("Directories: %zu\n", (size_t) dir_count);

    fseek(archive, 4 * 3, SEEK_CUR);

    // TODO: Create appropriate structures.
    for (u_int32_t dir_num = 0; dir_num < dir_count; ++dir_num) {
        char length = 0;
        fread(&length, 1, 1, archive);

        char *path = malloc((size_t) length + 1);
        if (NULL == path) {
            fclose(archive);
            return 3;
        }

        fread(path, length, 1, archive);
        path[(size_t) length] = 0;

        printf("%s\n", path);
        free(path);
    }

    // TODO: Iterate over files' entries.
    for (u_int32_t dir_num = 0; dir_num < dir_count; ++dir_num) {
        //
    }

    fclose(archive);
    return 0;
}
