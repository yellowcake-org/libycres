#include <ycipro.h>
#include <stdlib.h>

static arg_lit_t *help;
static arg_file_t *input;
static arg_end_t *end;

void ycipro_print_cb(yc_res_pro_object_t *object);

void *ycipro_io_fopen(const char *filename, const char *mode);
int ycipro_io_fclose(void *stream);

int ycipro_io_fseek(void *stream, long offset, int whence);
size_t ycipro_io_fread(void *dest, size_t len, size_t cnt, void *str);

int main(int argc, char *argv[]) {
    void *arg_table[] = {
            help = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
            input = arg_filen("i", "input", "<object.pro>", 1, 1, "input game object file"),
            end = arg_end(1),
    };

    int exit_code = 0;
    char program_name[] = "ycipro";

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
                .fopen = &ycipro_io_fopen,
                .fclose = &ycipro_io_fclose,
                .fseek = &ycipro_io_fseek,
                .fread = &ycipro_io_fread,
        };

        yc_res_pro_parse_result_t result = {NULL};
        if (YC_RES_PRO_STATUS_OK != yc_res_pro_parse(filename, &io_api, &result)) {
            exit_code = 3;
            goto exit;
        }

        ycipro_print_cb(result.object);
    }

    exit:

    arg_freetable(arg_table, sizeof(arg_table) / sizeof(arg_table[0]));
    if (0 != exit_code) { printf("Error occurred, code: %d\n", exit_code); }

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

            if (yc_res_pro_is_valid_id(object->data.item->sprite_id)) {
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
                    printf("Ammo index: %d\n", weapon->ammo_pid);
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
            printf("\n\n");

            yc_res_pro_object_critter_t *critter = object->data.critter;

            printf("Head FID: 0x%X\n", critter->sprite_id);
            printf("Team index: %d\n", critter->team_idx);

            if (yc_res_pro_is_valid_id(critter->script_id)) {
                printf("\n");
                printf("Script type: %d\n", yc_res_pro_script_type_from_sid(critter->script_id));
                printf("Script index: %d\n", yc_res_pro_index_from_script_id(critter->script_id));
            }

            printf("\n");
            printf("Body type: %d\n", critter->body);
            printf("Kill type: %d\n", critter->kill_type);
            printf("Kill reward: %d\n", critter->kill_reward);
            break;
        case YC_RES_PRO_OBJECT_TYPE_SCENERY:
            printf("SCENERY");
            printf("\n\n");

            yc_res_pro_object_scenery_t *scenery = object->data.scenery;

            printf("Material: %d\n", scenery->material);
            if (yc_res_pro_is_valid_id(scenery->script_id)) {
                printf("Script type: %d\n", yc_res_pro_script_type_from_sid(scenery->script_id));
                printf("Script index: %d\n", yc_res_pro_index_from_script_id(scenery->script_id));
            }

            printf("\n");
            printf("Scenery is ");

            switch (scenery->type) {
                case YC_RES_PRO_OBJECT_SCENERY_TYPE_DOOR:
                    printf("DOOR");
                    printf("\n\n");
                    printf("Passable: %d\n", scenery->data.door->is_passable);
                    break;
                case YC_RES_PRO_OBJECT_SCENERY_TYPE_STAIRS:
                    printf("STAIRS");
                    printf("\n");

                    yc_res_pro_object_scenery_stairs_t *stairs = scenery->data.stairs;

                    if (yc_res_pro_is_valid_id((uint32_t) stairs->map_id)) {
                        printf("\n");
                        printf("Map ID: 0x%X\n", stairs->map_id);
                        printf("Destination tile: %d\n", scenery->data.stairs->destination.grid_idx);
                        printf("Destination elevation: %d\n", scenery->data.stairs->destination.elevation_idx);
                        printf("Destination orientation_idx: %d\n", scenery->data.stairs->destination.orientation_idx);
                    }
                    break;
                case YC_RES_PRO_OBJECT_SCENERY_TYPE_ELEVATOR:
                    break;
                case YC_RES_PRO_OBJECT_SCENERY_TYPE_LADDER_BOTTOM:
                    printf("LADDER BOTTOM");
                    printf("\n");

                    if (yc_res_math_is_valid_location(scenery->data.ladder->destination)) {
                        printf("\n");
                        printf("Destination tile: %d\n", scenery->data.ladder->destination.grid_idx);
                        printf("Destination elevation: %d\n", scenery->data.ladder->destination.elevation_idx);
                        printf("Destination orientation_idx: %d\n", scenery->data.ladder->destination.orientation_idx);
                    }
                    break;
                case YC_RES_PRO_OBJECT_SCENERY_TYPE_LADDER_TOP:
                    printf("LADDER TOP");
                    printf("\n");

                    if (yc_res_math_is_valid_location(scenery->data.ladder->destination)) {
                        printf("\n");
                        printf("Destination tile: %d\n", scenery->data.ladder->destination.grid_idx);
                        printf("Destination elevation: %d\n", scenery->data.ladder->destination.elevation_idx);
                        printf("Destination orientation_idx: %d\n", scenery->data.ladder->destination.orientation_idx);
                    }
                    break;
                case YC_RES_PRO_OBJECT_SCENERY_TYPE_GENERIC:
                    printf("GENERIC");
                    printf("\n\n");
                    printf("Unknown: 0x%X\n", scenery->data.generic->_unknown);
                    break;
            }

            break;
        case YC_RES_PRO_OBJECT_TYPE_WALL:
            printf("WALL");
            printf("\n\n");

            yc_res_pro_object_wall_t *wall = object->data.wall;

            printf("Material: %d\n", wall->material);
            if (yc_res_pro_is_valid_id(wall->script_id)) {
                printf("Script type: %d\n", yc_res_pro_script_type_from_sid(wall->script_id));
                printf("Script index: %d\n", yc_res_pro_index_from_script_id(wall->script_id));
            }
            break;
        case YC_RES_PRO_OBJECT_TYPE_TILE:
            printf("TILE");
            printf("\n\n");

            printf("Material: %d\n", object->data.tile->material);
            break;
        case YC_RES_PRO_OBJECT_TYPE_MISC:
            printf("MISC");
            printf("\n\n");
            printf("Unknown: 0x%X\n", object->data.misc->_unknown);
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

    yc_res_pro_object_invalidate(object);
    free(object);
}

void *ycipro_io_fopen(const char *filename, const char *mode) { return fopen(filename, mode); }
int ycipro_io_fclose(void *stream) { return fclose(stream); }

int ycipro_io_fseek(void *stream, long offset, int whence) { return fseek(stream, offset, whence); }
size_t ycipro_io_fread(void *dest, size_t len, size_t cnt, void *str) {
    return fread(dest, len, cnt, str);
}
