#include <stdio.h>
#include <stdlib.h>

#include <libycres.h>

void file_read(const void* input, unsigned long offset, unsigned long length, unsigned char* output);

void print_directory(yc_res_dat_directory_t* node) {
    unsigned long i;
    for (i = 0; i < node->directories_count; ++i) {
//        unsigned int j;
//        for (j = 0; j < node->files_count; ++j) {
//            printf("%s\\", node->name);
//            printf("%s\n", node->files[j].name);
//        }

        printf("%s\\", node->name);
        print_directory(&node->directories[i]);
    }
    
    printf("\n");
}

// TODO: Handle errors.
int main(__unused int argc, char *argv[]) {
    FILE* handle = fopen(argv[1], "rb");
        
    yc_res_dat_directory_t *root;
    yc_res_dat_tree(&file_read, handle, &root);
    
    print_directory(root);
    
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
    fread(output, sizeof(*output), length, handle);
}
