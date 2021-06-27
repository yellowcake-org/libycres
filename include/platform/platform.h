#ifndef PLATFORM_H
#define PLATFORM_H

/// Provides pointer to the first of 'length' bytes from 'input'.
typedef void (yc_res_platform_reader_t)(const void* input,
                                        unsigned long offset, unsigned long length,
                                        unsigned char* output);

#endif /* PLATFORM_H */
