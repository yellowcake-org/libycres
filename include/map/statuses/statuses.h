#ifndef LIB_YCRES_MAP_STATUSES_H
#define LIB_YCRES_MAP_STATUSES_H

typedef enum yc_res_map_status {
    YC_RES_MAP_STATUS_OK = 0,
    YC_RES_MAP_STATUS_IO,
    YC_RES_MAP_STATUS_IN,
    YC_RES_MAP_STATUS_MEM,
    YC_RES_MAP_STATUS_CORR,
} yc_res_map_status_t;

#endif //LIB_YCRES_MAP_STATUSES_H
