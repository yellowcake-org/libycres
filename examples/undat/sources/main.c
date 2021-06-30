#include <stdio.h>
#include <stdlib.h>

#include <libycres.h>

void file_read(void* input, unsigned long offset, unsigned long length, unsigned char* output);

void print_directory(yc_res_dat_directory_t* node, unsigned long level) {
    unsigned long l, f, d;
    
    for (l = 0; l < level; ++l) {
        if (1 == level - l) {
            printf("\\---");
        } else {
            printf("    ");
        }
    }
    
    printf("%s\n", node->name);
    
    for (f = 0; f < node->files_count; ++f) {
        for (l = 0; l < level; ++l) {
            printf("    ");
        }
        
        printf("\\---");
        printf("%s\n", node->files[f].name);
    }

    for (d = 0; d < node->directories_count; ++d) {
        print_directory(&node->directories[d], level + 1);
    }
}

int main(__unused int argc, char *argv[]) {
    FILE* handle = fopen(argv[1], "rb");
        
    yc_res_dat_directory_t *root;
    yc_res_dat_tree(&file_read, handle, &root);
    
    print_directory(root, 0);
    
    yc_res_dat_free_tree(root);
    
    free(root);
    root = NULL;
    
    fclose(handle);
    handle = NULL;
    
    return 0;
}

void file_read(void* input, unsigned long offset, unsigned long length, unsigned char* output) {
    FILE* handle = (FILE*)input;
    
    fseeko(handle, offset, SEEK_SET);
    fread(output, sizeof(*output), length, handle);
}
