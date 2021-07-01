#include <undat.h>
#include <stdio.h>

void undat_print_node(yc_res_dat_directory_t* node, unsigned long level) {
    const edge_length = 4;
    unsigned long f;
    
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
}

void undat_print_arg_errors(struct arg_end* end, char* name) {
    arg_print_errors(stdout, end, name);
    fprintf(stderr, "Try '%s --help' for more information.\n", name);
}
