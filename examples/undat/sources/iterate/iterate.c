#include <undat.h>
#include <stdio.h>

void undat_print_tree(yc_res_dat_directory_t* node, unsigned long level) {
    unsigned long l, f, d;
    
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

    for (d = 0; d < node->directories_count; ++d) {
        undat_print_tree(&node->directories[d], level + 1);
    }
}
