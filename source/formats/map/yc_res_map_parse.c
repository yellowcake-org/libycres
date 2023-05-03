#include <libycres.h>
#include <private.h>

#include <stdlib.h>

void yc_res_map_parse_cleanup(void *file, const yc_res_io_fs_api_t *io, yc_res_pro_map_t *map);

yc_res_map_status_t yc_res_map_parse(
        const char *filename,
        const yc_res_io_fs_api_t *io,
        yc_res_map_parse_result_t *result
) {
    void *file = io->fopen(filename, "rb");

    if (NULL == file) {
        yc_res_map_parse_cleanup(NULL, io, NULL);
        return YC_RES_MAP_STATUS_IO;
    }

    yc_res_pro_map_t *map = malloc(sizeof(yc_res_pro_map_t));

    if (NULL == map) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_MEM;
    }

    uint32_t version = 0;
    if (0 == io->fread(&version, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }
    version = yc_res_byteorder_uint32(version);

    unsigned char _filename[16] = "";
    if (0 == io->fread(&_filename, sizeof(unsigned char), 16, file)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }

    uint32_t position = 0;
    if (0 == io->fread(&position, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }
    position = yc_res_byteorder_uint32(position);

    uint32_t elevation = 0;
    if (0 == io->fread(&elevation, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }
    elevation = yc_res_byteorder_uint32(elevation);

    uint32_t direction = 0;
    if (0 == io->fread(&direction, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }
    direction = yc_res_byteorder_uint32(direction);

    map->entrance.tile_idx = position;
    map->entrance.elevation_idx = elevation;
    map->entrance.orientation = direction;

    uint32_t local_vars_count = 0;
    if (0 == io->fread(&local_vars_count, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }
    local_vars_count = yc_res_byteorder_uint32(local_vars_count);

    if (0 == io->fread(&map->script_id, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }
    map->script_id = yc_res_byteorder_uint32(map->script_id);

    unsigned char flags[4] = {0, 0, 0, 0};
    if (0 == io->fread(&flags, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }

    map->is_save = (flags[3] & 0x01) != 0x00;

    uint32_t _darkness = 0;
    if (0 == io->fread(&_darkness, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }
    _darkness = yc_res_byteorder_uint32(_darkness);

    uint32_t global_vars_count = 0;
    if (0 == io->fread(&global_vars_count, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }
    global_vars_count = yc_res_byteorder_uint32(global_vars_count);

    if (0 == io->fread(&map->map_idx, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }
    map->map_idx = yc_res_byteorder_uint32(map->map_idx);

    if (0 == io->fread(&map->ticks, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }
    map->ticks = yc_res_byteorder_uint32(map->ticks);

    if (0 != io->fseek(file, sizeof(uint32_t) * 44, SEEK_CUR)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }

    yc_res_map_parse_cleanup(file, io, NULL);

    result->map = map;
    return YC_RES_MAP_STATUS_OK;
}

void yc_res_map_parse_cleanup(void *file, const yc_res_io_fs_api_t *io, yc_res_pro_map_t *map) {
    if (NULL != file) {
        io->fclose(file);
    }

    if (NULL != map) {
        yc_res_map_invalidate(map);
        free(map);
    }
}
