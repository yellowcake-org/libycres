#include <private.h>

uint16_t yc_res_byteorder_uint16(uint16_t input) {
    return (input >> 8) | (input << 8);
}

uint32_t yc_res_byteorder_uint32(u_int32_t input) {
    return ((input >> 24) & 0xff) |
           ((input << 8) & 0xff0000) |
           ((input >> 8) & 0xff00) |
           ((input << 24) & 0xff000000);
}
