#include <undat.h>

void undat_iterate_tree(yc_res_dat_directory_t* node, void* accum, unsigned long level, undat_iterate_handler_t* handler) {
    unsigned long i;
    
    handler(node, accum, level);

    for (i = 0; i < node->directories_count; ++i) {
        undat_iterate_tree(&node->directories[i], accum, level + 1, handler);
    }
}
