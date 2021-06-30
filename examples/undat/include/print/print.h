#ifndef UNDAT_PRINT_H
#define UNDAT_PRINT_H

#include <undat.h>

void undat_print_node(yc_res_dat_directory_t* node, unsigned long level);
void undat_print_arg_errors(struct arg_end* end, char* name);

#endif /* UNDAT_PRINT_H */
