#include <libycres.h>

#include <stdio.h>
#include <stdlib.h>

void yc_res_pal_parse_cleanup(FILE *palette, yc_res_pal_color_t *colors);

yc_res_pal_status_t yc_res_pal_parse(const char *filename, yc_res_pal_colors_cb_t *callback) {
    FILE *palette = fopen(filename, "rb");

    if (NULL == palette) {
        yc_res_pal_parse_cleanup(NULL, NULL);
        return YC_RES_PAL_STATUS_IO;
    }

    const size_t LENGTH = 256;
    yc_res_pal_color_t *colors = malloc(sizeof(yc_res_pal_color_t) * LENGTH);

    if (NULL == colors) {
        yc_res_pal_parse_cleanup(palette, NULL);
        return YC_RES_PAL_STATUS_MEM;
    }

    if (0 == fread(colors, sizeof(yc_res_pal_color_t), LENGTH, palette)) {
        yc_res_pal_parse_cleanup(palette, colors);
        return YC_RES_PAL_STATUS_IO;
    }

    for (size_t idx = 0; idx < LENGTH; ++idx) {
        yc_res_pal_color_t *color = &colors[idx];

        const size_t MAX = (LENGTH / 4) - 1;
        yc_res_pal_color_t transparent = {.r = 0, .g = 0, .b = 0};

        if (color->r > MAX || color->g > MAX || color->b > MAX) {
            *color = transparent;
        } else {
            color->r *= 4;
            color->g *= 4;
            color->b *= 4;
        }
    }

    callback(colors, LENGTH);
    return YC_RES_PAL_STATUS_OK;
}

void yc_res_pal_parse_cleanup(FILE *palette, yc_res_pal_color_t *colors) {
    if (NULL != palette) {
        fclose(palette);
    }

    if (NULL != colors) {
        free(colors);
    }
}
