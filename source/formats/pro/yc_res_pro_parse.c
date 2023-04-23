#include <libycres.h>
#include <private.h>

#include <stdlib.h>

void yc_res_pro_parse_flags(uint32_t flags, yc_res_pro_object_header_t *into);

void yc_res_pro_parse_cleanup(void *file, const yc_res_io_fs_api_t *io, yc_res_pro_object_header_t *header);

yc_res_pro_status_t yc_res_pro_header_parse(
        const char *filename,
        const yc_res_io_fs_api_t *io,
        yc_res_frm_object_header_cb_t *callback
) {
    void *file = io->fopen(filename, "rb");

    if (NULL == file) {
        yc_res_pro_parse_cleanup(file, io, NULL);
        return YC_RES_PRO_STATUS_IO;
    }

    yc_res_pro_object_header_t *header = malloc(sizeof(yc_res_pro_object_header_t));

    if (NULL == header) {
        yc_res_pro_parse_cleanup(file, io, header);
        return YC_RES_PRO_STATUS_MEM;
    }

    if (0 == io->fread(&header->proto_id, sizeof(uint32_t), 1, file)) {
        yc_res_pro_parse_cleanup(file, io, header);
        return YC_RES_PRO_STATUS_IO;
    }
    header->proto_id = yc_res_byteorder_uint32(header->proto_id);

    if (0 == io->fread(&header->text_id, sizeof(uint32_t), 1, file)) {
        yc_res_pro_parse_cleanup(file, io, header);
        return YC_RES_PRO_STATUS_IO;
    }
    header->text_id = yc_res_byteorder_uint32(header->text_id);

    if (0 == io->fread(&header->sprite_id, sizeof(uint32_t), 1, file)) {
        yc_res_pro_parse_cleanup(file, io, header);
        return YC_RES_PRO_STATUS_IO;
    }
    header->sprite_id = yc_res_byteorder_uint32(header->sprite_id);

    uint32_t light_radius = 0;
    if (0 == io->fread(&light_radius, sizeof(uint32_t), 1, file)) {
        yc_res_pro_parse_cleanup(file, io, header);
        return YC_RES_PRO_STATUS_IO;
    }

    light_radius = yc_res_byteorder_uint32(light_radius);
    header->lighting.radius = (u_int8_t) light_radius;

    uint32_t light_level = 0;
    if (0 == io->fread(&light_level, sizeof(uint32_t), 1, file)) {
        yc_res_pro_parse_cleanup(file, io, header);
        return YC_RES_PRO_STATUS_IO;
    }

    light_level = yc_res_byteorder_uint32(light_level);
    header->lighting.level = (u_int16_t) light_level;

    uint32_t flags = 0;
    if (0 == io->fread(&flags, sizeof(uint32_t), 1, file)) {
        yc_res_pro_parse_cleanup(file, io, header);
        return YC_RES_PRO_STATUS_IO;
    }

    flags = yc_res_byteorder_uint32(flags);
    yc_res_pro_parse_flags(flags, header);

    yc_res_pro_parse_cleanup(file, io, NULL);

    callback(header);
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_parse_flags(uint32_t flags, yc_res_pro_object_header_t *into) {
    into->flags.multi_hex = 0x00000800 == (flags & 0x00000800);

    into->flags.is_flat = 0x00000008 == (flags & 0x00000008);
    into->flags.is_hidden = 0x00000001 == (flags & 0x00000001);
    into->flags.is_jammed = 0x04000000 == (flags & 0x04000000);
    into->flags.is_locked = 0x02000000 == (flags & 0x02000000);

    into->flags.no_block = 0x00000010 == (flags & 0x00000010);
    into->flags.no_border = 0x00000020 == (flags & 0x00000020);

    into->flags.light_through = 0x20000000 == (flags & 0x20000000);
    into->flags.shoot_through = 0x80000000 == (flags & 0x80000000);

    into->flags.transparency = YC_RES_PRO_TRANS_NONE;

    if (0x4000 == (flags & 0x4000)) { into->flags.transparency = YC_RES_PRO_TRANS_RED; }
    if (0x8000 == (flags & 0x8000)) { into->flags.transparency = YC_RES_PRO_TRANS_NONE; }
    if (0x10000 == (flags & 0x10000)) { into->flags.transparency = YC_RES_PRO_TRANS_WALL; }
    if (0x20000 == (flags & 0x20000)) { into->flags.transparency = YC_RES_PRO_TRANS_GLASS; }
    if (0x40000 == (flags & 0x40000)) { into->flags.transparency = YC_RES_PRO_TRANS_STEAM; }
    if (0x80000 == (flags & 0x80000)) { into->flags.transparency = YC_RES_PRO_TRANS_ENERGY; }
    if (0x10000000 == (flags & 0x10000000)) { into->flags.transparency = YC_RES_PRO_TRANS_WALL_END; }
}

void yc_res_pro_parse_cleanup(void *file, const yc_res_io_fs_api_t *io, yc_res_pro_object_header_t *header) {
    if (NULL != file) { io->fclose(file); }
    if (NULL != header) { free(header); }
}
