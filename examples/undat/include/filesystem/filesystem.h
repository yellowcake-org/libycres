#ifndef UNDAT_FILESYSTEM_H
#define UNDAT_FILESYSTEM_H

typedef enum {
    UNDAT_FILESYSTEM_MKPATH_OK,
    UNDAT_FILESYSTEM_MKPATH_ERROR
} undat_filesystem_mkpath_status_t;

undat_filesystem_mkpath_status_t undat_filesystem_mkpath(const char *path);

#endif /* UNDAT_FILESYSTEM_H */
