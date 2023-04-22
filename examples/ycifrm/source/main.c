#include <ycifrm.h>
#include <stdlib.h>
#include <memory.h>

arg_lit_t *help, *merge;
arg_file_t *input;
arg_end_t *end;

size_t count = 0;
yc_res_frm_sprite_t **split = NULL;

void ycifrm_print_cb(yc_res_frm_sprite_t *sprite);

void ycifrm_append_cb(yc_res_frm_sprite_t *sprite);

int main(int argc, char *argv[]) {
    void *arg_table[] = {
            help = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
            input = arg_filen("i", "input", "<sprite.fr(0-5)|(m)>", 1, 1, "input sprite file"),
            merge = arg_litn(NULL, "merge", 0, 1, "specified sprite is split into .fr(0-5)"),
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

        if (merge->count == 0) {
            if (YC_RES_FRM_STATUS_OK != yc_res_frm_sprite_parse(filename, &io_api, &ycifrm_print_cb)) {
                exit_code = 3;
                goto exit;
            }
        } else {
            split = malloc(sizeof(yc_res_frm_sprite_t *));
            if (NULL == split) {
                exit_code = 2;
                goto exit;
            }

            *split = malloc(sizeof(yc_res_frm_sprite_t) * 6);
            if (NULL == *split) {
                exit_code = 2;
                goto exit;
            }

            size_t base = strlen(filename);
            for (size_t idx = 0; idx < 6; ++idx) {
                char *final = malloc(base + 4);

                if (NULL == final) {
                    exit_code = 3;
                    goto exit;
                }

                strcpy(final, filename);
                sprintf(&final[base], ".FR%lu", idx);

                yc_res_frm_status_t status =
                        yc_res_frm_sprite_parse(final, &io_api, &ycifrm_append_cb);

                free(final);

                if (YC_RES_FRM_STATUS_OK != status) {
                    exit_code = 4;
                    goto exit;
                }
            }

            yc_res_frm_status_t status = yc_res_frm_sprites_merge(split, count);

            if (YC_RES_FRM_STATUS_OK != status) {
                exit_code = 4;
                goto exit;
            }

            ycifrm_print_cb(*split);
            yc_res_frm_sprite_invalidate(*split);

            free(split);
            split = NULL;
        }
    }

    exit:
    arg_freetable(arg_table, sizeof(arg_table) / sizeof(arg_table[0]));

    return exit_code;
}

void ycifrm_append_cb(yc_res_frm_sprite_t *sprite) {
    if (NULL != split) {
        yc_res_frm_sprite_t *list = *split;

        count++;
        memmove(&list[count - 1], sprite, sizeof(yc_res_frm_sprite_t));
    }
}

void ycifrm_print_cb(yc_res_frm_sprite_t *sprite) {
    printf("Animations: %zu\n", sprite->count);

    for (size_t animation_idx = 0; animation_idx < sprite->count; ++animation_idx) {
        yc_res_frm_animation_t *animation = &sprite->animations[animation_idx];

        printf("\n");
        printf("Animation #%zu\n", animation_idx);
        printf("FPS: %u\n", animation->fps);
        printf("Frames: %zu\n", animation->count);
        printf("Shift x: %d\nShift y: %d\n", animation->shift.horizontal, animation->shift.vertical);
        printf("\n");

        for (size_t frame_idx = 0; frame_idx < animation->count; ++frame_idx) {
            yc_res_frm_texture_t *frame = &animation->frames[frame_idx];

            printf("Frame #%zu\n", frame_idx);
            printf(
                    "Width: %u + %d shift\nHeight: %u + %d shift\n",
                    frame->dimensions.horizontal, frame->shift.horizontal,
                    frame->dimensions.vertical, frame->shift.vertical
            );
        }
    }

    yc_res_frm_sprite_invalidate(sprite);
    free(sprite);
}
