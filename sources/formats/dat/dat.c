#include <libycres.h>
#include <private.h>

#include <stdlib.h>
#include <memory.h>
#include <assert.h>

yc_res_dat_tree_status_t
yc_res_dat_tree(yc_res_platform_reader_t* reader, void* input, yc_res_dat_directory_t* root) {
    unsigned long i;

    unsigned long read;
    unsigned long offset = 0;
    
    unsigned long count = 0;
    
    if (NULL == reader || NULL == input || NULL == root)
        return YC_RES_DAT_TREE_STATUS_INPUT;
    
    root->name = malloc(sizeof(*root->name) * 2);
    root->name_length = 1;

    if (NULL == root->name) {
        return YC_RES_DAT_TREE_STATUS_MALLOC;
    }
    
    root->name[0] = '.';
    root->name[1] = '\0';
    
    root->files_count = 0;
    root->directories_count = 0;
    
    root->files = NULL;
    root->directories = NULL;
    
    switch (yc_res_dat_private_load_count(reader, input, 0, &count, &read)) {
        case YC_RES_DAT_PRIVATE_LOAD_STATUS_OK: break;
        case YC_RES_DAT_PRIVATE_LOAD_STATUS_MALLOC: return YC_RES_DAT_TREE_STATUS_MALLOC;
        case YC_RES_DAT_PRIVATE_LOAD_STATUS_PLATFORM: return YC_RES_DAT_TREE_STATUS_READ;
        case YC_RES_DAT_PRIVATE_LOAD_STATUS_CORRUPTED: return YC_RES_DAT_TREE_STATUS_FORMAT;
    }
    
    offset += read;
    offset += 3 * 4;
    
    for (i = 0; i < count; ++i) {
        char* path;
        unsigned long path_size = 0;
        
        unsigned long token_start = 0;
        unsigned long token_end;
        
        unsigned long level = 0;
        yc_res_dat_directory_t* current = root;
        
        switch (yc_res_dat_private_load_string(reader, input, offset, &path, &path_size, &read)) {
            case YC_RES_DAT_PRIVATE_LOAD_STATUS_OK: break;
            case YC_RES_DAT_PRIVATE_LOAD_STATUS_MALLOC: return YC_RES_DAT_TREE_STATUS_MALLOC;
            case YC_RES_DAT_PRIVATE_LOAD_STATUS_PLATFORM: return YC_RES_DAT_TREE_STATUS_READ;
            case YC_RES_DAT_PRIVATE_LOAD_STATUS_CORRUPTED: return YC_RES_DAT_TREE_STATUS_FORMAT;
        }
        
        offset += read;
        path_size++;
        
        if (2 > path_size || (2 == path_size && (path[0] != '.')) || (2 < path_size && (path[0] != '.' && path[1] != '\\'))) {
            if (path_size < 1) {
                free(path);
                return YC_RES_DAT_TREE_STATUS_FORMAT;
            }
            
            path_size += 2;
            
            {
                char *new_path = malloc(path_size);
                
                if (NULL == new_path) {
                    free(path);
                    return YC_RES_DAT_TREE_STATUS_MALLOC;
                }
                
                memcpy(&new_path[2], path, path_size - 2);
                
                free(path);
                path = new_path;
            }
            
            path[0] = '.';
            path[1] = '\\';
            path[path_size - 1] = '\0';
        }
        
        for (token_end = 0; token_end < path_size - 1; ++token_end) {
            if (path[token_end + 1] == '\\' || path[token_end + 1] == '\0') {
                if (0 < level) {
                    yc_res_dat_directory_t* existed = NULL;
                    unsigned int token_length = token_end - token_start;
                    
                    unsigned int di;
                    for (di = 0; di < current->directories_count; ++di) {
                        char *name = current->directories[di].name;
                        
                        unsigned long name_length = current->directories[di].name_length;
                        unsigned long cmp_length = token_length < name_length ? token_length : name_length;
                        
                        if (0 == memcmp(&path[token_start + 1], name, cmp_length)) {
                            existed = &current->directories[di];
                        }
                    }
                    
                    if (NULL == existed) {
                        yc_res_dat_directory_t* new = NULL;
                        
                        if (0 == current->directories_count || NULL == current->directories) {
                            current->directories_count = 1;
                            current->directories = malloc(sizeof(*current->directories));
                        } else {
                            current->directories_count++;
                            current->directories = realloc(current->directories,
                                                           current->directories_count * sizeof(*current->directories));;
                        }
                        
                        if (NULL == current->directories) {
                            current->directories_count = 0;
                            free(path);
                            
                            return YC_RES_DAT_TREE_STATUS_MALLOC;
                        }
                        
                        new = &current->directories[current->directories_count - 1];
                        new->_marked = 0;
                        
                        new->files_count = 0;
                        new->directories_count = 0;
                        
                        new->files = NULL;
                        new->directories = NULL;
                        
                        new->name = malloc(token_length + 1);
                        new->name_length = token_length;

                        if (NULL == new->name) {
                            new->name_length = 0;
                            free(path);
                            
                            return YC_RES_DAT_TREE_STATUS_MALLOC;
                        }
                        
                        memcpy(new->name, &path[token_start + 1], token_length);
                        (new->name)[token_length] = '\0';
                        
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
        
        current->_marked = 1;
        
        free(path);
        path = NULL;
    }
    
    {
        unsigned long j;
        unsigned long appended = 0;
        
        yc_res_dat_directory_t** flat = malloc(sizeof(*flat) * count);
        
        if (NULL == flat)
            return YC_RES_DAT_TREE_STATUS_MALLOC;
        
        yc_res_dat_private_flatten_marked_dirs(root, flat, &appended);
        assert(appended == count);
        
        if (appended != count) {
            free(flat);
            return YC_RES_DAT_TREE_STATUS_INTERNAL;
        }
        
        for (i = 0; i < count; ++i) {
            yc_res_dat_directory_t* current = flat[i];
            assert(0 != current->_marked);
            
            if (0 == current->_marked) {
                free(flat);
                return YC_RES_DAT_TREE_STATUS_INTERNAL;
            }

            switch (yc_res_dat_private_load_count(reader, input, offset, &current->files_count, &read)) {
                case YC_RES_DAT_PRIVATE_LOAD_STATUS_OK: break;
                case YC_RES_DAT_PRIVATE_LOAD_STATUS_MALLOC: { free(flat); return YC_RES_DAT_TREE_STATUS_MALLOC; }
                case YC_RES_DAT_PRIVATE_LOAD_STATUS_PLATFORM: { free(flat); return YC_RES_DAT_TREE_STATUS_READ; }
                case YC_RES_DAT_PRIVATE_LOAD_STATUS_CORRUPTED: { free(flat); return YC_RES_DAT_TREE_STATUS_FORMAT; }
            }
            
            offset += read;
            offset += 3 * 4;

            current->files = malloc(current->files_count * sizeof(*current->files));

            if (NULL == current->files) {
                current->files_count = 0;
                free(flat);
                
                return YC_RES_DAT_TREE_STATUS_MALLOC;
            }
            
            for (j = 0; j < current->files_count; ++j) {
                unsigned long plain_size, packed_size;
                
                switch (yc_res_dat_private_load_string(reader, input, offset,
                                                       &current->files[j].name, &current->files[j].name_length, &read)) {
                    case YC_RES_DAT_PRIVATE_LOAD_STATUS_OK: break;
                    case YC_RES_DAT_PRIVATE_LOAD_STATUS_MALLOC: { free(flat); return YC_RES_DAT_TREE_STATUS_MALLOC; }
                    case YC_RES_DAT_PRIVATE_LOAD_STATUS_PLATFORM: { free(flat); return YC_RES_DAT_TREE_STATUS_READ; }
                    case YC_RES_DAT_PRIVATE_LOAD_STATUS_CORRUPTED: { free(flat); return YC_RES_DAT_TREE_STATUS_FORMAT; }
                }
                offset += read;
                offset += 4;
                
                if (0 == current->files[j].name_length) {
                    if (NULL != current->files[j].name) {
                        free(current->files[j].name);
                    }
                    
                    free(flat);
                    return YC_RES_DAT_TREE_STATUS_FORMAT;
                }

                switch (yc_res_dat_private_load_count(reader, input, offset, &current->files[j].start, &read)) {
                    case YC_RES_DAT_PRIVATE_LOAD_STATUS_OK: break;
                    case YC_RES_DAT_PRIVATE_LOAD_STATUS_MALLOC: { free(flat); return YC_RES_DAT_TREE_STATUS_MALLOC; }
                    case YC_RES_DAT_PRIVATE_LOAD_STATUS_PLATFORM: { free(flat); return YC_RES_DAT_TREE_STATUS_READ; }
                    case YC_RES_DAT_PRIVATE_LOAD_STATUS_CORRUPTED: { free(flat); return YC_RES_DAT_TREE_STATUS_FORMAT; }
                }
                offset += read;

                switch (yc_res_dat_private_load_count(reader, input, offset, &plain_size, &read)) {
                    case YC_RES_DAT_PRIVATE_LOAD_STATUS_OK: break;
                    case YC_RES_DAT_PRIVATE_LOAD_STATUS_MALLOC: { free(flat); return YC_RES_DAT_TREE_STATUS_MALLOC; }
                    case YC_RES_DAT_PRIVATE_LOAD_STATUS_PLATFORM: { free(flat); return YC_RES_DAT_TREE_STATUS_READ; }
                    case YC_RES_DAT_PRIVATE_LOAD_STATUS_CORRUPTED: { free(flat); return YC_RES_DAT_TREE_STATUS_FORMAT; }
                }
                offset += read;

                switch (yc_res_dat_private_load_count(reader, input, offset, &packed_size, &read)) {
                    case YC_RES_DAT_PRIVATE_LOAD_STATUS_OK: break;
                    case YC_RES_DAT_PRIVATE_LOAD_STATUS_MALLOC: { free(flat); return YC_RES_DAT_TREE_STATUS_MALLOC; }
                    case YC_RES_DAT_PRIVATE_LOAD_STATUS_PLATFORM: { free(flat); return YC_RES_DAT_TREE_STATUS_READ; }
                    case YC_RES_DAT_PRIVATE_LOAD_STATUS_CORRUPTED: { free(flat); return YC_RES_DAT_TREE_STATUS_FORMAT; }
                }
                offset += read;

                current->files[j].size = packed_size > 0 ? packed_size : plain_size;
                current->files[j].original_size = packed_size > 0 ? plain_size : 0;
            }
        }
        
        free(flat);
        flat = NULL;
    }
    
    return YC_RES_DAT_TREE_STATUS_OK;
}

yc_res_dat_extract_status_t
yc_res_dat_extract(yc_res_platform_reader_t* reader, void* input, yc_res_platform_writer_t* writer, void* output,
                   yc_res_dat_file_t* file) {
    unsigned char* bytes;
    
    if (NULL == reader || NULL == input || NULL == writer || NULL == output || NULL == file)
        return YC_RES_DAT_TREE_STATUS_INPUT;
    
    bytes = malloc(sizeof(*bytes) * file->size);
    
    if (NULL == bytes)
        return YC_RES_DAT_EXTRACT_STATUS_MALLOC;
    
    switch (reader(input, file->start, file->size, bytes)) {
        case YC_RES_DAT_PLATFORM_READ_STATUS_OK: break;
        case YC_RES_DAT_PLATFORM_READ_STATUS_ERROR: free(bytes); return YC_RES_DAT_EXTRACT_STATUS_READ;
    }
    
    if (0 < file->original_size) {
        unsigned char *decompressed = malloc(sizeof(*bytes) * file->original_size);
        
        if (NULL == decompressed) {
            free(bytes);
            return YC_RES_DAT_EXTRACT_STATUS_MALLOC;
        }
        
        {
            unsigned char* input = bytes;
            unsigned char* output = decompressed;
            
            unsigned char* input_end = input + file->size;
            unsigned char* output_end = output + file->original_size;

            while (input < input_end) {
                short next_bytes_count;
                
                next_bytes_count = *(input++) << 8;
                next_bytes_count |= *(input++);
                
                if (next_bytes_count == 0) break;

                if (next_bytes_count < 0) {
                    unsigned char* end = input - next_bytes_count;
                    while (input < end && output < output_end) {
                        unsigned char byte = *(input++);
                        *(output++) = byte;
                    }
                } else {
                    unsigned short buffer_size = 4096;
                    unsigned char buffer[4096];
                    unsigned short buffer_read = 0;
                    
                    unsigned char* end = input + next_bytes_count;
                    
                    buffer_read = buffer_size - 18;
                    memset(buffer, ' ', buffer_size);
                    
                    while (input < end) {
                        unsigned char flags = 0;
                        unsigned int i;
                        
                        flags = *(input++);
                        
                        for (i = 0; i != 8 && input < end; ++i) {
                            if ((flags & 1) != 0) {
                                unsigned char byte = *(input++);
                                *(output++) = byte;
                                
                                buffer[buffer_read] = byte;
                                buffer_read++;
                                
                                if (buffer_read >= buffer_size)
                                    buffer_read = 0;
                            } else {
                                unsigned short buffer_write = 0;
                                unsigned char match_length = 0;
                                int j;
                                
                                buffer_write = *(input++);
                                match_length = *(input++);
                                
                                buffer_write = buffer_write | ((0xF0 & match_length) << 4);
                                match_length &= 0x0F;

                                for (j = 0; j < match_length + 3; j++){
                                    unsigned char byte = buffer[buffer_write];
                                    
                                    *(output++) = byte;
                                    buffer[buffer_read] = byte;
                                    
                                    buffer_write++;
                                    buffer_read++;
                                    
                                    if (buffer_read >= buffer_size) buffer_read = 0;
                                    if (buffer_write >= buffer_size) buffer_write = 0;
                                }
                            }
                            
                            flags >>= 1;
                        }
                    }
                }
            }
        }
        
        switch (writer(decompressed, file->original_size, output)) {
            case YC_RES_DAT_PLATFORM_WRITE_STATUS_OK: break;
            case YC_RES_DAT_PLATFORM_WRITE_STATUS_ERROR: {
                free(decompressed);
                free(bytes);
                
                return YC_RES_DAT_EXTRACT_STATUS_WRITE;
            }
        }
        
        free(decompressed);
    } else {
        switch (writer(bytes, file->size, output)) {
            case YC_RES_DAT_PLATFORM_WRITE_STATUS_OK: break;
            case YC_RES_DAT_PLATFORM_WRITE_STATUS_ERROR: free(bytes); return YC_RES_DAT_EXTRACT_STATUS_WRITE;
        }
    }
    
    free(bytes);
    
    return YC_RES_DAT_EXTRACT_STATUS_OK;
}

void yc_res_dat_free_tree(yc_res_dat_directory_t* directory) {
    if (NULL == directory)
        return;
    
    if (NULL != directory->name) {
        free(directory->name);
        directory->name = NULL;
    }
    
    if (NULL != directory->files) {
        unsigned long i;
        for (i = 0; i < directory->files_count; ++i) {
            yc_res_dat_free_file(&directory->files[i]);
        }
        
        free(directory->files);
        directory->files = NULL;
    }
    
    if (NULL != directory->directories) {
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
