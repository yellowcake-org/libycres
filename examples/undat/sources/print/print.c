#include <undat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

undat_iterate_handler_result_t undat_print_node(yc_res_dat_directory_t* node, __unused void* accum, unsigned long level);
void undat_print_tree(yc_res_dat_directory_t* root) {
    undat_iterate_tree(root, NULL, 0, &undat_print_node);
}

void undat_print_arg_errors(struct arg_end* end, const char* appname) {
    arg_print_errors(stdout, end, appname);
    fprintf(stderr, "Try '%s --help' for more information.\n", appname);
}

void undat_print_arg_help(void* argtable, const char* appname) {
    printf("Usage: %s", appname);
    arg_print_syntaxv(stdout, argtable, "\n");
    
    printf("Utility for working with Fallout™ resource archives.\n\n");
    arg_print_glossary(stdout, argtable, "  %-25s %s\n");
}

void undat_print_version(const char* appname) {
    printf("%s v0.3.0\n", appname);
}

undat_iterate_handler_result_t undat_print_node(yc_res_dat_directory_t* node, __unused void* accum, unsigned long level) {
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
