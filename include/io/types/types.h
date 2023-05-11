#ifndef LIB_YCRES_IO_TYPES_H
#define LIB_YCRES_IO_TYPES_H

#include <stddef.h>

typedef void *(yc_res_io_fopen_t)(const char *, const char *);
typedef int (yc_res_io_fclose_t)(void *);

typedef int (yc_res_io_fseek_t)(void *, long, int);
typedef size_t (yc_res_io_fread_t)(void *, size_t, size_t, void *);

typedef struct yc_res_io_fs_api {
    yc_res_io_fopen_t *fopen;
    yc_res_io_fclose_t *fclose;
    yc_res_io_fread_t *fread;
    yc_res_io_fseek_t *fseek;
} yc_res_io_fs_api_t;

#ifndef SEEK_SET
#define	SEEK_SET	0	/* set file offset to offset */
#endif
#ifndef SEEK_CUR
#define	SEEK_CUR	1	/* set file offset to current plus offset */
#endif
#ifndef SEEK_END
#define	SEEK_END	2	/* set file offset to EOF plus offset */
#endif

#endif //LIB_YCRES_IO_TYPES_H
