#include <undat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void undat_print_node(yc_res_dat_directory_t* node, __unused void** accum, unsigned long level) {
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

void undat_print_node_path(yc_res_dat_directory_t* node, void** accum, __unused unsigned long level) {
    char* path = (char*)(*accum);
    unsigned long f;
    char d = '/';
    
    if (NULL == path) {
        path = malloc(sizeof(*path) * (node->name_length + 1));
        memcpy(path, node->name, node->name_length);
        memset(&path[node->name_length], '\0', 1);
    } else {
        unsigned long len = 0;
        unsigned long initial_ds = level;
        unsigned long free_ds, si, path_level;
        unsigned long s_found = 0;
        
        free_ds = initial_ds;
        path_level = 0;
        for (si = 0; path[si] != '\0'; ++si) {
            len++;
            if (d == path[si]) {
                path_level++;
                
                if (free_ds > 0) {
                    free_ds--;
                    s_found = si;
                }
            }
        }
        
        {
            long level_diff = level - path_level;
            long ds_diff = initial_ds - free_ds;
                            
            if (ds_diff > level_diff && free_ds == 0) {
                len -= len - s_found;
            }
        }
        
        {
            unsigned long new_len = len + 1 + node->name_length;
            
            path = realloc(path, sizeof(*path) * (new_len + 1));
            memset(&path[len], d, 1);
            memcpy(&path[len + 1], node->name, node->name_length);
            memset(&path[new_len], '\0', 1);
        }
    }
    
    *accum = path;
    
    for (f = 0; f < node->files_count; ++f) {
        printf("%s", path);
        printf("/");
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

void undat_print_version(const char* appname) {
    printf("%s v0.3.0\n", appname);
}
