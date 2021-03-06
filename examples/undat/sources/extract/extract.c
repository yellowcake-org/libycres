#include <undat.h>
#include <private/extract.h>

#include <stdlib.h>
#include <memory.h>

undat_extract_tree_status_t undat_extract_tree(yc_res_dat_directory_t* root, void* file, const char** path) {
    undat_iterate_tree_result_t result;
    undat_private_extract_node_accum_t* accumulator = malloc(sizeof(*accumulator));
    
    if (NULL == accumulator) {
        return UNDAT_EXTRACT_TREE_STATUS_MALLOC;
    }
    
    accumulator->input = file;
    accumulator->output = *path;
    accumulator->current = NULL;

    result = undat_iterate_tree(root, (void*)accumulator, 0, &undat_private_extract_node);
    
    if (NULL != accumulator->current) {
        free(accumulator->current);
    }
    
    free(accumulator);
    
    switch (result.inner.status) {
        case UNDAT_ITERATE_HANDLER_STATUS_OK: break;
        case UNDAT_ITERATE_HANDLER_STATUS_ERROR: {
            undat_private_extract_node_error_t error = (undat_private_extract_node_error_t)(size_t)result.inner.error;
            switch (error) {
                case UNDAT_PRIVATE_EXTRACT_NODE_ERROR_MALLOC: return UNDAT_EXTRACT_TREE_STATUS_MALLOC;
                case UNDAT_PRIVATE_EXTRACT_NODE_ERROR_OPEN: return UNDAT_EXTRACT_TREE_STATUS_OPEN;
                case UNDAT_PRIVATE_EXTRACT_NODE_ERROR_READ: return UNDAT_EXTRACT_TREE_STATUS_READ;
                case UNDAT_PRIVATE_EXTRACT_NODE_ERROR_WRITE: return UNDAT_EXTRACT_TREE_STATUS_WRITE;
                case UNDAT_PRIVATE_EXTRACT_NODE_ERROR_CLOSE: return UNDAT_EXTRACT_TREE_STATUS_CLOSE;
                case UNDAT_PRIVATE_EXTRACT_NODE_ERROR_MKDIR: return UNDAT_EXTRACT_TREE_STATUS_MKDIR;
                case UNDAT_PRIVATE_EXTRACT_NODE_ERROR_INTERNAL: return UNDAT_EXTRACT_TREE_STATUS_INTERNAL;
            }
        }
    }
    
    return UNDAT_EXTRACT_TREE_STATUS_OK;
}
