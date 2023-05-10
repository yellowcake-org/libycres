#include <ycimap.h>

#include <stdlib.h>
#include <memory.h>

#include <sys/stat.h>

arg_lit_t *help;
arg_file_t *input, *resources;
arg_end_t *end;

yc_res_pro_object_item_type_t ycimap_fetch_items_type(uint32_t pid, void *context);

yc_res_pro_object_scenery_type_t ycimap_fetch_scenery_type(uint32_t pid, void *context);

int main(int argc, char *argv[]) {
    void *arg_table[] = {
            help = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
            input = arg_filen("i", "input", "<vault13.map>", 1, 1, "input map file"),
            resources = arg_filen("r", "resources", "</path/to/resources>", 1, 1, "root resources directory"),
            end = arg_end(1),
    };

    int exit_code = 0;
    char program_name[] = "ycimap";

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

    if (input->count == 1 && resources->count == 1) {
        const char *rootname = resources->filename[0];
        yc_res_map_parse_db_api_t db_api = {
                .context = (void *) rootname,
                .item_type_from_pid = &ycimap_fetch_items_type,
                .scenery_type_from_pid = &ycimap_fetch_scenery_type,
        };

        const char *filename = input->filename[0];
        yc_res_io_fs_api_t io_api = {
                .fopen = (yc_res_io_fopen_t *) &fopen,
                .fclose = (yc_res_io_fclose_t *) &fclose,
                .fseek = (yc_res_io_fseek_t *) &fseek,
                .fread = (yc_res_io_fread_t *) &fread,
        };

        yc_res_map_parse_result_t result = {NULL};
        yc_res_map_status_t status = yc_res_map_parse(filename, &io_api, &db_api, &result);

        if (YC_RES_MAP_STATUS_OK != status) {
            exit_code = 2;
            goto exit;
        }

        printf("Map index: %d\n", result.map->map_idx);
        if (result.map->is_save) { printf("This is save file.\n"); }

        if (yc_res_pro_is_valid_id(result.map->script_idx)) {
            printf("\n");
            printf("Script type: %d\n", yc_res_pro_script_type_from_sid(result.map->script_idx));
            printf("Script index: %d\n", yc_res_pro_index_from_script_id(result.map->script_idx));
        }

        printf("\n");
        printf("Ticks passed: %d\n", result.map->ticks);

        if (yc_res_math_is_valid_location(result.map->entrance)) {
            printf("\n");
            printf("Entrance details:\n");
            printf("Elevation: %d\n", result.map->entrance.elevation_idx);
            printf("Tile index: %d\n", result.map->entrance.grid_idx);
            printf("Orientation: %d\n", result.map->entrance.orientation_idx);
        }

        printf("\n");
        printf("Local vars: %lu\n", result.map->local.count);
        printf("Global vars: %lu\n", result.map->global.count);

        printf("\n");
        for (size_t script_idx = 0; script_idx < result.map->scripts.count; ++script_idx) {
            yc_res_map_script_t *script = &result.map->scripts.pointers[script_idx];
            yc_res_pro_script_type_t type = yc_res_pro_script_type_from_sid(script->script_id);

            printf("Script ID: 0x%X", script->script_id);

            if (YC_RES_PRO_SCRIPT_TYPE_SPATIAL == type) {
                printf(", elevation == %d", script->data.spatial->elevation);
                printf(", distance == %d", script->data.spatial->distance);
            }

            if (YC_RES_PRO_SCRIPT_TYPE_TIMED == type) {
                printf(", duration == %d", script->data.timed->duration);
            }

            printf(", object ID: 0x%X", script->object_id);

            if (script->variables_count > 0) {
                printf(", vars count: %lu", script->variables_count);
            }

            printf("\n");
        }

        for (size_t elevation_idx = 0; elevation_idx < YC_RES_MAP_ELEVATION_COUNT; ++elevation_idx) {
            yc_res_map_level_t *level = result.map->levels[elevation_idx];

            if (NULL != level) {
                printf("\n");
                printf("Level #%lu presented.\n", elevation_idx + 1);
                printf("%lu objects on this level.\n", level->objects.count);

                printf("\n");
                for (size_t object_idx = 0; object_idx < level->objects.count; ++object_idx) {
                    yc_res_map_level_object_t *object = &level->objects.pointers[object_idx];

                    printf(
                            "Object IDx: 0x%X, type: %u, inventory: %lu/%lu\n",
                            yc_res_pro_index_from_object_id(object->proto_id),
                            yc_res_pro_object_type_from_pid(object->proto_id),
                            object->occupied,
                            object->capacity
                    );
                }
            }
        }

        yc_res_map_invalidate(result.map);
    }

    exit:

    arg_freetable(arg_table, sizeof(arg_table) / sizeof(arg_table[0]));
    if (0 != exit_code) { printf("Error occurred, code: %d\n", exit_code); }

    return exit_code;
}

char *proto_filename(uint32_t pid, const char *root, const char *type) {
    const char path[] = "/PROTO";
    const char ext[] = ".LST";

    size_t lstname_size =
            strlen(root) + strlen(path) + 1 + strlen(type) + 1 + strlen(type) + strlen(ext) + 1;

    char *protoname = NULL;
    char *lstname = malloc(lstname_size);
    if (NULL == lstname) { return NULL; }
    snprintf(lstname, lstname_size, "%s%s/%s/%s%s", root, path, type, type, ext);

    yc_res_io_fs_api_t io_api = {
            .fopen = (yc_res_io_fopen_t *) &fopen,
            .fclose = (yc_res_io_fclose_t *) &fclose,
            .fseek = (yc_res_io_fseek_t *) &fseek,
            .fread = (yc_res_io_fread_t *) &fread,
    };

    yc_res_lst_parse_result_t result = {.entries =  NULL};
    yc_res_lst_status_t status = yc_res_lst_parse(lstname, &io_api, &result);

    size_t index = yc_res_pro_index_from_object_id(pid) - 1;
    if (YC_RES_LST_STATUS_OK != status || result.entries->count <= index) { goto cleanup; }

    yc_res_lst_entry_t *entry = &result.entries->pointers[index];
    if (NULL == entry->value) { goto cleanup; }

    size_t protoname_size = strlen(root) + strlen(path) + 1 + strlen(type) + 1 + strlen(entry->value) + 1;
    protoname = malloc(protoname_size);
    if (NULL == protoname) { goto cleanup; }

    snprintf(protoname, protoname_size, "%s%s/%s/%s", root, path, type, entry->value);

    cleanup:
    for (size_t entry_idx = 0; entry_idx < result.entries->count; ++entry_idx) {
        yc_res_lst_invalidate(&result.entries->pointers[entry_idx]);
    }

    free(result.entries->pointers);
    result.entries->pointers = NULL;
    result.entries->count = 0;

    free(result.entries);
    result.entries = NULL;

    return protoname;
}

uint32_t type_byte_from_proto(uint32_t pid, char *root, char *type) {
    char *protoname = proto_filename(pid, root, type);
    FILE *file = fopen(protoname, "rb");

    if (NULL == file) { goto error; }
    if (0 != fseek(file, 0x20, SEEK_CUR)) { goto error; }

    uint32_t result = 0xFFFFFFFF;
    if (0 == fread(&result, sizeof(uint32_t), 1, file)) { goto error; }
    free(protoname);
    fclose(file);

    // from BE
    return ((result >> 24) & 0xff) |
           ((result << 8) & 0xff0000) |
           ((result >> 8) & 0xff00) |
           ((result << 24) & 0xff000000);

    error:
    fclose(file);
    free(protoname);
    return 0xFFFFFFFF;
}

yc_res_pro_object_item_type_t ycimap_fetch_items_type(uint32_t pid, void *context) {
    return type_byte_from_proto(pid, context, "ITEMS");
}

yc_res_pro_object_scenery_type_t ycimap_fetch_scenery_type(uint32_t pid, void *context) {
    return type_byte_from_proto(pid, context, "SCENERY");
}
