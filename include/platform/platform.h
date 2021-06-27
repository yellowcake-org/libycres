#ifndef PLATFORM_H
#define PLATFORM_H

#include <stddef.h>

/// Provides pointer to the first of 'length' bytes from 'input'.
typedef unsigned char* (yc_res_platform_reader_t)(const void* input, const size_t* offset, const size_t* length);

#endif /* PLATFORM_H */
