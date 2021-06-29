#include <stdio.h>
#include <stdlib.h>

#include <libycres.h>

void reader(const void* input, unsigned long offset, unsigned long length, unsigned char* output);

// TODO: Handle errors.
int main(__unused int argc, char *argv[]) {
    FILE* handle = fopen(argv[1], "rb");
    
    unsigned long dirs_count = 0;
    yc_res_dat_count(&reader, handle, &dirs_count);
    
    yc_res_dat_directory_t *dirs = malloc(dirs_count * sizeof(typeof(*dirs)));
    yc_res_dat_directories(&reader, handle, dirs_count, dirs);
    
    unsigned long i;
    for (i = 0; i < dirs_count; ++i) {
        unsigned int j;
        for (j = 0; j < dirs[i].count; ++j) {
            printf("%s\\", dirs[i].name);
            printf("%s\n", dirs[i].files[j].name);
        }
        
        yc_res_dat_directory_free(&dirs[i]);
    }
    
    free(dirs);
    dirs = NULL;
    
    fclose(handle);
    handle = NULL;
    
    return 0;
}

// TODO: Handle errors.
void reader(const void* input, unsigned long offset, unsigned long length, unsigned char* output) {
    FILE* handle = (FILE*)input;
    
    fseeko(handle, offset, SEEK_SET);
    fread(output, sizeof(typeof(*output)), length, handle);
}
