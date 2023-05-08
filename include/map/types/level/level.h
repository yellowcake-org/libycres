#ifndef LIB_YCRES_MAP_TYPES_LEVEL_H
#define LIB_YCRES_MAP_TYPES_LEVEL_H

#include "exit/exit.h"
#include "tiles/tiles.h"
#include "object/object.h"

typedef struct yc_res_map_level {
    yc_res_map_level_tiles_t floor, roof;
    yc_res_map_level_objects_t objects;
} yc_res_map_level_t;

#endif //LIB_YCRES_MAP_TYPES_LEVEL_H
