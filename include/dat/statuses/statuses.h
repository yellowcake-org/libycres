#ifndef INCLUDE_DAT_STATUSES_H
#define INCLUDE_DAT_STATUSES_H

typedef enum yc_res_dat_status {
    YC_RES_DAT_STATUS_OK = 0,
    YC_RES_DAT_STATUS_IO,
    YC_RES_DAT_STATUS_MEM,
    YC_RES_DAT_STATUS_CORR,
} yc_res_dat_status_t;

#endif //INCLUDE_DAT_STATUSES_H
