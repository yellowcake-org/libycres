#include <libycres.h>
#include <stdlib.h>

void yc_res_map_invalidate(yc_res_map_t *map) {
    for (size_t elevation_idx = 0; elevation_idx < YC_RES_MAP_ELEVATION_COUNT; ++elevation_idx) {
        yc_res_map_level_t *level = map->levels[elevation_idx];

        if (NULL != level->objects.pointers) {
            for (size_t object_idx = 0; object_idx < level->objects.count; ++object_idx) {
                // invalidate object
            }

            free(level->objects.pointers);
            level->objects.pointers = NULL;
        }

        level->objects.count = 0;

        if (NULL != level) {
            free(level);
            map->levels[elevation_idx] = NULL;
        }
    }

    map->local.count = 0;
    if (NULL != map->local.values) {
        free(map->local.values);
        map->local.values = NULL;
    }

    map->global.count = 0;
    if (NULL != map->global.values) {
        free(map->global.values);
        map->global.values = NULL;
    }

    if (NULL != map->scripts.pointers) {
        for (size_t script_idx = 0; script_idx < map->scripts.count; ++script_idx) {
            yc_res_map_script_t *script = &map->scripts.pointers[script_idx];

            if (NULL != script->data.timed) {
                free(script->data.timed);
                script->data.timed = NULL;
            }

            if (NULL != script->data.spatial) {
                free(script->data.spatial);
                script->data.spatial = NULL;
            }
        }

        map->scripts.count = 0;

        free(map->scripts.pointers);
        map->scripts.pointers = NULL;
    }
}
