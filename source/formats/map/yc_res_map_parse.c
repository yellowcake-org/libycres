#include <libycres.h>
#include <private.h>

#include "yc_res_map_parse.h"

#include <stdlib.h>
#include <assert.h>

void yc_res_map_parse_cleanup(void *file, const yc_res_io_fs_api_t *io, yc_res_map_t *map);

yc_res_map_status_t yc_res_map_parse(
        const char *filename,
        const yc_res_io_fs_api_t *io,
        const yc_res_map_parse_db_api_t *db,
        yc_res_map_parse_result_t *result
) {
    void *file = io->fopen(filename, "rb");

    if (NULL == file) {
        yc_res_map_parse_cleanup(NULL, io, NULL);
        return YC_RES_MAP_STATUS_IO;
    }

    yc_res_map_t *map = malloc(sizeof(yc_res_map_t));

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

    map->entrance.grid_idx = position;
    map->entrance.elevation_idx = elevation;
    map->entrance.orientation_idx = direction;

    if (0 == io->fread(&map->local.count, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }
    map->local.count = yc_res_byteorder_uint32(map->local.count);

    if (0 == io->fread(&map->script_idx, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }
    map->script_idx = yc_res_byteorder_uint32(map->script_idx);

    unsigned char flags[4] = {0, 0, 0, 0};
    if (0 == io->fread(&flags, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }

    map->is_save = (flags[3] & 0x01) != 0x00;

    for (size_t elevation_idx = 0, byte = 0x02;
         elevation_idx < YC_RES_MAP_ELEVATION_COUNT;
         ++elevation_idx, byte <<= 1) {
        if ((flags[3] & byte) == 0x00) {
            map->levels[elevation_idx] = malloc(sizeof(yc_res_map_level_t));

            if (NULL == map->levels[elevation_idx]) {
                yc_res_map_parse_cleanup(file, io, map);
                return YC_RES_MAP_STATUS_MEM;
            }
        }
    }

    uint32_t _darkness = 0;
    if (0 == io->fread(&_darkness, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }
    _darkness = yc_res_byteorder_uint32(_darkness);

    if (0 == io->fread(&map->global.count, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }
    map->global.count = yc_res_byteorder_uint32(map->global.count);

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

    if (0 < map->global.count) {
        map->global.values = malloc(sizeof(int32_t) * map->global.count);

        if (NULL == map->global.values) {
            yc_res_map_parse_cleanup(file, io, map);
            return YC_RES_MAP_STATUS_MEM;
        }

        if (0 == io->fread(map->global.values, sizeof(int32_t), map->global.count, file)) {
            yc_res_map_parse_cleanup(file, io, map);
            return YC_RES_MAP_STATUS_IO;
        }

        for (size_t var_idx = 0; var_idx < map->global.count; ++var_idx) {
            map->global.values[var_idx] = yc_res_byteorder_int32(map->global.values[var_idx]);
        }
    }

    if (0 < map->local.count) {
        map->local.values = malloc(sizeof(int32_t) * map->local.count);

        if (NULL == map->local.values) {
            yc_res_map_parse_cleanup(file, io, map);
            return YC_RES_MAP_STATUS_MEM;
        }

        if (0 == io->fread(map->local.values, sizeof(int32_t), map->local.count, file)) {
            yc_res_map_parse_cleanup(file, io, map);
            return YC_RES_MAP_STATUS_IO;
        }

        for (size_t var_idx = 0; var_idx < map->local.count; ++var_idx) {
            map->local.values[var_idx] = yc_res_byteorder_int32(map->local.values[var_idx]);
        }
    }

    for (size_t elevation_idx = 0; elevation_idx < YC_RES_MAP_ELEVATION_COUNT; ++elevation_idx) {
        yc_res_map_level_t *level = map->levels[elevation_idx];

        if (NULL != level) {
            size_t const GRID_SIZE_HORIZONTAL = sizeof(level->floor.idxes) / sizeof(level->floor.idxes[0]);
            size_t const GRID_SIZE_VERTICAL = sizeof(level->floor.idxes[0]) / sizeof(level->floor.idxes[0][0]);

            assert(GRID_SIZE_HORIZONTAL == GRID_SIZE_HORIZONTAL);

            for (size_t y = 0; y < GRID_SIZE_VERTICAL; ++y) {
                for (size_t x = 0; x < GRID_SIZE_HORIZONTAL; ++x) {
                    uint16_t roof_idx = 0, floor_idx = 0;

                    if (0 == io->fread(&roof_idx, sizeof(uint16_t), 1, file)) {
                        yc_res_map_parse_cleanup(file, io, map);
                        return YC_RES_MAP_STATUS_IO;
                    }

                    if (0 == io->fread(&floor_idx, sizeof(uint16_t), 1, file)) {
                        yc_res_map_parse_cleanup(file, io, map);
                        return YC_RES_MAP_STATUS_IO;
                    }

                    level->roof.idxes[GRID_SIZE_HORIZONTAL - 1 - x][y] = yc_res_byteorder_uint16(roof_idx);
                    level->floor.idxes[GRID_SIZE_HORIZONTAL - 1 - x][y] = yc_res_byteorder_uint16(floor_idx);
                }
            }
        }
    }

    map->scripts.count = 0;
    size_t consumed_idx = 0;

    for (size_t script_type = 0; script_type < YC_RES_PRO_SCRIPT_TYPE_COUNT; ++script_type) {
        uint32_t count = 0;
        if (0 == io->fread(&count, sizeof(uint32_t), 1, file)) {
            yc_res_map_parse_cleanup(file, io, map);
            return YC_RES_MAP_STATUS_IO;
        }
        count = yc_res_byteorder_uint32(count);

        map->scripts.count += count;
        size_t size = sizeof(yc_res_map_script_t) * map->scripts.count;

        yc_res_map_script_t *scripts =
                map->scripts.pointers == NULL ? malloc(size) : realloc(map->scripts.pointers, size);

        if (NULL == scripts) {
            yc_res_map_parse_cleanup(file, io, map);
            return YC_RES_MAP_STATUS_MEM;
        } else {
            map->scripts.pointers = scripts;
        }

        const size_t BATCH_LENGTH = 16;
        size_t batches = count / BATCH_LENGTH + (count % BATCH_LENGTH != 0);

        size_t processed = 0;
        for (size_t batch_idx = 0; batch_idx < batches; ++batch_idx) {
            for (size_t record_idx = 0; record_idx < BATCH_LENGTH; ++record_idx) {
                if (batch_idx * BATCH_LENGTH + record_idx < count) {
                    yc_res_map_script_t *script = &map->scripts.pointers[consumed_idx++];
                    yc_res_map_status_t status = yc_res_map_parse_script(
                            file, io, script, script_type
                    );

                    if (YC_RES_MAP_STATUS_OK != status) {
                        yc_res_map_parse_cleanup(file, io, map);
                        return status;
                    }
                } else {
                    unsigned char sid[4];
                    if (0 == io->fread(&sid, sizeof(unsigned char) * 4, 1, file)) {
                        yc_res_map_parse_cleanup(file, io, map);
                        return YC_RES_MAP_STATUS_IO;
                    }

                    yc_res_pro_script_type_t type = sid[0];
                    long fields_count = 15 + (type == YC_RES_PRO_SCRIPT_TYPE_SPATIAL ? 2 : 0);

                    if (0 != io->fseek(file, (long) sizeof(uint32_t) * fields_count, SEEK_CUR)) {
                        yc_res_map_parse_cleanup(file, io, map);
                        return YC_RES_MAP_STATUS_IO;
                    }
                }
            }

            uint32_t checksum = 0;
            if (0 == io->fread(&checksum, sizeof(uint32_t), 1, file)) {
                yc_res_map_parse_cleanup(file, io, map);
                return YC_RES_MAP_STATUS_IO;
            }
            processed += yc_res_byteorder_uint32(checksum);

            if (0 != io->fseek(file, sizeof(uint32_t), SEEK_CUR)) {
                yc_res_map_parse_cleanup(file, io, map);
                return YC_RES_MAP_STATUS_IO;
            }
        }

        if (processed != count) {
            yc_res_map_parse_cleanup(file, io, map);
            return YC_RES_MAP_STATUS_CORR;
        }
    }

    uint32_t total_objects_count = 0;
    if (0 == io->fread(&total_objects_count, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, io, map);
        return YC_RES_MAP_STATUS_IO;
    }
    total_objects_count = yc_res_byteorder_uint32(total_objects_count);

    for (size_t elevation_idx = 0; elevation_idx < YC_RES_MAP_ELEVATION_COUNT; ++elevation_idx) {
        yc_res_map_level_t *level = map->levels[elevation_idx];

        if (NULL != level) {
            if (0 == io->fread(&level->objects.count, sizeof(uint32_t), 1, file)) {
                yc_res_map_parse_cleanup(file, io, map);
                return YC_RES_MAP_STATUS_IO;
            }
            level->objects.count = yc_res_byteorder_uint32(level->objects.count);

            level->objects.pointers = malloc(sizeof(yc_res_map_level_object_t) * level->objects.count);
            if (NULL == level->objects.pointers) {
                yc_res_map_parse_cleanup(file, io, map);
                return YC_RES_MAP_STATUS_MEM;
            }

            for (size_t object_idx = 0; object_idx < level->objects.count; ++object_idx) {
                yc_res_map_level_object_t *object = &level->objects.pointers[object_idx];
                if (NULL == object) { continue; }

                yc_res_map_status_t status = yc_res_map_parse_object(file, io, db, object);
                if (YC_RES_MAP_STATUS_OK != status) {
                    yc_res_map_parse_cleanup(file, io, map);
                    return status;
                }
            }
        }
    }

    yc_res_map_parse_cleanup(file, io, NULL);

    result->map = map;
    return YC_RES_MAP_STATUS_OK;
}

void yc_res_map_parse_cleanup(void *file, const yc_res_io_fs_api_t *io, yc_res_map_t *map) {
    if (NULL != file) { io->fclose(file); }

    if (NULL != map) {
        yc_res_map_invalidate(map);
        free(map);
    }
}
