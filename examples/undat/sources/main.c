#include <undat.h>

#include <stdio.h>
#include <stdlib.h>

int main(__unused int argc, char *argv[]) {
    struct arg_lit *help, *version, *list;
    struct arg_file *input;
    struct arg_end *end;
        
    void *(argtable[5]);
    char progname[] = "undat";
    
    int nerrors;
    int result = 1;
    
    input = arg_filen(NULL, "input", "<file>", 1, 1, "path to archive file");
    list = arg_litn(NULL, "list", 0, 1, "list contents");
    help = arg_litn(NULL, "help", 0, 1, "display this help and exit");
    version = arg_litn(NULL, "version", 0, 1, "display version info and exit");
    end = arg_end(2);
    
    argtable[0] = input;
    argtable[1] = list;
    argtable[2] = help;
    argtable[3] = version;
    argtable[4] = end;
    
    nerrors = arg_parse(argc,argv,argtable);

    if (help->count > 0) {
        printf("Usage: %s", progname);
        arg_print_syntaxv(stdout, argtable, "\n");
        
        printf("Utility for working with Fallout™ resource archives.\n\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\n");
                
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return 0;
    }

    if (nerrors > 0) {
        undat_print_arg_errors(end, progname);
    } else {
        FILE* file = fopen(input->filename[0], "rb");
        
        if (NULL != file) {
            if (list->count > 0) {
                yc_res_dat_directory_t *root;
                yc_res_dat_tree(&undat_platform_file_reader, file, &root);
                
                undat_iterate_tree(root, 0, &undat_print_node);
                
                yc_res_dat_free_tree(root);
                
                free(root);
                root = NULL;
                
                result = 0;
            } else {
                undat_print_arg_errors(end, progname);
            }
        }
        
        if (NULL != file) {
            fclose(file);
            file = NULL;
        }
    }
    
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    return result;
}
