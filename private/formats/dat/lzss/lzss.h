#ifndef LIBYCRES_PRIVATE_DAT_LZSS_H
#define LIBYCRES_PRIVATE_DAT_LZSS_H

void yc_res_dat_lzss_decompress(unsigned char* input, unsigned long packed_size,
                                unsigned char* output, unsigned long plain_size);

#endif /* LIBYCRES_PRIVATE_DAT_LZSS_H */
