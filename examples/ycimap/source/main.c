#include <ycimap.h>

#include <stdlib.h>
#include <memory.h>

#include <sys/stat.h>

arg_lit_t *help;
arg_file_t *input;
arg_end_t *end;

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
                .fopen = (yc_res_io_fopen_t *) &fopen,
                .fclose = (yc_res_io_fclose_t *) &fclose,
                .fseek = (yc_res_io_fseek_t *) &fseek,
                .fread = (yc_res_io_fread_t *) &fread,
        };

        yc_res_map_parse_result_t result = {NULL};
        yc_res_map_status_t status = yc_res_map_parse(filename, &io_api, &result);

        if (YC_RES_MAP_STATUS_OK != status) {
            exit_code = 2;
            goto exit;
        }

        printf("Map index: %d\n", result.map->map_idx);
        if (result.map->is_save) { printf("This is save file.\n"); }

        if (yc_res_pro_is_valid_id(result.map->script_id)) {
            printf("\n");
            printf("Script type: %d\n", yc_res_pro_script_type_from_sid(result.map->script_id));
            printf("Script type: %d\n", yc_res_pro_index_from_script_id(result.map->script_id));
        }

        printf("\n");
        printf("Ticks passed: %d\n", result.map->ticks);

        if (yc_res_pro_is_valid_destination(result.map->entrance)) {
            printf("\n");
            printf("Entrance details:\n");
            printf("Elevation: %d\n", result.map->entrance.elevation_idx);
            printf("Tile index: %d\n", result.map->entrance.tile_idx);
            printf("Orientation: %d\n", result.map->entrance.orientation);
        }

        printf("Global vars: %lu\n", result.map->count_global_variables);
        printf("Local vars: %lu\n", result.map->count_local_variables);

        printf("\n");
        for (size_t elevation_idx = 0; elevation_idx < YC_RES_MAP_ELEVATION_COUNT; ++elevation_idx) {
            yc_res_map_level_t *level = result.map->levels[elevation_idx];

            if (NULL != level) {
                printf("Level #%lu presented.\n\n", elevation_idx + 1);
            }
        }
    }

    exit:

    arg_freetable(arg_table, sizeof(arg_table) / sizeof(arg_table[0]));
    if (0 != exit_code) { printf("Error occurred, code: %d\n", exit_code); }

    return exit_code;
}
