#include <libycres.h>
#include <stdlib.h>

void yc_res_lst_invalidate(yc_res_lst_entry_t *entry) {
    if (NULL != entry->value) {
        free(entry->value);
        entry->value = NULL;
    }
}
