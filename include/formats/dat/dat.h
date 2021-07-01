#ifndef LIBYCRES_DAT_H
#define LIBYCRES_DAT_H

#include <libycres.h>

typedef struct yc_res_dat_file yc_res_dat_file_t;
typedef struct yc_res_dat_directory yc_res_dat_directory_t;

struct yc_res_dat_file {
    char* name;
    unsigned long name_length;
    
    unsigned long start;
    unsigned long size;
    
    unsigned long original_size;
};

struct yc_res_dat_directory {
    char* name;
    unsigned long name_length;
    
    unsigned long files_count;
    yc_res_dat_file_t *files;
    
    unsigned long directories_count;
    yc_res_dat_directory_t *directories;
    
    unsigned char _marked;
};

void yc_res_dat_tree(yc_res_platform_reader_t* reader, void* input, yc_res_dat_directory_t* root);

void yc_res_dat_free_tree(yc_res_dat_directory_t* root);
void yc_res_dat_free_file(yc_res_dat_file_t* file);

#endif /* LIBYCRES_DAT_H */
