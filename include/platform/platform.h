#ifndef LIBYCRES_PLATFORM_H
#define LIBYCRES_PLATFORM_H

typedef void yc_res_platform_reader_t(void* input,
                                      unsigned long offset, unsigned long length,
                                      unsigned char* output);

#endif /* LIBYCRES_PLATFORM_H */
