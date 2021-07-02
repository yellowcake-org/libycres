#include <undat.h>

#include <stdlib.h>
#include <memory.h>

typedef struct {
    char *current;
    const char *output;
} undat_extract_iterate_accum_t;

void undat_extract_node(yc_res_dat_directory_t* node, void* accum, unsigned long level);
void undat_extract_tree(yc_res_dat_directory_t* root, const char** path) {
    undat_extract_iterate_accum_t* accumulator = malloc(sizeof(*accumulator));
    accumulator->output = *path;
    accumulator->current = NULL;

    undat_iterate_tree(root, (void*)accumulator, 0, &undat_extract_node);
    
    free(accumulator->current);
    free(accumulator);
}

void undat_extract_node(yc_res_dat_directory_t* node, void* accum, __unused unsigned long level) {
    undat_extract_iterate_accum_t* accumulator = (undat_extract_iterate_accum_t*)accum;
    unsigned long f;
    char d = '/';
    
    if (NULL == accumulator->current) {
        accumulator->current = malloc(sizeof(*accumulator->current) * (node->name_length + 1));
        memcpy(accumulator->current, node->name, node->name_length);
        memset(&accumulator->current[node->name_length], '\0', 1);
    } else {
        unsigned long len = 0;
        unsigned long initial_ds = level;
        unsigned long free_ds, si, path_level;
        unsigned long s_found = 0;
        
        free_ds = initial_ds;
        path_level = 0;
        for (si = 0; accumulator->current[si] != '\0'; ++si) {
            len++;
            if (d == accumulator->current[si]) {
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
            
            accumulator->current = realloc(accumulator->current, sizeof(*accumulator->current) * (new_len + 1));
            memset(&accumulator->current[len], d, 1);
            memcpy(&accumulator->current[len + 1], node->name, node->name_length);
            memset(&accumulator->current[new_len], '\0', 1);
        }
    }
    
    for (f = 0; f < node->files_count; ++f) {
        printf("%s", accumulator->output);
        
        printf("/");
        printf("%s", accumulator->current);
        
        printf("/");
        printf("%s\n", node->files[f].name);
    }
}
