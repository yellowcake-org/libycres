#include <libycres.h>

#include <stdlib.h>

void yc_res_pal_parse_cleanup(
        void *file,
        const yc_res_io_fs_api_t *api,
        yc_res_pal_color_t *colors
);

yc_res_pal_status_t yc_res_pal_parse(
        const char *filename,
        const yc_res_io_fs_api_t *api,
        yc_res_pal_parse_result_t *result
) {
    void *palette = api->fopen(filename, "rb");

    if (NULL == palette) {
        yc_res_pal_parse_cleanup(NULL, api, NULL);
        return YC_RES_PAL_STATUS_IO;
    }

    const size_t LENGTH = 256;
    yc_res_pal_color_t *colors = malloc(sizeof(yc_res_pal_color_t) * LENGTH);

    if (NULL == colors) {
        yc_res_pal_parse_cleanup(palette, api, NULL);
        return YC_RES_PAL_STATUS_MEM;
    }

    if (0 == api->fread(colors, sizeof(yc_res_pal_color_t), LENGTH, palette)) {
        yc_res_pal_parse_cleanup(palette, api, colors);
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

    yc_res_pal_parse_cleanup(palette, api, NULL);

    result->count = LENGTH;
    result->colors = colors;

    return YC_RES_PAL_STATUS_OK;
}

void yc_res_pal_parse_cleanup(
        void *file,
        const yc_res_io_fs_api_t *api,
        yc_res_pal_color_t *colors
) {
    if (NULL != file) {
        api->fclose(file);
    }

    if (NULL != colors) {
        free(colors);
    }
}
