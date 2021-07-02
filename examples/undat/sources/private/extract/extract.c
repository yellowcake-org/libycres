#include <private/extract.h>

#include <stdlib.h>
#include <memory.h>

undat_iterate_handler_result_t
undat_private_extract_node(yc_res_dat_directory_t* node, void* accum, __unused unsigned long level) {
    undat_iterate_handler_result_t result;
    undat_private_extract_node_accum_t* accumulator = (undat_private_extract_node_accum_t*)accum;

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
        unsigned long length = 0;
        unsigned long slash_index = 0, si = 0;
        
        unsigned long slash_count = level;
        unsigned long remained_slash_count, path_level;
        
        remained_slash_count = slash_count;
        path_level = 0;
        
        for (si = 0; accumulator->current[si] != '\0'; ++si) {
            length++;
            
            if (d == accumulator->current[si]) {
                path_level++;
                
                if (remained_slash_count > 0) {
                    remained_slash_count--;
                    slash_index = si;
                }
            }
        }
        
        {
            long path_level_difference = level - path_level;
            long extra_slashes_count = slash_count - remained_slash_count;
                            
            if (0 == remained_slash_count && extra_slashes_count > path_level_difference) {
                length -= length - slash_index;
            }
        }
        
        {
            char *new_current = NULL;
            unsigned long new_len = length + 1 + node->name_length;
            
            new_current = realloc(accumulator->current, sizeof(*accumulator->current) * (new_len + 1));
            
            if (NULL == new_current) {
                result.error = UNDAT_PRIVATE_EXTRACT_NODE_ERROR_MALLOC;
                result.status = UNDAT_ITERATE_HANDLER_STATUS_ERROR;
                
                return result;
            }
            
            accumulator->current = new_current;
            
            memset(&accumulator->current[length], d, 1);
            memcpy(&accumulator->current[length + 1], node->name, node->name_length);
            memset(&accumulator->current[new_len], '\0', 1);
        }
    }
    
    for (f = 0; f < node->files_count; ++f) {
        unsigned long length = strlen(accumulator->output) + 1 + strlen(accumulator->current) + 1 + node->files[f].name_length;
        char* filename = malloc(sizeof(*filename) * length + 1);
        
        if (NULL == filename) {
            free(accumulator->current);
            
            result.error = UNDAT_PRIVATE_EXTRACT_NODE_ERROR_MALLOC;
            result.status = UNDAT_ITERATE_HANDLER_STATUS_ERROR;
            
            return result;
        }
        
        memcpy(filename, accumulator->output, strlen(accumulator->output));
        memset(&filename[strlen(accumulator->output)], '/', 1);
        memcpy(&filename[strlen(accumulator->output) + 1], accumulator->current, strlen(accumulator->current));
        memset(&filename[strlen(accumulator->output) + 1 + strlen(accumulator->current)], '/', 1);
        memcpy(&filename[strlen(accumulator->output) + 1 + strlen(accumulator->current) + 1],
               node->files[f].name, node->files[f].name_length);
        memset(&filename[length], '\0', 1);

        printf("%s\n", filename);
        
        {
            FILE* file = fopen(filename, "wb");
            free(filename);
            
            if (NULL == file) {
                result.error = UNDAT_PRIVATE_EXTRACT_NODE_ERROR_OPEN;
                result.status = UNDAT_ITERATE_HANDLER_STATUS_ERROR;
                
                return result;
            } else {
                /* TODO: Handle errors */
                yc_res_dat_extract(&undat_platform_file_reader, accumulator->input,
                                   &undat_platform_file_writer, file, &node->files[f]);
                
                if (0 != fclose(file)) {
                    result.error = UNDAT_PRIVATE_EXTRACT_NODE_ERROR_CLOSE;
                    result.status = UNDAT_ITERATE_HANDLER_STATUS_ERROR;
                    
                    return result;
                }
            }
        }
    }
    
    result.error = NULL;
    result.status = UNDAT_ITERATE_HANDLER_STATUS_OK;
    
    return result;
}
