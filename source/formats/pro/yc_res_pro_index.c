#include <libycres.h>

uint16_t yc_res_index_from_any_id(uint32_t any_id) {
    return any_id & 0x00FFFFFF;
}
