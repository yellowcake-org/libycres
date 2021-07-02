#include <undat.h>
#include <private/print.h>

#include <stdio.h>

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
