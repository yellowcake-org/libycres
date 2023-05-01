#include <private.h>

int16_t yc_res_byteorder_int16(int16_t input) {
    return (int16_t) (((((int32_t) input) >> 8) & 0xff) | ((((int32_t) input) & 0xff) << 8));
}

int32_t yc_res_byteorder_int32(int32_t input) {
    int64_t result = (input << 8 & 0xFF00FF00) | (input >> 8 & 0xFF00FF);
    return (int32_t) ((result << 16) | ((result >> 16) & 0xFFFF));
}

uint16_t yc_res_byteorder_uint16(uint16_t input) {
    return (input >> 8) | (input << 8);
}

uint32_t yc_res_byteorder_uint32(uint32_t input) {
    return ((input >> 24) & 0xff) |
           ((input << 8) & 0xff0000) |
           ((input >> 8) & 0xff00) |
           ((input << 24) & 0xff000000);
}
