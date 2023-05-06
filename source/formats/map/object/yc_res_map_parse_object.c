#include <libycres.h>
#include <private.h>

#include "yc_res_map_parse_object.h"

#include <stdlib.h>

void yc_res_map_parse_object_cleanup(yc_res_map_level_object_t *into);

yc_res_map_status_t yc_res_map_parse_object(
        void *file, const yc_res_io_fs_api_t *io, yc_res_map_level_object_t *into
) {
    uint32_t _entry_id = 0;
    if (0 == io->fread(&_entry_id, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_object_cleanup(into);
        return YC_RES_MAP_STATUS_IO;
    }
    _entry_id = yc_res_byteorder_uint32(_entry_id);

    if (0 == io->fread(&into->hex_idx, sizeof(uint32_t), 1, file)) {
        yc_res_map_parse_object_cleanup(into);
        return YC_RES_MAP_STATUS_IO;
    }
    into->hex_idx = yc_res_byteorder_uint32(into->hex_idx);

    return YC_RES_MAP_STATUS_OK;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void yc_res_map_parse_object_cleanup(yc_res_map_level_object_t *into) {
    //
}
#pragma clang diagnostic pop
