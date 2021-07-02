#include <undat.h>
#include <private/extract.h>

#include <stdlib.h>
#include <memory.h>

void undat_extract_tree(yc_res_dat_directory_t* root, const char** path) {
    undat_iterate_tree_result_t result;
    undat_private_extract_iterate_accum_t* accumulator = malloc(sizeof(*accumulator));
    
    accumulator->output = *path;
    accumulator->current = NULL;

    result = undat_iterate_tree(root, (void*)accumulator, 0, &undat_private_extract_node);
    switch (result.inner.status) {
        case UNDAT_ITERATE_HANDLER_STATUS_OK: break;
        case UNDAT_ITERATE_HANDLER_STATUS_ERROR: {
            undat_private_extract_node_error_t error = result.inner.error;
            switch (error) {
                case UNDAT_PRIVATE_EXTRACT_NODE_ERROR_MALLOC: break;
            }
        }
    }
    
    if (NULL != accumulator->current) {
        free(accumulator->current);
    }
    
    free(accumulator);
}
