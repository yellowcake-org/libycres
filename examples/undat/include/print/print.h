#ifndef UNDAT_PRINT_H
#define UNDAT_PRINT_H

#include <undat.h>

void undat_print_tree(yc_res_dat_directory_t* root);

void undat_print_arg_help(void* argtable, const char* appname);
void undat_print_arg_errors(struct arg_end* end, const char* appname);

void undat_print_version(const char* appname);

#endif /* UNDAT_PRINT_H */
