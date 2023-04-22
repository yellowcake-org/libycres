#include <libycres.h>
#include <private.h>

#include <stdlib.h>

void yc_res_frm_parse_cleanup(void *file, const yc_res_io_fs_api_t *io, yc_res_frm_sprite_t *sprite);

yc_res_frm_status_t yc_res_frm_sprite_parse(
        const char *filename,
        yc_res_io_fs_api_t *io,
        yc_res_frm_sprite_cb_t *callback
) {
    void *file = io->fopen(filename, "rb");

    if (NULL == file) {
        yc_res_frm_parse_cleanup(NULL, io, NULL);
        return YC_RES_FRM_STATUS_IO;
    }

    yc_res_frm_sprite_t *sprite = malloc(sizeof(yc_res_frm_sprite_t));

    if (NULL == sprite) {
        yc_res_frm_parse_cleanup(file, io, sprite);
        return YC_RES_FRM_STATUS_IO;
    }

    if (0 != io->fseek(file, sizeof(uint32_t), SEEK_CUR)) {
        yc_res_frm_parse_cleanup(file, io, sprite);
        return YC_RES_FRM_STATUS_IO;
    }

    uint16_t fps = 0;
    if (0 == io->fread(&fps, sizeof(uint16_t), 1, file)) {
        yc_res_frm_parse_cleanup(file, io, sprite);
        return YC_RES_FRM_STATUS_IO;
    }

    fps = yc_res_byteorder_uint16(fps);

    uint16_t keyframe_idx = 0;
    if (0 == io->fread(&keyframe_idx, sizeof(uint16_t), 1, file)) {
        yc_res_frm_parse_cleanup(file, io, sprite);
        return YC_RES_FRM_STATUS_IO;
    }

    keyframe_idx = yc_res_byteorder_uint16(keyframe_idx);

    uint16_t fpo = 0;
    if (0 == io->fread(&fpo, sizeof(uint16_t), 1, file)) {
        yc_res_frm_parse_cleanup(file, io, sprite);
        return YC_RES_FRM_STATUS_IO;
    }

    fpo = yc_res_byteorder_uint16(fpo);

    // warning: storing temporary in big endian, still
    int16_t animations_shifts_h[6], animations_shifts_v[6];
    uint32_t offsets[6];

    if (0 == io->fread(&animations_shifts_h, sizeof(int16_t) * 6, 1, file)) {
        yc_res_frm_parse_cleanup(file, io, sprite);
        return YC_RES_FRM_STATUS_IO;
    }

    if (0 == io->fread(&animations_shifts_v, sizeof(int16_t) * 6, 1, file)) {
        yc_res_frm_parse_cleanup(file, io, sprite);
        return YC_RES_FRM_STATUS_IO;
    }

    if (0 == io->fread(&offsets, sizeof(uint32_t) * 6, 1, file)) {
        yc_res_frm_parse_cleanup(file, io, sprite);
        return YC_RES_FRM_STATUS_IO;
    }

    if (0 != io->fseek(file, sizeof(uint32_t), SEEK_CUR)) {
        yc_res_frm_parse_cleanup(file, io, sprite);
        return YC_RES_FRM_STATUS_IO;
    }

    sprite->count = 0;
    sprite->animations = NULL;

    char _table[6] = {-1, -1, -1, -1, -1, -1};

    for (size_t orientation = 0; orientation < 6; ++orientation) {
        const uint32_t offset = yc_res_byteorder_uint32(offsets[orientation]);

        if (_table[orientation] >= 0) {
            sprite->orientations[orientation] = (unsigned char) _table[orientation];
            continue;
        }

        sprite->count++;
        size_t size = sizeof(yc_res_frm_animation_t) * sprite->count;
        sprite->animations = NULL == sprite->animations ? malloc(size) : realloc(sprite->animations, size);

        if (NULL == sprite->animations) {
            yc_res_frm_parse_cleanup(file, io, sprite);
            return YC_RES_FRM_STATUS_MEM;
        }

        size_t idx = sprite->count - 1;
        sprite->orientations[orientation] = idx;

        for (size_t o = orientation; o < 6; ++o) {
            if (offset == yc_res_byteorder_uint32(offsets[o])) { _table[o] = (char) idx; }
        }

        yc_res_frm_animation_t *animation = &sprite->animations[idx];

        animation->fps = fps;
        animation->keyframe_idx = keyframe_idx;

        animation->count = fpo;
        animation->frames = malloc(sizeof(yc_res_frm_texture_t) * animation->count);

        if (NULL == animation->frames) {
            yc_res_frm_parse_cleanup(file, io, sprite);
            return YC_RES_FRM_STATUS_MEM;
        }

        animation->shift.vertical = yc_res_byteorder_int16(animations_shifts_v[orientation]);
        animation->shift.horizontal = yc_res_byteorder_int16(animations_shifts_h[orientation]);

        for (uint16_t frame_idx = 0; frame_idx < fpo; ++frame_idx) {
            yc_res_frm_texture_t *frame = &animation->frames[frame_idx];

            uint16_t width, height;

            if (0 == io->fread(&width, sizeof(uint16_t), 1, file)) {
                yc_res_frm_parse_cleanup(file, io, sprite);
                return YC_RES_FRM_STATUS_IO;
            }

            if (0 == io->fread(&height, sizeof(uint16_t), 1, file)) {
                yc_res_frm_parse_cleanup(file, io, sprite);
                return YC_RES_FRM_STATUS_IO;
            }

            frame->dimensions.vertical = yc_res_byteorder_uint16(height);
            frame->dimensions.horizontal = yc_res_byteorder_uint16(width);

            uint32_t square;
            if (0 == io->fread(&square, sizeof(uint32_t), 1, file)) {
                yc_res_frm_parse_cleanup(file, io, sprite);
                return YC_RES_FRM_STATUS_IO;
            }
            square = yc_res_byteorder_uint32(square);

            if (square != (size_t) frame->dimensions.horizontal * (size_t) frame->dimensions.vertical) {
                yc_res_frm_parse_cleanup(file, io, sprite);
                return YC_RES_FRM_STATUS_CORR;
            }

            int16_t x, y;

            if (0 == io->fread(&x, sizeof(int16_t), 1, file)) {
                yc_res_frm_parse_cleanup(file, io, sprite);
                return YC_RES_FRM_STATUS_IO;
            }

            if (0 == io->fread(&y, sizeof(int16_t), 1, file)) {
                yc_res_frm_parse_cleanup(file, io, sprite);
                return YC_RES_FRM_STATUS_IO;
            }

            frame->shift.vertical = yc_res_byteorder_int16(y);
            frame->shift.horizontal = yc_res_byteorder_int16(x);

            frame->pixels = malloc(square);

            if (NULL == frame->pixels) {
                yc_res_frm_parse_cleanup(file, io, sprite);
                return YC_RES_FRM_STATUS_MEM;
            }

            if (0 == io->fread(frame->pixels, square, 1, file)) {
                yc_res_frm_parse_cleanup(file, io, sprite);
                return YC_RES_FRM_STATUS_IO;
            }
        }
    }

    yc_res_frm_parse_cleanup(file, io, NULL);

    callback(sprite);
    return YC_RES_FRM_STATUS_OK;
}

void yc_res_frm_parse_cleanup(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_frm_sprite_t *sprite
) {
    if (NULL != file) {
        io->fclose(file);
    }

    if (NULL != sprite) {
        yc_res_frm_sprite_invalidate(sprite);
        free(sprite);
    }
}
