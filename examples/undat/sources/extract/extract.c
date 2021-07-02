#include <undat.h>
#include <private/extract.h>

#include <stdlib.h>
#include <memory.h>

undat_extract_tree_status_t undat_extract_tree(yc_res_dat_directory_t* root, const char** path) {
    undat_iterate_tree_result_t result;
    undat_private_extract_node_accum_t* accumulator = malloc(sizeof(*accumulator));
    
    if (NULL == accumulator) {
        return UNDAT_EXTRACT_TREE_STATUS_MALLOC;
    }
    
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
            undat_private_extract_node_error_t error = result.inner.error;
            switch (error) {
                case UNDAT_PRIVATE_EXTRACT_NODE_ERROR_MALLOC: return UNDAT_EXTRACT_TREE_STATUS_MALLOC;
            }
        }
    }
    
    return UNDAT_EXTRACT_TREE_STATUS_OK;
}
