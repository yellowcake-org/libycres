#ifndef DAT_H
#define DAT_H

#include "../../../include/platform/platform.h"

typedef struct yc_res_dat_file yc_res_dat_file_t;
typedef struct yc_res_dat_directory yc_res_dat_directory_t;

struct yc_res_dat_file {
    char* name;
    
    unsigned long start;
    unsigned long size;
    
    unsigned long original_size;
};

struct yc_res_dat_directory {
    char* name;
    unsigned char has_content_block;
    
    unsigned long files_count;
    yc_res_dat_file_t *files;
    
    unsigned long directories_count;
    yc_res_dat_directory_t *directories;
};

void yc_res_dat_tree(yc_res_platform_reader_t* reader, void* input, yc_res_dat_directory_t** root);

void yc_res_dat_free_tree(yc_res_dat_directory_t* root);
void yc_res_dat_free_file(yc_res_dat_file_t* file);

#endif /* DAT_H */
