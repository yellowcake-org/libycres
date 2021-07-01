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
