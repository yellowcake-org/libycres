#include <ycifrm.h>
#include <stdlib.h>

arg_lit_t *help;
arg_file_t *input;
arg_end_t *end;

void ycifrm_parse_cb(yc_res_frm_sprite_t *sprite);

int main(int argc, char *argv[]) {
    void *arg_table[] = {
            help = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
            input = arg_filen("i", "input", "<sprite.frm>", 1, 1, "input sprite file"),
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

        if (YC_RES_FRM_STATUS_OK != yc_res_frm_sprite_parse(filename, &io_api, &ycifrm_parse_cb)) {
            exit_code = 2;
            goto exit;
        }
    }

    exit:
    arg_freetable(arg_table, sizeof(arg_table) / sizeof(arg_table[0]));

    return exit_code;
}

void ycifrm_parse_cb(yc_res_frm_sprite_t *sprite) {
    printf("FPS: %u\n", sprite->fps);
    printf("Animations: %zu\n", sprite->count);

    for (size_t animation_idx = 0; animation_idx < sprite->count; ++animation_idx) {
        yc_res_frm_animation_t *animation = &sprite->animations[animation_idx];

        printf("\n");
        printf("Animation #%zu\n", animation_idx);
        printf("Shift x: %u\nShift y: %u\n", animation->shift.horizontal, animation->shift.vertical);
        printf("\n");

        for (size_t frame_idx = 0; frame_idx < animation->count; ++frame_idx) {
            yc_res_frm_texture_t *frame = &animation->frames[frame_idx];

            printf("Frame #%zu\n", frame_idx);
            printf(
                    "Width: %u + %u shift\nHeight: %u + %u shift\n",
                    frame->dimensions.horizontal, frame->shift.horizontal,
                    frame->dimensions.vertical, frame->shift.vertical
            );
        }
    }

    yc_res_frm_sprite_invalidate(sprite);
    free(sprite);
}
