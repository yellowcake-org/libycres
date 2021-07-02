#ifndef UNDAT_ITERATE_H
#define UNDAT_ITERATE_H

#include <undat.h>

typedef enum {
    UNDAT_ITERATE_HANDLER_STATUS_OK = 0,
    UNDAT_ITERATE_HANDLER_STATUS_ERROR
} undat_iterate_handler_status_t;

typedef struct {
    undat_iterate_handler_status_t status;
    void* error;
} undat_iterate_handler_result_t;

typedef undat_iterate_handler_result_t
undat_iterate_handler_t(yc_res_dat_directory_t* node, void* accum, unsigned long level);

typedef struct {
    undat_iterate_handler_result_t inner;
} undat_iterate_tree_result_t;

undat_iterate_tree_result_t
undat_iterate_tree(yc_res_dat_directory_t* node, void* accum, unsigned long level, undat_iterate_handler_t* handler);

#endif /* UNDAT_ITERATE_H */
