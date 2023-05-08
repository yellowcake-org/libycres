#include <libycres.h>

bool yc_res_math_is_valid_location(yc_res_math_location_t location) {
    bool is_elevation_valid = location.elevation_idx >= 0 && location.elevation_idx < 3;
    bool is_orientation_valid = location.orientation_idx >= 0 && location.orientation_idx <= 6;
    bool is_grid_position_valid = location.grid_idx != 0xFFFF;

    return is_elevation_valid && is_orientation_valid && is_grid_position_valid;
}
