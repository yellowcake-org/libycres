#include <stdio.h>
#include <stdlib.h>

#include <libycres.h>

void file_read(const void* input, unsigned long offset, unsigned long length, unsigned char* output);

// TODO: Handle errors.
int main(__unused int argc, char *argv[]) {
    FILE* handle = fopen(argv[1], "rb");
        
    yc_res_dat_directory_t *root;
    yc_res_dat_tree(&file_read, handle, &root);
    
//    unsigned long i;
//    for (i = 0; i < dirs_count; ++i) {
//        unsigned int j;
//        for (j = 0; j < dirs[i].count; ++j) {
//            printf("%s\\", dirs[i].name);
//            printf("%s\n", dirs[i].files[j].name);
//        }
//
//        yc_res_dat_free_directory(&dirs[i]);
//    }
    
    yc_res_dat_free_tree(root);
    
    free(root);
    root = NULL;
    
    fclose(handle);
    handle = NULL;
    
    return 0;
}

// TODO: Handle errors.
void file_read(const void* input, unsigned long offset, unsigned long length, unsigned char* output) {
    FILE* handle = (FILE*)input;
    
    fseeko(handle, offset, SEEK_SET);
    fread(output, sizeof(typeof(*output)), length, handle);
}
