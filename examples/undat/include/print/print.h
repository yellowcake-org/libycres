#ifndef UNDAT_PRINT_H
#define UNDAT_PRINT_H

#include <undat.h>

void undat_print_tree(yc_res_dat_directory_t* root);

void undat_print_arg_help(void* argtable, const char* app_name);
void undat_print_arg_errors(struct arg_end* end, const char* app_name);

void undat_print_version(const char* app_name, const char *app_version);

#endif /* UNDAT_PRINT_H */
