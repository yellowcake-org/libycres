#include <libycres.h>

bool yc_res_pal_color_is_transparent(yc_res_pal_color_t *color) {
    return color->r == 0 && color->g == 0 && color->b == 0;
}
