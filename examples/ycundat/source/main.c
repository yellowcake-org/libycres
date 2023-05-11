#include <ycundat.h>

#include <stdlib.h>
#include <memory.h>

#include <sys/stat.h>

static arg_lit_t *help;
static arg_file_t *input, *output;
static arg_end_t *end;

static void mkdir_recursive(const char *dir);

void *yciundat_io_fopen(const char *filename, const char *mode);
int yciundat_io_fclose(void *stream);

int yciundat_io_fseek(void *stream, long offset, int whence);
size_t yciundat_io_fread(void *dest, size_t len, size_t cnt, void *str);

void ycundat_cb_parse(yc_res_dat_directory_t *list, uint32_t count);

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
                .fopen = &yciundat_io_fopen,
                .fclose = &yciundat_io_fclose,
                .fseek = &yciundat_io_fseek,
                .fread = &yciundat_io_fread,
        };

        yc_res_dat_parse_result_t result = {0, NULL};
        if (YC_RES_DAT_STATUS_OK != yc_res_dat_parse(filename, &io_api, &result)) {
            exit_code = 2;
            goto exit;
        }

        for (uint32_t dir_idx = 0; dir_idx < result.count; ++dir_idx) {
            yc_res_dat_directory_t *directory = &result.list[dir_idx];

            for (uint32_t file_idx = 0; file_idx < directory->count; ++file_idx) {
                yc_res_dat_file_t *file = &directory->files[file_idx];

                char *destination = malloc(
                        strlen(*output->filename) + 1 +
                        strlen(directory->path) + 1 +
                        strlen(file->name) + 1
                );

                strcpy(destination, *output->filename);
                strcat(destination, "/");
                strcat(destination, directory->path);

                for (size_t i = 0; i < strlen(destination); ++i) {
                    if (destination[i] == '\\') { destination[i] = '/'; }
                }

                mkdir_recursive(destination);

                strcat(destination, "/");
                strcat(destination, file->name);

                printf("%s\n", destination);

                FILE *exporting = fopen(destination, "wb");
                free(destination);

                if (NULL == exporting) {
                    exit_code = 3;
                    goto exit;
                }

                yc_res_dat_extract_result_t ext_result = {
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

            yc_res_dat_directory_invalidate(&result.list[dir_idx]);
        }

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

static void mkdir_recursive(const char *dir) {
    char temp[256];
    char *iterator = NULL;

    snprintf(temp, sizeof(temp), "%s", dir);
    size_t length = strlen(temp);

    if (temp[length - 1] == '/') { temp[length - 1] = 0; }

    for (iterator = temp + 1; *iterator; iterator++)
        if (*iterator == '/') {
            *iterator = 0;
            mkdir(temp, S_IRWXU);
            *iterator = '/';
        }

    mkdir(temp, S_IRWXU);
}

void *yciundat_io_fopen(const char *filename, const char *mode) { return fopen(filename, mode); }
int yciundat_io_fclose(void *stream) { return fclose(stream); }

int yciundat_io_fseek(void *stream, long offset, int whence) { return fseek(stream, offset, whence); }
size_t yciundat_io_fread(void *dest, size_t len, size_t cnt, void *str) {
    return fread(dest, len, cnt, str);
}
