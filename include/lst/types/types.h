#ifndef INCLUDE_LST_TYPES_H
#define INCLUDE_LST_TYPES_H

bool yc_res_lst_is_valid_index(uint32_t fallback_idx);

typedef struct yc_res_lst_entry {
    uint32_t index;
    char *value;
} yc_res_lst_entry_t;

typedef struct yc_res_lst_entries {
    size_t count;
    yc_res_lst_entry_t *pointers;
} yc_res_lst_entries_t;

#endif //INCLUDE_LST_TYPES_H
