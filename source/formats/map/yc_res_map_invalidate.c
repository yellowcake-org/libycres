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

    map->count_local_variables = 0;
    if (NULL != map->local_variables) {
        free(map->local_variables);
        map->local_variables = NULL;
    }

    map->count_global_variables = 0;
    if (NULL != map->global_variables) {
        free(map->global_variables);
        map->global_variables = NULL;
    }
}
#pragma clang diagnostic pop
