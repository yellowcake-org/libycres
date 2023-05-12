#ifndef SOURCE_FORMATS_DAT_EXTRACT_H
#define SOURCE_FORMATS_DAT_EXTRACT_H

typedef struct yc_res_dat_extract_counters {
    size_t *processed_ptr, *written_ptr;
} yc_res_dat_extract_counters_t;

typedef struct yc_res_dat_extract_buffer {
    size_t size;
    unsigned char *bytes;

    size_t match_min;
    uint16_t *offset_ptr;
} yc_res_dat_extract_buffer_t;

yc_res_dat_status_t yc_res_dat_read_plain_chunk(
        void *archive,
        const yc_res_io_fs_api_t *api,
        size_t chunk_size, bool whole,
        yc_res_dat_extract_api_t *result
);

yc_res_dat_status_t yc_res_dat_extract_file(
        void *archive,
        const yc_res_io_fs_api_t *api,
        yc_res_dat_file_t *file,
        yc_res_dat_extract_api_t *result
);

yc_res_dat_status_t yc_res_dat_decompress_chunk(
        void *archive,
        const yc_res_io_fs_api_t *api,
        size_t size,
        yc_res_dat_extract_api_t *result,
        yc_res_dat_extract_counters_t counters
);

yc_res_dat_status_t yc_res_extract_block_unbuffered(
        void *archive,
        const yc_res_io_fs_api_t *api,
        yc_res_dat_extract_api_t *result,
        yc_res_dat_extract_counters_t counters,
        yc_res_dat_extract_buffer_t buffer
);

yc_res_dat_status_t yc_res_extract_block_buffered(
        void *archive,
        const yc_res_io_fs_api_t *api,
        yc_res_dat_extract_api_t *result,
        yc_res_dat_extract_counters_t counters,
        yc_res_dat_extract_buffer_t buffer
);

#endif //SOURCE_FORMATS_DAT_EXTRACT_H
