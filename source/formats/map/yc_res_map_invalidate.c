#include <libycres.h>
#include <stdlib.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void yc_res_map_invalidate(yc_res_pro_map_t *map) {
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
