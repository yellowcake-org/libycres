#ifndef LIBYCRES_INCLUDE_LST_METHODS_H
#define LIBYCRES_INCLUDE_LST_METHODS_H

yc_res_lst_status_t yc_res_lst_parse(const char *filename, const yc_res_io_fs_api_t *api, yc_res_lst_entries_t *result);

void yc_res_lst_invalidate(yc_res_lst_entry_t *entry);

#endif //LIBYCRES_INCLUDE_LST_METHODS_H
