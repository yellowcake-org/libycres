#include <undat.h>

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <errno.h>
#include <assert.h>

int main(int argc, char *argv[]) {
    struct arg_lit *help, *version, *list;
    struct arg_file *input, *output;
    struct arg_end *end;
    
    int nerrors;
    int result = 1;
        
    void* argtable[6];
    char app_name[] = "undat";
    char app_version[] = "0.3.2";
    
    input = arg_filen(NULL, "input", "<file>", 1, 1, "path to archive file");
    output = arg_filen(NULL, "extract", "<directory>", 0, 1, "extract to output directory");
    list = arg_litn(NULL, "list", 0, 1, "list archive contents");
    help = arg_litn(NULL, "help", 0, 1, "display this help and exit");
    version = arg_litn(NULL, "version", 0, 1, "display version info and exit");
    end = arg_end(2);
    
    argtable[0] = input;
    argtable[1] = output;
    argtable[2] = list;
    argtable[3] = help;
    argtable[4] = version;
    argtable[5] = end;
    
    nerrors = arg_parse(argc, argv, argtable);

    if (help->count > 0) {
        undat_print_arg_help(argtable, app_name);
        result = 0;
    } else if (version->count > 0) {
        undat_print_version(app_name, app_version);
        result = 0;
    } else {
        if (nerrors > 0) {
            undat_print_arg_errors(end, app_name);
        } else {
            yc_res_dat_directory_t *root = malloc(sizeof(*root));
            
            if (NULL == root) {
                fprintf(stderr, "Couldn't allocate memory for content tree.\n");
            } else {
                FILE* file = fopen(input->filename[0], "rb");
                
                if (NULL == file) {
                    fprintf(stderr, "Couldn't open file: %s.\n", strerror(errno));
                } else {
                    switch (yc_res_dat_tree(&undat_platform_file_reader, file, root)) {
                        case YC_RES_DAT_TREE_STATUS_FORMAT:
                            fprintf(stderr, "Provided file is corrupted or is not a Fallout™ .dat archive.\n");
                            break;
                        case YC_RES_DAT_TREE_STATUS_MALLOC:
                            fprintf(stderr, "Couldn't allocate memory when parsing file.\n");
                            break;
                        case YC_RES_DAT_TREE_STATUS_READ:
                            fprintf(stderr, "File read error occured: %s.\n", strerror(errno));
                            break;
                        case YC_RES_DAT_TREE_STATUS_INPUT:
                        case YC_RES_DAT_TREE_STATUS_INTERNAL: {
                            fprintf(stderr, "Internal error occured. Please, make a bug report.\n");
                            assert(0);
                            break;
                        }
                        case YC_RES_DAT_TREE_STATUS_OK: {
                            if (list->count > 0) {
                                undat_print_tree(root);
                            } else if (output->count > 0) {
                                switch (undat_filesystem_mkpath(*output->filename)) {
                                    case UNDAT_FILESYSTEM_MKPATH_ERROR:
                                        fprintf(stderr, "Couldn't create or access output directory.\n");
                                        break;
                                    case UNDAT_FILESYSTEM_MKPATH_OK: {
                                        switch (undat_extract_tree(root, file, output->filename)) {
                                            case UNDAT_EXTRACT_TREE_STATUS_MALLOC:
                                                fprintf(stderr, "Couldn't allocate memory.\n");
                                                break;
                                            case UNDAT_EXTRACT_TREE_STATUS_OPEN:
                                                fprintf(stderr, "Couldn't open file.\n");
                                                break;
                                            case UNDAT_EXTRACT_TREE_STATUS_WRITE:
                                                fprintf(stderr, "Couldn't write to file.\n");
                                                break;
                                            case UNDAT_EXTRACT_TREE_STATUS_CLOSE:
                                                fprintf(stderr, "Couldn't close file correctly.\n");
                                                break;
                                            case UNDAT_EXTRACT_TREE_STATUS_MKDIR:
                                                fprintf(stderr, "Couldn't create or access directory.\n");
                                                break;
                                            case UNDAT_EXTRACT_TREE_STATUS_READ:
                                                fprintf(stderr, "Couldn't read from input file, is it corrupted?.\n");
                                                break;
                                            case UNDAT_EXTRACT_TREE_STATUS_INTERNAL:
                                                fprintf(stderr, "Internal error occured. Please, make a bug report.\n");
                                                break;
                                            case UNDAT_EXTRACT_TREE_STATUS_OK: {
                                                result = 0;
                                                break;
                                            }
                                        }
                                        
                                        break;
                                    }
                                }
                            } else {
                                undat_print_arg_errors(end, app_name);
                            }
                            
                            if (0 != fclose(file)) {
                                fprintf(stderr, "Couldn't close file: %s.\n", strerror(errno));
                            }
                            
                            file = NULL;
                            
                            break;
                        }
                    }
                    
                    yc_res_dat_free_tree(root);
                }
                
                free(root);
                root = NULL;
            }
        }
    }
    
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    return result;
}
