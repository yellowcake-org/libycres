#include <libycres.h>

#include "yc_res_pro_parse_object_destination.h"

void yc_res_pro_parse_object_destination(uint32_t raw, yc_res_pro_destination_t *into) {
    const uint32_t BUILT_TILE_TILE_MASK = 0x3FFFFFF;
    const uint32_t BUILT_TILE_ELEVATION_MASK = 0xE0000000;
    const uint32_t BUILT_TILE_ELEVATION_SHIFT = 29;
    const uint32_t BUILT_TILE_ROTATION_MASK = 0x1C000000;
    const uint32_t BUILT_TILE_ROTATION_SHIFT = 26;

    into->tile_idx = raw & BUILT_TILE_TILE_MASK;
    into->elevation_idx = (raw & BUILT_TILE_ELEVATION_MASK) >> BUILT_TILE_ELEVATION_SHIFT;
    into->orientation = (raw & BUILT_TILE_ROTATION_MASK) >> BUILT_TILE_ROTATION_SHIFT;
}
