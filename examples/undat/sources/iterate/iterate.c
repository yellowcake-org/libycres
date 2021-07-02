#include <undat.h>

undat_iterate_tree_result_t
undat_iterate_tree(yc_res_dat_directory_t* node, void* accum, unsigned long level, undat_iterate_handler_t* handler) {
    undat_iterate_handler_result_t inner = handler(node, accum, level);
    
    undat_iterate_tree_result_t result;
    result.inner = inner;
    
    switch (result.inner.status) {
        case UNDAT_ITERATE_HANDLER_STATUS_ERROR: break;
        case UNDAT_ITERATE_HANDLER_STATUS_OK: {
            unsigned long i;
            for (i = 0; i < node->directories_count; ++i) {
                undat_iterate_tree_result_t loop_result = undat_iterate_tree(&node->directories[i], accum, level + 1, handler);
                
                switch (loop_result.inner.status) {
                    case UNDAT_ITERATE_HANDLER_STATUS_OK: break;
                    case UNDAT_ITERATE_HANDLER_STATUS_ERROR: return loop_result;
                }
            }
            
            break;
        }
    }
    
    return result;
}
