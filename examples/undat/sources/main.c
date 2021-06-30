#include <undat.h>

#include <stdio.h>
#include <stdlib.h>

int main(__unused int argc, char *argv[]) {
    FILE* handle = fopen(argv[1], "rb");
        
    yc_res_dat_directory_t *root;
    yc_res_dat_tree(&undat_platform_file_reader, handle, &root);
    
    undat_iterate_tree(root, 0, &undat_print_node);
    
    yc_res_dat_free_tree(root);
    
    free(root);
    root = NULL;
    
    fclose(handle);
    handle = NULL;
    
    return 0;
}
