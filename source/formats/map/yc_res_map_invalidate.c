#include <libycres.h>
#include <stdlib.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void yc_res_map_invalidate(yc_res_map_t *map) {
    for (size_t elevation_idx = 0; elevation_idx < YC_RES_MAP_ELEVATION_COUNT; ++elevation_idx) {
        yc_res_map_level_t *level = map->levels[elevation_idx];

        if (NULL != level) {
            free(level);
            map->levels[elevation_idx] = NULL;
        }
    }

    map->count_lvars = 0;
    if (NULL != map->lvars) {
        free(map->lvars);
        map->lvars = NULL;
    }

    map->count_gvars = 0;
    if (NULL != map->gvars) {
        free(map->gvars);
        map->gvars = NULL;
    }

    if (NULL != map->scripts) {
        for (size_t script_idx = 0; script_idx < map->count_scripts; ++script_idx) {
            yc_res_map_script_t *script = &map->scripts[script_idx];

            if (NULL != script->data.timed) {
                free(script->data.timed);
                script->data.timed = NULL;
            }

            if (NULL != script->data.spatial) {
                free(script->data.spatial);
                script->data.spatial = NULL;
            }
        }

        map->count_scripts = 0;

        free(map->scripts);
        map->scripts = NULL;
    }
}
#pragma clang diagnostic pop
