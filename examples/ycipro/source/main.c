#include <ycipro.h>

#include <stdlib.h>
#include <memory.h>

arg_lit_t *help;
arg_file_t *input;
arg_end_t *end;

void ycipro_print_cb(yc_res_pro_object_header_t *header);

int main(int argc, char *argv[]) {
    void *arg_table[] = {
            help = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
            input = arg_filen("i", "input", "<object.pro>", 1, 1, "input sprite file"),
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

        if (YC_RES_FRM_STATUS_OK != yc_res_pro_header_parse(filename, &io_api, &ycipro_print_cb)) {
            exit_code = 3;
            goto exit;
        }
    }

    exit:
    arg_freetable(arg_table, sizeof(arg_table) / sizeof(arg_table[0]));

    return exit_code;
}

void ycipro_print_cb(yc_res_pro_object_header_t *header) {
    printf("Prototype is ");


    switch (yc_res_entity_from_pid(header->proto_id)) {
        case YC_RES_PRO_ENTITY_ITEM:
            printf("ITEM");
            break;
        case YC_RES_PRO_ENTITY_CRITTER:
            printf("CRITTER");
            break;
        case YC_RES_PRO_ENTITY_SCENERY:
            printf("SCENERY");
            break;
        case YC_RES_PRO_ENTITY_WALL:
            printf("WALL");
            break;
        case YC_RES_PRO_ENTITY_TILE:
            printf("TILE");
            break;
        case YC_RES_PRO_ENTITY_MISC:
            printf("MISC");
            break;
        case YC_RES_PRO_ENTITY_INTERFACE:
            printf("INTERFACE");
            break;
        case YC_RES_PRO_ENTITY_INVENTORY:
            printf("INVENTORY");
            break;
        case YC_RES_PRO_ENTITY_HEAD:
            printf("HEAD");
            break;
        case YC_RES_PRO_ENTITY_BACKGROUND:
            printf("BACKGROUND");
            break;
        default:
            printf("UNKNOWN");
            break;
    }
    printf("\n");

    printf("\n");
    printf("Proto .LST: %u\n", yc_res_index_from_any_id(header->proto_id));
    printf("Text .LST: %u\n", yc_res_index_from_any_id(header->text_id));
    printf("Sprite .LST: %u\n", yc_res_index_from_any_id(header->sprite_id));
    printf("\n");
    printf("Light level: %u\n", header->lighting.level);
    printf("Light radius: %u\n", header->lighting.radius);

    printf("\n");
    printf("Flags: {\n");

    if (true == header->flags.multi_hex) { printf("multi_hex\n"); }
    if (true == header->flags.light_through) { printf("light_through\n"); }
    if (true == header->flags.shoot_through) { printf("shoot_through\n"); }

    if (true == header->flags.is_flat) { printf("is_flat\n"); }
    if (true == header->flags.is_hidden) { printf("is_hidden\n"); }
    if (true == header->flags.is_locked) { printf("is_locked\n"); }
    if (true == header->flags.is_jammed) { printf("is_jammed\n"); }

    printf("}\n");

    free(header);
}
