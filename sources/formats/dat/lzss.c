#include <private.h>
#include <memory.h>

void yc_res_dat_lzss_decompress(unsigned char* input, unsigned long packed_size,
                                unsigned char* output, unsigned long plain_size) {
    unsigned char* input_end = input + packed_size;
    unsigned char* output_end = output + plain_size;

    while (input < input_end) {
        short next_bytes_count;
        
        next_bytes_count = *(input++) << 8;
        next_bytes_count |= *(input++);
        
        if (next_bytes_count == 0)
            break;

        if (next_bytes_count < 0) {
            unsigned char* end = input - next_bytes_count;
            while (input < end && output < output_end) {
                unsigned char byte = *(input++);
                *(output++) = byte;
            }
        } else {
            unsigned short buffer_size = 4096;
            unsigned char buffer[4096];
            unsigned short buffer_read = 0;
            
            unsigned char* end = input + next_bytes_count;
            
            buffer_read = buffer_size - 18;
            memset(buffer, ' ', buffer_size);
            
            while (input < end) {
                unsigned char flags = 0;
                unsigned int i;
                
                flags = *(input++);
                
                for (i = 0; i != 8 && input < end; ++i) {
                    if ((flags & 1) != 0) {
                        unsigned char byte = *(input++);
                        *(output++) = byte;
                        
                        buffer[buffer_read] = byte;
                        buffer_read++;
                        
                        if (buffer_read >= buffer_size)
                            buffer_read = 0;
                    } else {
                        unsigned short buffer_write = 0;
                        unsigned char match_length = 0;
                        int j;
                        
                        buffer_write = *(input++);
                        match_length = *(input++);
                        
                        buffer_write = buffer_write | ((0xF0 & match_length) << 4);
                        match_length &= 0x0F;

                        for (j = 0; j < match_length + 3; j++){
                            unsigned char byte = buffer[buffer_write];
                            
                            *(output++) = byte;
                            buffer[buffer_read] = byte;
                            
                            buffer_write++;
                            buffer_read++;
                            
                            if (buffer_read >= buffer_size) buffer_read = 0;
                            if (buffer_write >= buffer_size) buffer_write = 0;
                        }
                    }
                    
                    flags >>= 1;
                }
            }
        }
    }
}
