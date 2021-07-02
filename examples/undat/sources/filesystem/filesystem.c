#include <undat.h>

#include <sys/stat.h>
#include <limits.h>

#include <string.h>
#include <errno.h>

/* TODO: Adapt this for Windows too? */
undat_filesystem_mkpath_status_t undat_filesystem_mkpath(const char *path) {
    const size_t len = strlen(path);
    char _path[PATH_MAX];
    char *p;

    if (len > sizeof(_path) - 1) {
        return UNDAT_FILESYSTEM_MKPATH_ERROR;
    }
    
    strcpy(_path, path);

    for (p = _path + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';

            if (0 != mkdir(_path, S_IRWXU)) {
                if (errno != EEXIST)
                    return UNDAT_FILESYSTEM_MKPATH_ERROR;
            }

            *p = '/';
        }
    }

    if (0 != mkdir(_path, S_IRWXU)) {
        if (errno != EEXIST)
            return UNDAT_FILESYSTEM_MKPATH_ERROR;
    }

    return UNDAT_FILESYSTEM_MKPATH_OK;
}
