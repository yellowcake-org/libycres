#ifndef UNDAT_ITERATE_H
#define UNDAT_ITERATE_H

#include <undat.h>

typedef void undat_iterate_handler_t(yc_res_dat_directory_t* node, void** accum, unsigned long level);
void undat_iterate_tree(yc_res_dat_directory_t* node, void** accum, unsigned long level, undat_iterate_handler_t* handler);

#endif /* UNDAT_ITERATE_H */
