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
    printf("Proto .LST: %u\n", yc_res_pro_index_from_object_id(object->proto_id));
    printf("Text .LST: %u\n", yc_res_pro_index_from_object_id(object->text_id));
    printf("Sprite .LST: %u\n", yc_res_pro_index_from_object_id(object->sprite_id));
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
    switch (yc_res_pro_object_type_from_pid(object->proto_id)) {
        case YC_RES_PRO_OBJECT_TYPE_ITEM: {
            printf("ITEM");
            printf("\n\n");
            printf("Cost: %d\n", object->data.item->cost);
            printf("Weight: %d\n", object->data.item->weight);
            printf("Volume: %d\n", object->data.item->volume);

            if (yc_res_pro_is_valid_object_id(object->data.item->sprite_id)) {
                printf("Sprite index: %d\n", yc_res_pro_index_from_object_id(object->data.item->sprite_id));
            }

            printf("\n");
            printf("Item is ");
            switch (object->data.item->type) {
                case YC_RES_PRO_OBJECT_ITEM_TYPE_ARMOR:
                    printf("ARMOR");
                    printf("\n\n");
                    printf("Class: %d\n", object->data.item->data.armor->class);
                    printf("Perk: %d\n", object->data.item->data.armor->perk);

                    printf(
                            "[♂︎] Sprite index: %d\n",
                            yc_res_pro_index_from_object_id(
                                    object->data.item->data.armor->sprite_ids[YC_RES_PRO_TYPES_GENDER_MALE]
                            )
                    );
                    printf(
                            "[♀︎] Sprite index: %d\n",
                            yc_res_pro_index_from_object_id(
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
                case YC_RES_PRO_OBJECT_ITEM_TYPE_CONTAINER:
                    printf("CONTAINER");
                    printf("\n\n");
                    printf("Capacity: %d\n", object->data.item->data.container->capacity);
                    printf("Flags: {\n");

                    if (true == object->data.item->data.container->flags.no_pickup) { printf("no_pickup\n"); }
                    if (true == object->data.item->data.container->flags.is_grounded) { printf("is_grounded\n"); }

                    printf("}\n");
                    printf("\n");
                    break;
                case YC_RES_PRO_OBJECT_ITEM_TYPE_DRUG:
                    printf("DRUG");
                    printf("\n\n");

                    yc_res_pro_object_item_drug_t *drug = object->data.item->data.drug;

                    printf("Addiction chance: %d\n", drug->addiction_chance);
                    printf("Perk: %d\n", drug->withdrawal_perk);
                    printf("Delay: %d\n", drug->withdrawal_delay);

                    printf("\n");
                    for (size_t effect_idx = 0; effect_idx < drug->count; ++effect_idx) {
                        yc_res_pro_object_item_drug_effect_t effect = drug->effects[effect_idx];

                        printf("Effect: %lu\n\n", effect_idx);
                        printf("Stat: %d\n", effect.stat);
                        for (size_t mod_idx = 0; mod_idx < 3; ++mod_idx) {
                            printf("Modifier %lu:\n", mod_idx);
                            yc_res_pro_object_item_drug_stat_modifier_t mod = effect.modifiers[mod_idx];

                            printf("Delay: %d\n", mod.delay);
                            printf("Impact: %d", mod.impact.to);

                            if (mod.impact.from > 0) { printf(" to %d\n", mod.impact.to); }
                            else { printf("\n"); }
                        }
                        printf("\n");
                    }
                    break;
                case YC_RES_PRO_OBJECT_ITEM_TYPE_WEAPON:
                    printf("WEAPON");
                    printf("\n\n");

                    yc_res_pro_object_item_weapon_t *weapon = object->data.item->data.weapon;

                    printf("Min ST: %d\n", weapon->requirement.value);
                    printf("Capacity: %d\n", weapon->capacity);
                    printf("Burst: %d\n", weapon->burst);
                    printf("\n");
                    printf("Ammo index: %d\n", weapon->ammo_item_idx);
                    printf("Crit fail index: %d\n", weapon->critical_failure_idx);
                    printf("Projectile PID: 0x%X\n", weapon->projectile_pid);
                    break;
                case YC_RES_PRO_OBJECT_ITEM_TYPE_AMMO:
                    printf("AMMO");
                    printf("\n\n");

                    yc_res_pro_object_item_ammo_t *ammo = object->data.item->data.ammo;

                    printf("Count: %d\n", ammo->count);
                    printf("Caliber: %d\n", ammo->caliber);
                    printf("\n");
                    printf("Armor Class: %d\n", ammo->armor_class);
                    printf("Damage Resistance: %d\n", ammo->damage_resistance);
                    printf("\n");
                    printf("Damage mul: %d\n", ammo->damage_multiplier);
                    printf("Damage div: %d\n", ammo->damage_divider);
                    break;
                case YC_RES_PRO_OBJECT_ITEM_TYPE_MISC:
                    printf("MISC");
                    printf("\n\n");

                    yc_res_pro_object_item_misc_t *misc = object->data.item->data.misc;

                    printf("Item index: %d\n", misc->item_idx);
                    printf("Count: %d\n", misc->count);
                    printf("Caliber: %d\n", misc->caliber);
                    break;
                case YC_RES_PRO_OBJECT_ITEM_TYPE_KEY:
                    printf("KEY");
                    printf("\n\n");

                    printf("Code: 0x%X\n", object->data.item->data.key->code);
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
