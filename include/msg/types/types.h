#ifndef LIB_YCRES_MSG_TYPES_H
#define LIB_YCRES_MSG_TYPES_H

bool yc_res_msg_is_valid_index(uint32_t index);

typedef struct yc_res_msg_entry {
    uint32_t index;
    char *audio, *description;
} yc_res_msg_entry_t;

typedef struct yc_res_msg_entries {
    size_t count;
    yc_res_msg_entry_t *pointers;
} yc_res_msg_entries_t;

#endif //LIB_YCRES_MSG_TYPES_H
