#ifndef UNDAT_PRIVATE_EXTRACT_H
#define UNDAT_PRIVATE_EXTRACT_H

#include <undat.h>

typedef struct {
    void *input; /* TODO: Move file open into iteration func! */
    char *current;
    const char *output;
} undat_private_extract_node_accum_t;

typedef enum {
    UNDAT_PRIVATE_EXTRACT_NODE_ERROR_MALLOC = 0,
    UNDAT_PRIVATE_EXTRACT_NODE_ERROR_OPEN,
    UNDAT_PRIVATE_EXTRACT_NODE_ERROR_READ,
    UNDAT_PRIVATE_EXTRACT_NODE_ERROR_WRITE,
    UNDAT_PRIVATE_EXTRACT_NODE_ERROR_CLOSE,
    UNDAT_PRIVATE_EXTRACT_NODE_ERROR_MKDIR,
    UNDAT_PRIVATE_EXTRACT_NODE_ERROR_INTERNAL
} undat_private_extract_node_error_t;

undat_iterate_handler_result_t
undat_private_extract_node(yc_res_dat_directory_t* node, void* accum, __unused unsigned long level);

#endif /* UNDAT_PRIVATE_EXTRACT_H */
