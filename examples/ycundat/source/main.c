#include <ycundat.h>

#include <memory.h>
#include <stdlib.h>
#include <stdbool.h>

// TODO: Remove!
#include "../../../private/byteorder/byteorder.h"

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

typedef struct yc_res_dat_file {
    char *name;
    bool is_compressed;

    uint32_t offset;
    uint32_t count_plain;
    uint32_t count_compressed;
} yc_res_dat_file_t;

typedef struct yc_res_dat_directory {
    char *path;

    uint32_t count;
    yc_res_dat_file_t *files;
} yc_res_dat_directory_t;

int list(const char *filename) {
    FILE *archive = fopen(filename, "rb"); // fclose?
    if (NULL == archive) { return 2; }

    uint32_t dir_count = 0;

    fread(&dir_count, sizeof(dir_count), 1, archive);
    dir_count = yc_res_byteorder_uint32(dir_count);

    printf("Directories: %zu\n", (size_t) dir_count);

    yc_res_dat_directory_t *list = malloc(sizeof(yc_res_dat_directory_t) * dir_count); // free?
    if (NULL == list) {
        fclose(archive);
        return 3;
    }

    fseek(archive, 4 * 3, SEEK_CUR);
    for (u_int32_t dir_num = 0; dir_num < dir_count; ++dir_num) {
        yc_res_dat_directory_t *current_dir = &list[dir_num];

        size_t length = 0;
        fread(&length, 1, 1, archive);

        char **path = &current_dir->path;
        *path = malloc(length + 1); // free?

        if (NULL == *path) {
            fclose(archive);
            return 4;
        }

        fread(*path, length, 1, archive);
        (*path)[length] = 0;
    }

    for (u_int32_t dir_num = 0; dir_num < dir_count; ++dir_num) {
        yc_res_dat_directory_t *current_dir = &list[dir_num];

        uint32_t files_count = 0;

        fread(&files_count, sizeof(files_count), 1, archive);
        files_count = yc_res_byteorder_uint32(files_count);

        current_dir->count = files_count;
        fseek(archive, 4 * 3, SEEK_CUR);

        current_dir->files = malloc(sizeof(yc_res_dat_file_t) * current_dir->count); // free?

        if (NULL == current_dir->files) {
            fclose(archive);
            return 5;
        }

        printf("%s\n", current_dir->path);
        printf("Files: %u\n", current_dir->count);

        for (size_t file_num = 0; file_num < current_dir->count; ++file_num) {
            yc_res_dat_file_t *current_file = &current_dir->files[file_num];

            size_t length = 0;
            fread(&length, 1, 1, archive);

            char **name = &current_file->name;
            *name = malloc(length + 1); // free?

            if (NULL == *name) {
                fclose(archive);
                return 6;
            }

            fread(*name, length, 1, archive);
            (*name)[length] = 0;

            uint32_t attr, offset, size, packed_size = 0;

            fread(&attr, sizeof(attr), 1, archive);
            fread(&offset, sizeof(offset), 1, archive);
            fread(&size, sizeof(size), 1, archive);
            fread(&packed_size, sizeof(packed_size), 1, archive);

            current_file->is_compressed = yc_res_byteorder_uint32(attr) == 0x40;
            current_file->offset = yc_res_byteorder_uint32(offset);
            current_file->count_plain = yc_res_byteorder_uint32(size);
            current_file->count_compressed = yc_res_byteorder_uint32(packed_size);
        }
    }

    fclose(archive);
    return 0;
}
