#include <stdio.h>
#include <stdlib.h>

#include <libycres.h>

void reader(const void* input, unsigned long offset, unsigned long length, unsigned char* output);

// TODO: Handle errors.
int main(__unused int argc, char *argv[]) {
    unsigned long dirs_count = 0;
    
    FILE* handle = fopen(argv[1], "rb");
    yc_res_dat_count(&reader, handle, &dirs_count);
    
    printf("Dirs count: %zu.\n", dirs_count);    
    return 0;
}

// TODO: Handle errors.
void reader(const void* input, unsigned long offset, unsigned long length, unsigned char* output) {
    FILE* handle = (FILE*)input;
    
    fseeko(handle, offset, SEEK_SET);
    fread(output, sizeof(unsigned char), length, handle);
}
