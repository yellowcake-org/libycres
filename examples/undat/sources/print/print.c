#include <undat.h>
#include <stdio.h>

void undat_print_node(yc_res_dat_directory_t* node, unsigned long level) {
    unsigned long l, f;
    
    for (l = 0; l < level; ++l) {
        if (1 == level - l) {
            printf("\\---");
        } else {
            printf("    ");
        }
    }
    
    printf("%s\n", node->name);
    
    for (f = 0; f < node->files_count; ++f) {
        for (l = 0; l < level; ++l) {
            printf("    ");
        }

        printf("\\---");
        printf("%s\n", node->files[f].name);
    }
}

void undat_print_arg_errors(struct arg_end* end, char* name) {
    arg_print_errors(stdout, end, name);
    printf("Try '%s --help' for more information.\n", name);
}
