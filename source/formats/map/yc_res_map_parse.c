#include <libycres.h>
#include <private.h>

#include "yc_res_map_parse.h"

#include <stdlib.h>
#include <assert.h>

void yc_res_map_parse_cleanup(void *file, const yc_res_io_fs_api_t *api, yc_res_map_t *map);

yc_res_map_status_t
yc_res_map_parse_tiles(void *file, const yc_res_io_fs_api_t *api, yc_res_map_t *map);

yc_res_map_status_t
yc_res_map_parse_scripts(void *file, const yc_res_io_fs_api_t *api, yc_res_map_t *map);

yc_res_map_status_t
yc_res_map_parse_objects(void *file, const yc_res_io_fs_api_t *api, yc_res_map_t *map,
                         const yc_res_map_parse_db_api_t *fetchers);

yc_res_map_status_t yc_res_map_parse(
        const char *filename,
        const yc_res_io_fs_api_t *api,
        const yc_res_map_parse_db_api_t *fetchers,
        yc_res_map_parse_result_t *result
) {
    void *file = api->fopen(filename, "rb");

    if (NULL == file) {
        yc_res_map_parse_cleanup(NULL, api, NULL);
        return YC_RES_MAP_STATUS_IO;
    }

    yc_res_map_t *map = malloc(sizeof(yc_res_map_t));
    if (NULL == map) {
        yc_res_map_parse_cleanup(file, api, map);
        return YC_RES_MAP_STATUS_MEM;
    }

    uint32_t version = 0;
    if (0 == api->fread(&version, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, api, map);
        return YC_RES_MAP_STATUS_IO;
    }
    version = yc_res_byteorder_uint32(version);

    unsigned char map_filename[16] = "";
    if (0 == api->fread(&map_filename, sizeof(unsigned char), 16, file)) {
        yc_res_map_parse_cleanup(file, api, map);
        return YC_RES_MAP_STATUS_IO;
    }

    int32_t position = 0;
    if (0 == api->fread(&position, sizeof(int32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, api, map);
        return YC_RES_MAP_STATUS_IO;
    }
    position = yc_res_byteorder_int32(position);

    uint32_t elevation = 0;
    if (0 == api->fread(&elevation, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, api, map);
        return YC_RES_MAP_STATUS_IO;
    }
    elevation = yc_res_byteorder_uint32(elevation);

    uint32_t direction = 0;
    if (0 == api->fread(&direction, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, api, map);
        return YC_RES_MAP_STATUS_IO;
    }
    direction = yc_res_byteorder_uint32(direction);

    map->entrance.grid_idx = position;
    map->entrance.elevation_idx = elevation;
    map->entrance.orientation_idx = direction;

    if (0 == api->fread(&map->local.count, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, api, map);
        return YC_RES_MAP_STATUS_IO;
    }
    map->local.count = yc_res_byteorder_uint32((uint32_t) map->local.count);

    if (0 == api->fread(&map->script_idx, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, api, map);
        return YC_RES_MAP_STATUS_IO;
    }
    map->script_idx = yc_res_byteorder_uint32(map->script_idx);

    unsigned char flags[4] = {0, 0, 0, 0};
    if (0 == api->fread(&flags, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, api, map);
        return YC_RES_MAP_STATUS_IO;
    }

    map->is_save = (flags[3] & 0x01) != 0x00;

    for (size_t elevation_idx = 0, byte = 0x02;
         elevation_idx < YC_RES_MAP_ELEVATION_COUNT; ++elevation_idx, byte <<= 1) {
        if ((flags[3] & byte) != 0x00) {
            map->levels[elevation_idx] = NULL;
        } else {
            map->levels[elevation_idx] = malloc(sizeof(yc_res_map_level_t));

            if (NULL == map->levels[elevation_idx]) {
                yc_res_map_parse_cleanup(file, api, map);
                return YC_RES_MAP_STATUS_MEM;
            }
        }
    }

    uint32_t darkness = 0;
    if (0 == api->fread(&darkness, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, api, map);
        return YC_RES_MAP_STATUS_IO;
    }
    darkness = yc_res_byteorder_uint32(darkness);

    if (0 == api->fread(&map->global.count, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, api, map);
        return YC_RES_MAP_STATUS_IO;
    }
    map->global.count = yc_res_byteorder_uint32((uint32_t) map->global.count);

    if (0 == api->fread(&map->map_idx, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, api, map);
        return YC_RES_MAP_STATUS_IO;
    }
    map->map_idx = yc_res_byteorder_uint32(map->map_idx);

    if (0 == api->fread(&map->ticks, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_cleanup(file, api, map);
        return YC_RES_MAP_STATUS_IO;
    }
    map->ticks = yc_res_byteorder_uint32(map->ticks);

    if (0 != api->fseek(file, sizeof(uint32_t) * 44, SEEK_CUR)) {
        yc_res_map_parse_cleanup(file, api, map);
        return YC_RES_MAP_STATUS_IO;
    }

    if (0 < map->global.count) {
        map->global.values = malloc(sizeof(int32_t) * map->global.count);

        if (NULL == map->global.values) {
            yc_res_map_parse_cleanup(file, api, map);
            return YC_RES_MAP_STATUS_MEM;
        }

        if (0 == api->fread(map->global.values, sizeof(int32_t), map->global.count, file)) {
            yc_res_map_parse_cleanup(file, api, map);
            return YC_RES_MAP_STATUS_IO;
        }

        for (size_t var_idx = 0; var_idx < map->global.count; ++var_idx) {
            map->global.values[var_idx] = yc_res_byteorder_int32(map->global.values[var_idx]);
        }
    }

    if (0 < map->local.count) {
        map->local.values = malloc(sizeof(int32_t) * map->local.count);

        if (NULL == map->local.values) {
            yc_res_map_parse_cleanup(file, api, map);
            return YC_RES_MAP_STATUS_MEM;
        }

        if (0 == api->fread(map->local.values, sizeof(int32_t), map->local.count, file)) {
            yc_res_map_parse_cleanup(file, api, map);
            return YC_RES_MAP_STATUS_IO;
        }

        for (size_t var_idx = 0; var_idx < map->local.count; ++var_idx) {
            map->local.values[var_idx] = yc_res_byteorder_int32(map->local.values[var_idx]);
        }
    }

    yc_res_map_status_t status = yc_res_map_parse_tiles(file, api, map);
    if (YC_RES_MAP_STATUS_OK != status) {
        yc_res_map_parse_cleanup(file, api, map);
        return status;
    }

    status = yc_res_map_parse_scripts(file, api, map);
    if (YC_RES_MAP_STATUS_OK != status) {
        yc_res_map_parse_cleanup(file, api, map);
        return status;
    }

    status = yc_res_map_parse_objects(file, api, map, fetchers);
    if (YC_RES_MAP_STATUS_OK != status) {
        yc_res_map_parse_cleanup(file, api, map);
        return status;
    }

    yc_res_map_parse_cleanup(file, api, NULL);

    result->map = map;
    return YC_RES_MAP_STATUS_OK;
}

yc_res_map_status_t yc_res_map_parse_tiles(void *file, const yc_res_io_fs_api_t *api, yc_res_map_t *map) {
    for (size_t elevation_idx = 0; elevation_idx < YC_RES_MAP_ELEVATION_COUNT; ++elevation_idx) {
        yc_res_map_level_t *level = map->levels[elevation_idx];

        if (NULL != level) {
            size_t const grid_size_vertical = sizeof(level->floor.idxes) / sizeof(level->floor.idxes[0]);
            size_t const grid_size_horizontal = sizeof(level->floor.idxes[0]) / sizeof(level->floor.idxes[0][0]);

            assert(grid_size_horizontal == grid_size_horizontal);

            for (size_t pos_y = 0; pos_y < grid_size_vertical; ++pos_y) {
                for (size_t pos_x = 0; pos_x < grid_size_horizontal; ++pos_x) {
                    uint16_t roof_idx = 0, floor_idx = 0;

                    if (0 == api->fread(&roof_idx, sizeof(uint16_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
                    if (0 == api->fread(&floor_idx, sizeof(uint16_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }

                    level->roof.idxes[pos_y][grid_size_horizontal - 1 - pos_x] =
                            yc_res_pro_index_from_sprite_id(yc_res_byteorder_uint16(roof_idx));

                    level->floor.idxes[pos_y][grid_size_horizontal - 1 - pos_x] =
                        yc_res_pro_index_from_sprite_id(yc_res_byteorder_uint16(floor_idx));
                }
            }
        }
    }

    return YC_RES_MAP_STATUS_OK;
}

yc_res_map_status_t yc_res_map_parse_scripts(void *file, const yc_res_io_fs_api_t *api, yc_res_map_t *map) {
    map->scripts.count = 0;
    size_t consumed_idx = 0;

    for (size_t script_type = 0; script_type < YC_RES_PRO_SCRIPT_TYPE_COUNT; ++script_type) {
        uint32_t count = 0;
        if (0 == api->fread(&count, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
        count = yc_res_byteorder_uint32(count);

        map->scripts.count += count;
        size_t size = sizeof(yc_res_map_script_t) * map->scripts.count;

        yc_res_map_script_t *scripts =
                0 == map->scripts.count - count ? malloc(size) : realloc(map->scripts.pointers, size);
        if (NULL == scripts) { return YC_RES_MAP_STATUS_MEM; }

        map->scripts.pointers = scripts;

        const size_t batch_length = 16;
        size_t batches = count / batch_length + (count % batch_length != 0);

        size_t processed = 0;
        for (size_t batch_idx = 0; batch_idx < batches; ++batch_idx) {
            for (size_t record_idx = 0; record_idx < batch_length; ++record_idx) {
                if (batch_idx * batch_length + record_idx < count) {
                    yc_res_map_script_t *script = &map->scripts.pointers[consumed_idx++];
                    yc_res_map_status_t status = yc_res_map_parse_script(
                            file, api, script, (uint32_t) script_type
                    );

                    if (YC_RES_MAP_STATUS_OK != status) { return status; }
                } else {
                    unsigned char sid[4];
                    if (0 == api->fread(&sid, sizeof(unsigned char) * 4, 1, file)) { return YC_RES_MAP_STATUS_IO; }

                    yc_res_pro_script_type_t type = sid[0];
                    long fields_count = 15 + (type == YC_RES_PRO_SCRIPT_TYPE_SPATIAL ? 2 : 0);

                    if (0 != api->fseek(file, (long) sizeof(uint32_t) * fields_count, SEEK_CUR)) {
                        return YC_RES_MAP_STATUS_IO;
                    }
                }
            }

            uint32_t checksum = 0;
            if (0 == api->fread(&checksum, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
            processed += yc_res_byteorder_uint32(checksum);

            if (0 != api->fseek(file, sizeof(uint32_t), SEEK_CUR)) { return YC_RES_MAP_STATUS_IO; }
        }

        if (processed != count) { return YC_RES_MAP_STATUS_CORR; }
    }

    return YC_RES_MAP_STATUS_OK;
}

yc_res_map_status_t yc_res_map_parse_objects(
        void *file, const yc_res_io_fs_api_t *api, yc_res_map_t *map,
        const yc_res_map_parse_db_api_t *fetchers
) {
    uint32_t total_objects_count = 0;
    if (0 == api->fread(&total_objects_count, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    total_objects_count = yc_res_byteorder_uint32(total_objects_count);

    for (size_t elevation_idx = 0; elevation_idx < YC_RES_MAP_ELEVATION_COUNT; ++elevation_idx) {
        yc_res_map_level_t *level = map->levels[elevation_idx];

        if (NULL != level) {
            if (0 == api->fread(&level->objects.count, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
            level->objects.count = yc_res_byteorder_uint32((uint32_t) level->objects.count);

            level->objects.pointers = malloc(sizeof(yc_res_map_level_object_t) * level->objects.count);
            if (NULL == level->objects.pointers) { return YC_RES_MAP_STATUS_MEM; }

            for (size_t object_idx = 0; object_idx < level->objects.count; ++object_idx) {
                yc_res_map_level_object_t *object = &level->objects.pointers[object_idx];
                if (NULL == object) { continue; }

                yc_res_map_status_t status = yc_res_map_parse_object(file, api, fetchers, object);
                if (YC_RES_MAP_STATUS_OK != status) { return status; }
            }
        }
    }

    return YC_RES_MAP_STATUS_OK;
}

void yc_res_map_parse_cleanup(void *file, const yc_res_io_fs_api_t *api, yc_res_map_t *map) {
    if (NULL != file) { api->fclose(file); }

    if (NULL != map) {
        yc_res_map_invalidate(map);
        free(map);
    }
}
