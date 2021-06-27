#ifndef PLATFORM_H
#define PLATFORM_H

/// Provides pointer to the first of 'length' bytes from 'input'.
typedef unsigned char* (yc_res_platform_reader_t)(const void* input, const unsigned long* offset, const unsigned long* length);

#endif /* PLATFORM_H */
