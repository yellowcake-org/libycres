#ifndef LIB_YCRES_MAP_TYPES_SCRIPT_H
#define LIB_YCRES_MAP_TYPES_SCRIPT_H

#include "timed/timed.h"
#include "spatial/spatial.h"

typedef union yc_res_map_script_data {
    yc_res_map_script_timed_t *timed;
    yc_res_map_script_spatial_t *spatial;
} yc_res_map_script_data_t;

typedef struct yc_res_map_script {
    uint32_t script_id, program_id, object_id;

    size_t variables_offset, variables_count;
    yc_res_map_script_data_t data;
} yc_res_map_script_t;

#endif //LIB_YCRES_MAP_TYPES_SCRIPT_H
