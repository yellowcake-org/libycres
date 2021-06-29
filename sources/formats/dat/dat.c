#include <libycres.h>
#include <private.h>

#include <stdlib.h>
#include <memory.h>
#include <assert.h>

void yc_res_dat_tree(yc_res_platform_reader_t* reader, const void* input, yc_res_dat_directory_t** output) {
    if (NULL == reader || NULL == input || NULL == output)
        return; // TODO: Handle error.
    
    unsigned long i;
    unsigned long read;
    unsigned long offset = 0;
    
    unsigned long count = 0;
    
    yc_res_dat_private_load_count(reader, input, 0, &count, &read);
    offset += read;
    offset += 3 * 4; // skip attributes
    
    // tree of directories according to path's list
    *output = malloc(sizeof(**output));
    yc_res_dat_directory_t* root = *output;
    
    if (NULL == *output)
        return; // TODO: Handle errors.
    
    root->name = malloc(sizeof(*root->name) * 2);

    if (NULL == root->name) {
        free(root);
        return; // TODO: Handle errors.
    }

    root->name[0] = '.';
    root->name[1] = '\0';
    
    root->files_count = 0;
    root->directories_count = 0;
    
    for (i = 0; i < count; ++i) {
        char* path;
        unsigned long path_size = 0;
        
        yc_res_dat_private_load_string(reader, input, offset, &path, &read);
        offset += read;
        path_size += read;
        
        if (1 > path_size || path[0] != '.') {
            path = realloc(path, path_size + 2);
            
            if (NULL == path)
                return; // TODO: Handle errors.
            
            memmove(path + 2, path, path_size);
            path_size += 2;
            
            path[0] = '.';
            path[1] = '\\';
        }
        
        unsigned long token_start = 0;
        unsigned long token_end;
        
        yc_res_dat_directory_t* current = root;
        unsigned long level = 0;
        
        for (token_end = 0; token_end < path_size; ++token_end) {
            if (path[token_end + 1] == '\\' || path[token_end + 1] == '\0') {
                if (0 < level) {
                    yc_res_dat_directory_t* existed = NULL;
                    unsigned int token_length = token_end - token_start;
                    
                    unsigned int di;
                    for (di = 0; di < current->directories_count; ++di) {
                        if (0 == memcmp(&path[token_start + 1], current->directories[di].name, token_length)) {
                            existed = &current->directories[di];
                        }
                    }
                    
                    if (NULL == existed) {
                        // TODO: handle errors.
                        if (0 == current->directories_count || NULL == current->directories) {
                            current->directories_count = 1;
                            current->directories = malloc(sizeof(*current->directories));
                        } else {
                            current->directories_count++;
                            current->directories = realloc(current->directories,
                                                           current->directories_count *
                                                           sizeof(*current->directories));
                        }
                        
                        yc_res_dat_directory_t* new = &current->directories[current->directories_count - 1];
                        new->name = malloc(token_length + 1);

                        if (NULL == new->name) {
                            free(path); free(root);
                            return; // TODO: Handle errors.
                        }
                        
                        memcpy(new->name, &path[token_start + 1], token_length);
                        (new->name)[token_length] = '\0';
                        
                        new->files_count = 0;
                        new->directories_count = 0;
                        
                        new->has_content_block = 0;
                        
                        current = new;
                    } else {
                        current = existed;
                    }
                }
                
                level++;
                token_end++;
                token_start = token_end;
            }
        }
        
        current->has_content_block = 1;
        
        free(path);
        path = NULL;
    }
    
    // list of directories presented as is within dat file
    yc_res_dat_directory_t** flat = malloc(sizeof(*flat) * count);
    
    if (NULL == flat)
        return; //TODO: Handle errors.
    
    unsigned long appended = 0;
    yc_res_dat_private_flatten_marked_dirs(root, flat, &appended);
    
    assert(appended == count);
    
    for (i = 0; i < count; ++i) {
        yc_res_dat_directory_t* current = flat[i];
        
        assert(current->has_content_block == 1);

        yc_res_dat_private_load_count(reader, input, offset, &current->files_count, &read);
        offset += read;
        offset += 3 * 4; // skip attributes

        current->files = malloc(current->files_count * sizeof(*current->files));

        if (NULL == current->files) {
            free(flat);
            return; // TODO: Handle errors.
        }

        unsigned long j;
        for (j = 0; j < current->files_count; ++j) {
            yc_res_dat_private_load_string(reader, input, offset, &current->files[j].name, &read);
            offset += read;
            offset += 4; // skip attributes

            yc_res_dat_private_load_count(reader, input, offset, &current->files[j].start, &read);
            offset += read;

            unsigned long plain_size, packed_size;

            yc_res_dat_private_load_count(reader, input, offset, &plain_size, &read);
            offset += read;

            yc_res_dat_private_load_count(reader, input, offset, &packed_size, &read);
            offset += read;

            current->files[j].size = packed_size > 0 ? packed_size : plain_size;
            current->files[j].original_size = packed_size > 0 ? plain_size : 0;
        }
    }
    
    free(flat);
    flat = NULL;
}

void yc_res_dat_free_tree(yc_res_dat_directory_t* directory) {
    if (NULL == directory)
        return;
    
    if (NULL != directory->name) {
        free(directory->name);
        directory->name = NULL;
    }
    
    if (NULL != directory->files && 0 < directory->files_count) {
        unsigned long i;
        for (i = 0; i < directory->files_count; ++i) {
            yc_res_dat_free_file(&directory->files[i]);
        }
        
        free(directory->files);
        directory->files = NULL;
    }
    
    if (NULL != directory->directories && 0 < directory->directories_count) {
        unsigned long i;
        for (i = 0; i < directory->directories_count; ++i) {
            yc_res_dat_free_tree(&directory->directories[i]);
        }
        
        free(directory->directories);
        directory->directories = NULL;
    }
}

void yc_res_dat_free_file(yc_res_dat_file_t* file) {
    if (NULL == file)
        return;
    
    if (NULL != file->name){
        free(file->name);
        file->name = NULL;
    }
}
