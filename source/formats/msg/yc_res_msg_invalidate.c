#include <libycres.h>
#include <stdlib.h>

void yc_res_msg_invalidate(yc_res_msg_entry_t *entry) {
    if (NULL != entry->description) {
        free(entry->description);
        entry->description = NULL;
    }

    if (NULL != entry->description) {
        free(entry->description);
        entry->description = NULL;
    }
}
