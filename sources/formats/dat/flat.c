#include <libycres.h>
#include <private.h>

void yc_res_dat_private_flatten_marked_dirs(yc_res_dat_directory_t* node, yc_res_dat_directory_t** flat, unsigned long* fi) {
    if (node->has_content_block == 1) {
        flat[(*fi)++] = node;
    }
    
    unsigned long i;
    for (i = 0; i < node->directories_count; ++i) {
        yc_res_dat_private_flatten_marked_dirs(&node->directories[i], flat, fi);
    }
}
