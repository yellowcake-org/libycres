#include <ycundat.h>

#include <memory.h>
#include <stdlib.h>

#include <sys/stat.h>

static arg_lit_t *help;
static arg_file_t *input, *output;
static arg_end_t *end;

static void mkdir_recursive(const char *path, size_t length);

void *ycundat_io_fopen(const char *filename, const char *mode);

int ycundat_io_fclose(void *stream);

int ycundat_io_fseek(void *stream, long offset, int whence);

size_t ycundat_io_fread(void *dest, size_t len, size_t cnt, void *str);

void ycundat_cb_extract(unsigned char *bytes, size_t count, void *file);

int main(int argc, char *argv[]) {
    void *arg_table[] = {
            help = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
            input = arg_filen("i", "input", "<archive.dat>", 1, 1, "input file"),
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
        yc_res_io_fs_api_t io_api = {
                .fopen = &ycundat_io_fopen,
                .fclose = &ycundat_io_fclose,
                .fseek = &ycundat_io_fseek,
                .fread = &ycundat_io_fread,
        };

        yc_res_dat_parse_result_t result = {0, NULL};
        if (YC_RES_DAT_STATUS_OK != yc_res_dat_parse(filename, &io_api, &result)) {
            exit_code = 2;
            goto exit;
        }

        if (NULL == result.list) {
            exit_code = 3;
            goto exit;
        }

        for (uint32_t dir_idx = 0; dir_idx < result.count; ++dir_idx) {
            yc_res_dat_directory_t *directory = &result.list[dir_idx];

            if (NULL == directory->files) {
                exit_code = 3;
                goto exit_iteration;
            }

            for (uint32_t file_idx = 0; file_idx < directory->count; ++file_idx) {
                yc_res_dat_file_t *file = &directory->files[file_idx];

                if (NULL == file) {
                    exit_code = 3;
                    goto exit_iteration;
                }

                char *destination = malloc(
                        strlen(*output->filename) + 1 +
                        strlen(directory->path) + 1 +
                        strlen(file->name) + 1
                );

                if (NULL == destination) {
                    exit_code = 3;
                    goto exit_iteration;
                }

                strncpy(destination, *output->filename, strlen(*output->filename) + 1);
                strncat(&destination[strlen(*output->filename)], "/", 1 + 1);
                strncat(&destination[strlen(*output->filename) + 1], directory->path, strlen(directory->path) + 1);

                for (size_t i = 0; i < strlen(destination); ++i) {
                    if (destination[i] == '\\') { destination[i] = '/'; }
                }

                mkdir_recursive(destination, strlen(destination));

                strncat(&destination[strlen(*output->filename) + 1 + strlen(directory->path)], "/", 1 + 1);
                strncat(&destination[strlen(*output->filename) + 1 + strlen(directory->path) + 1],
                        file->name,
                        strlen(file->name) + 1
                );

                printf("%s\n", destination);

                FILE *exporting;
                exporting = fopen(destination, "wb");

                free(destination);

                if (NULL == exporting) {
                    exit_code = 4;
                    goto exit_iteration;
                }

                yc_res_dat_extract_api_t ext_result = {
                        &ycundat_cb_extract,
                        exporting
                };

                if (YC_RES_DAT_STATUS_OK != yc_res_dat_extract(
                        filename,
                        &io_api,
                        file,
                        &ext_result
                )) {
                    fclose(exporting);
                    exporting = NULL;

                    exit_code = 4;
                    goto exit;
                }

                fclose(exporting);
                exporting = NULL;
            }

            yc_res_dat_directory_invalidate(directory);
        }

        exit_iteration:
        result.count = 0;

        free(result.list);
        result.list = NULL;
    }

    exit:

    arg_freetable(arg_table, sizeof(arg_table) / sizeof(arg_table[0]));
    if (0 != exit_code) { printf("Error occurred, code: %d\n", exit_code); }

    return exit_code;
}

void ycundat_cb_extract(unsigned char *bytes, size_t count, void *file) {
    fwrite(bytes, count, 1, file);
    free(bytes);
}

static void mkdir_recursive(const char *path, size_t length) {
    char *copied_path = malloc(length + 1);
    if (NULL == copied_path) { return; }

    snprintf(copied_path, length + 1, "%s", path);
    if (copied_path[length] == '/') { copied_path[length] = 0; }

    for (size_t char_idx = 0; char_idx <= length; ++char_idx) {
        if ('/' == copied_path[char_idx]) {
            copied_path[char_idx] = '\0';
            mkdir(copied_path, S_IRWXU);
            copied_path[char_idx] = '/';
        }
    }

    mkdir(copied_path, S_IRWXU);
    free(copied_path);
}

void *ycundat_io_fopen(const char *filename, const char *mode) { return fopen(filename, mode); }

int ycundat_io_fclose(void *stream) { return fclose(stream); }

int ycundat_io_fseek(void *stream, long offset, int whence) { return fseek(stream, offset, whence); }

size_t ycundat_io_fread(void *dest, size_t len, size_t cnt, void *str) {
    return fread(dest, len, cnt, str);
}
