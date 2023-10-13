#ifndef LIBYCRES_INCLUDE_MAP_TYPES_LEVEL_H
#define LIBYCRES_INCLUDE_MAP_TYPES_LEVEL_H

#include "exit/exit.h"
#include "object/object.h"
#include "tiles/tiles.h"

typedef struct yc_res_map_level {
    yc_res_map_level_tiles_t floor, roof;
    yc_res_map_level_objects_t objects;
} yc_res_map_level_t;

#endif //LIBYCRES_INCLUDE_MAP_TYPES_LEVEL_H
