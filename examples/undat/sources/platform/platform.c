#include <undat.h>
#include <stdio.h>

void undat_platform_file_reader(void* input, unsigned long offset, unsigned long length, unsigned char* output) {
    FILE* handle = (FILE*)input;
    
    fseeko(handle, offset, SEEK_SET);
    fread(output, sizeof(*output), length, handle);
}
