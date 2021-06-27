#include "formats/dat/dat.h"
#include <stddef.h>

void yc_res_dat_count(yc_res_platform_reader_t* reader, const void* input, unsigned long* output) {
    if (NULL == reader || NULL == input || NULL == output)
        return; // TODO: Handle error.
    
    // read first 4 bytes
    unsigned char* slice = *reader(input, 0, 4);
    
    if (NULL == slice)
        return; // TODO: Handle error.
    
    // assemble 32-bit unsigned integer in big-endian
    *output = (slice[0] << 24) + (slice[1] << 16) + (slice[2] << 8) + slice[3];
}
