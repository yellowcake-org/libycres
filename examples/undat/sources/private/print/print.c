#include <undat.h>
#include <private/print.h>

#include <stdio.h>

undat_iterate_handler_result_t
undat_print_node(yc_res_dat_directory_t* node, __unused void* accum, unsigned long level) {
    const edge_length = 4;
    unsigned long f;
    
    undat_iterate_handler_result_t result;
    result.status = UNDAT_ITERATE_HANDLER_STATUS_OK;
    result.error = NULL;
    
    if (level > 1) {
        printf("%*s", edge_length * ((int)level - 1), "");
    }
    
    if (level > 0) {
        printf("\\---");
    }
    
    printf("%s\n", node->name);
    
    for (f = 0; f < node->files_count; ++f) {
        printf("%*s", edge_length * (int)level, "");

        printf("\\---");
        printf("%s\n", node->files[f].name);
    }
    
    return result;
}
