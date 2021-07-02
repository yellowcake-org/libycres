#ifndef UNDAT_EXTRACT_H
#define UNDAT_EXTRACT_H

#include <undat.h>

typedef enum {
    UNDAT_EXTRACT_TREE_STATUS_OK,
    UNDAT_EXTRACT_TREE_STATUS_MALLOC,
    UNDAT_EXTRACT_TREE_STATUS_OPEN,
    UNDAT_EXTRACT_TREE_STATUS_READ,
    UNDAT_EXTRACT_TREE_STATUS_WRITE,
    UNDAT_EXTRACT_TREE_STATUS_CLOSE,
    UNDAT_EXTRACT_TREE_STATUS_MKDIR,
    UNDAT_EXTRACT_TREE_STATUS_INTERNAL
} undat_extract_tree_status_t;

undat_extract_tree_status_t undat_extract_tree(yc_res_dat_directory_t* root, void* file, const char** path);

#endif /* UNDAT_EXTRACT_H */
