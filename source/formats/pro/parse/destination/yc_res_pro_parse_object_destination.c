#include <libycres.h>

#include "yc_res_pro_parse_object_destination.h"

void yc_res_pro_parse_object_destination(uint32_t raw, yc_res_math_location_t *into) {
    const uint32_t built_tile_tile_mask = 0x3FFFFFF;
    const uint32_t built_tile_elevation_mask = 0xE0000000;
    const uint32_t built_tile_elevation_shift = 29;
    const uint32_t built_tile_rotation_mask = 0x1C000000;
    const uint32_t built_tile_rotation_shift = 26;

    into->grid_idx = raw & built_tile_tile_mask;
    into->elevation_idx = (raw & built_tile_elevation_mask) >> built_tile_elevation_shift;
    into->orientation_idx = (raw & built_tile_rotation_mask) >> built_tile_rotation_shift;
}
