#include <ycipro.h>

#include <stdlib.h>
#include <memory.h>

arg_lit_t *help;
arg_file_t *input;
arg_end_t *end;

void ycipro_print_cb(yc_res_pro_object_t *object);

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

        if (YC_RES_FRM_STATUS_OK != yc_res_pro_object_parse(filename, &io_api, &ycipro_print_cb)) {
            exit_code = 3;
            goto exit;
        }
    }

    exit:
    arg_freetable(arg_table, sizeof(arg_table) / sizeof(arg_table[0]));

    return exit_code;
}

void ycipro_print_cb(yc_res_pro_object_t *object) {
    printf("Proto .LST: %u\n", yc_res_pro_index_from_id(object->proto_id));
    printf("Text .LST: %u\n", yc_res_pro_index_from_id(object->text_id));
    printf("Sprite .LST: %u\n", yc_res_pro_index_from_id(object->sprite_id));
    printf("\n");
    printf("Light level: %u\n", object->lighting.level);
    printf("Light radius: %u\n", object->lighting.radius);

    printf("\n");
    printf("Flags: {\n");

    if (true == object->flags.no_block) { printf("no_block\n"); }
    if (true == object->flags.no_border) { printf("no_border\n"); }

    if (true == object->flags.is_flat) { printf("is_flat\n"); }
    if (true == object->flags.multi_hex) { printf("multi_hex\n"); }
    if (true == object->flags.light_through) { printf("light_through\n"); }
    if (true == object->flags.shoot_through) { printf("shoot_through\n"); }

    printf("}\n");
    printf("\n");

    printf("Transparency: ");
    switch (object->flags.transparency) {
        case YC_RES_PRO_TRANS_NONE:
            printf("none.");
            break;
        case YC_RES_PRO_TRANS_RED:
            printf("RED.");
            break;
        case YC_RES_PRO_TRANS_WALL:
            printf("WALL.");
            break;
        case YC_RES_PRO_TRANS_GLASS:
            printf("GLASS.");
            break;
        case YC_RES_PRO_TRANS_STEAM:
            printf("STEAM.");
            break;
        case YC_RES_PRO_TRANS_ENERGY:
            printf("ENERGY.");
            break;
        case YC_RES_PRO_TRANS_WALL_END:
            printf("WALL END.");
            break;
        default:
            printf("UNKNOWN.");
            break;
    }
    printf("\n");

    printf("\n");
    printf("Prototype is ");
    switch (yc_res_pro_type_from_pid(object->proto_id)) {
        case YC_RES_PRO_OBJECT_TYPE_ITEM: {
            printf("ITEM");
            printf("\n\n");
            printf("Cost: %d\n", object->data.item->cost);
            printf("Weight: %d\n", object->data.item->weight);
            printf("Volume: %d\n", object->data.item->volume);

            printf("\n");
            printf("Item is ");
            switch (object->data.item->type) {
                case YC_RES_PRO_OBJECT_ITEM_ARMOR:
                    printf("ARMOR");
                    printf("\n\n");
                    printf("Class: %d\n", object->data.item->data.armor->class);
                    printf("Perk: %d\n", object->data.item->data.armor->perk);

                    printf(
                            "Male sprite index: %d\n",
                            yc_res_pro_index_from_id(
                                    object->data.item->data.armor->sprite_ids[YC_RES_PRO_TYPES_GENDER_MALE]
                            )
                    );
                    printf(
                            "Female sprite index: %d\n",
                            yc_res_pro_index_from_id(
                                    object->data.item->data.armor->sprite_ids[YC_RES_PRO_TYPES_GENDER_FEMALE]
                            )
                    );

                    printf("\n");
                    for (yc_res_pro_damage_type_t dmg_idx = 0; dmg_idx < YC_RES_PRO_DAMAGE_TYPE_COUNT; ++dmg_idx) {
                        switch (dmg_idx) {
                            case YC_RES_PRO_DAMAGE_TYPE_NORMAL:
                                printf("DR Normal: %d\n", object->data.item->data.armor->resistances[dmg_idx]);
                                printf("DT Normal: %d\n", object->data.item->data.armor->thresholds[dmg_idx]);
                                printf("\n");
                                break;
                            case YC_RES_PRO_DAMAGE_TYPE_LASER:
                                printf("DR Laser: %d\n", object->data.item->data.armor->resistances[dmg_idx]);
                                printf("DT Laser: %d\n", object->data.item->data.armor->thresholds[dmg_idx]);
                                printf("\n");
                                break;
                            case YC_RES_PRO_DAMAGE_TYPE_FIRE:
                                printf("DR Fire: %d\n", object->data.item->data.armor->resistances[dmg_idx]);
                                printf("DT Fire: %d\n", object->data.item->data.armor->thresholds[dmg_idx]);
                                printf("\n");
                                break;
                            case YC_RES_PRO_DAMAGE_TYPE_PLASMA:
                                printf("DR Plasma: %d\n", object->data.item->data.armor->resistances[dmg_idx]);
                                printf("DT Plasma: %d\n", object->data.item->data.armor->thresholds[dmg_idx]);
                                printf("\n");
                                break;
                            case YC_RES_PRO_DAMAGE_TYPE_ELECTRICAL:
                                printf("DR Electrical: %d\n", object->data.item->data.armor->resistances[dmg_idx]);
                                printf("DT Electrical: %d\n", object->data.item->data.armor->thresholds[dmg_idx]);
                                printf("\n");
                                break;
                            case YC_RES_PRO_DAMAGE_TYPE_EMP:
                                printf("DR EMP: %d\n", object->data.item->data.armor->resistances[dmg_idx]);
                                printf("DT EMP: %d\n", object->data.item->data.armor->thresholds[dmg_idx]);
                                printf("\n");
                                break;
                            case YC_RES_PRO_DAMAGE_TYPE_EXPLOSION:
                                printf("DR Explosion: %d\n", object->data.item->data.armor->resistances[dmg_idx]);
                                printf("DT Explosion: %d\n", object->data.item->data.armor->thresholds[dmg_idx]);
                                printf("\n");
                                break;
                            default:
                                break;
                        }
                    }
                    break;
                case YC_RES_PRO_OBJECT_ITEM_CONTAINER:
                    printf("CONTAINER");
                    break;
                case YC_RES_PRO_OBJECT_ITEM_DRUG:
                    printf("DRUG");
                    break;
                case YC_RES_PRO_OBJECT_ITEM_AMMO:
                    printf("AMMO");
                    break;
                case YC_RES_PRO_OBJECT_ITEM_MISC:
                    printf("MISC");
                    break;
                case YC_RES_PRO_OBJECT_ITEM_KEY:
                    printf("KEY");
                    break;
            }
        }
            break;
        case YC_RES_PRO_OBJECT_TYPE_CRITTER:
            printf("CRITTER");
            break;
        case YC_RES_PRO_OBJECT_TYPE_SCENERY:
            printf("SCENERY");
            break;
        case YC_RES_PRO_OBJECT_TYPE_WALL:
            printf("WALL");
            break;
        case YC_RES_PRO_OBJECT_TYPE_TILE:
            printf("TILE");
            break;
        case YC_RES_PRO_OBJECT_TYPE_MISC:
            printf("MISC");
            break;
        case YC_RES_PRO_OBJECT_TYPE_INTERFACE:
            printf("INTERFACE");
            break;
        case YC_RES_PRO_OBJECT_TYPE_INVENTORY:
            printf("INVENTORY");
            break;
        case YC_RES_PRO_OBJECT_TYPE_HEAD:
            printf("HEAD");
            break;
        case YC_RES_PRO_OBJECT_TYPE_BACKGROUND:
            printf("BACKGROUND");
            break;
        default:
            printf("UNKNOWN");
            break;
    }
    printf("\n");

    yc_res_pro_object_invalidate(object);
    free(object);
}
