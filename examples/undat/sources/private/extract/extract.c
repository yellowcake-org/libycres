#include <private/extract.h>

#include <stdlib.h>
#include <memory.h>

undat_iterate_handler_result_t
undat_private_extract_node(yc_res_dat_directory_t* node, void* accum, __unused unsigned long level) {
    undat_iterate_handler_result_t result;
    undat_private_extract_iterate_accum_t* accumulator = (undat_private_extract_iterate_accum_t*)accum;
    unsigned long f;
    char d = '/';
    
    if (NULL == accumulator->current) {
        accumulator->current = malloc(sizeof(*accumulator->current) * (node->name_length + 1));
        
        if (NULL == accumulator->current) {
            result.error = UNDAT_PRIVATE_EXTRACT_NODE_ERROR_MALLOC;
            result.status = UNDAT_ITERATE_HANDLER_STATUS_ERROR;
            
            return result;
        }
        
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
            
            if (NULL == accumulator->current) {
                result.error = UNDAT_PRIVATE_EXTRACT_NODE_ERROR_MALLOC;
                result.status = UNDAT_ITERATE_HANDLER_STATUS_ERROR;
                
                return result;
            }
            
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
    
    result.error = NULL;
    result.status = UNDAT_ITERATE_HANDLER_STATUS_OK;
    
    return result;
}
