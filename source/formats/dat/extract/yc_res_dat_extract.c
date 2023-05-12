#include "libycres.h"
#include "private.h"

#include <stdlib.h>
#include <memory.h>

#include "yc_res_dat_extract.h"

void yc_res_dat_extract_cleanup(void *archive, const yc_res_io_fs_api_t *api, unsigned char *bytes);

yc_res_dat_status_t yc_res_dat_extract(
        const char *filename,
        const yc_res_io_fs_api_t *api,
        yc_res_dat_file_t *file,
        yc_res_dat_extract_api_t *result
) {
    void *archive = api->fopen(filename, "rb");

    if (NULL == archive) {
        yc_res_dat_extract_cleanup(archive, api, NULL);
        return YC_RES_DAT_STATUS_IO;
    }

    if (0 != api->fseek(archive, file->offset, SEEK_SET)) {
        yc_res_dat_extract_cleanup(archive, api, NULL);
        return YC_RES_DAT_STATUS_IO;
    }

    yc_res_dat_status_t status =
            (false == file->is_compressed) ?
            yc_res_dat_read_plain_chunk(archive, api, file->count_plain, false, result) :
            yc_res_dat_extract_file(archive, api, file, result);

    if (YC_RES_DAT_STATUS_OK != status) { return status; }

    yc_res_dat_extract_cleanup(archive, api, NULL);
    return YC_RES_DAT_STATUS_OK;
}

yc_res_dat_status_t yc_res_dat_read_plain_chunk(
        void *archive,
        const yc_res_io_fs_api_t *api,
        size_t chunk_size, bool whole,
        yc_res_dat_extract_api_t *result
) {
    if (chunk_size == 0) { return YC_RES_DAT_STATUS_OK; }

    size_t read_size = whole ? chunk_size : 1;
    size_t sections = whole ? 1 : chunk_size;

    for (size_t cursor = 0; cursor < sections; cursor++) {
        unsigned char *bytes = malloc(read_size);

        if (NULL == bytes) {
            yc_res_dat_extract_cleanup(archive, api, NULL);
            return YC_RES_DAT_STATUS_MEM;
        }

        if (0 == api->fread(bytes, read_size, 1, archive)) {
            yc_res_dat_extract_cleanup(archive, api, bytes);
            return YC_RES_DAT_STATUS_IO;
        }

        result->callback(bytes, read_size, result->context);
    }

    return YC_RES_DAT_STATUS_OK;
}

yc_res_dat_status_t yc_res_dat_extract_file(
        void *archive,
        const yc_res_io_fs_api_t *api,
        yc_res_dat_file_t *file,
        yc_res_dat_extract_api_t *result
) {
    size_t processed = 0, written = 0;

    while (processed < file->count_compressed) {
        int16_t count = 0;

        processed += 2;
        if (0 == api->fread(&count, 2, 1, archive)) {
            yc_res_dat_extract_cleanup(archive, api, NULL);
            return YC_RES_DAT_STATUS_IO;
        }

        count = yc_res_byteorder_int16(count);
        if (count == 0) { break; }

        if (count < 0) {
            size_t end = -count;
            size_t limit = file->count_plain - written;
            size_t chunk_size = (limit < end ? limit : end);

            processed += chunk_size;
            written += chunk_size;

            yc_res_dat_status_t status = yc_res_dat_read_plain_chunk(archive, api, chunk_size, true, result);
            if (YC_RES_DAT_STATUS_OK != status) { return status; }
        } else {
            yc_res_dat_extract_counters_t counters = {.processed_ptr = &processed, .written_ptr = &written};
            yc_res_dat_status_t status = yc_res_dat_decompress_chunk(archive, api, count, result, counters);

            if (YC_RES_DAT_STATUS_OK != status) { return status; }
        }
    }

    if (written != file->count_plain) {
        yc_res_dat_extract_cleanup(archive, api, NULL);
        return YC_RES_DAT_STATUS_CORR;
    }

    return YC_RES_DAT_STATUS_OK;
}

yc_res_dat_status_t yc_res_dat_decompress_chunk(
        void *archive,
        const yc_res_io_fs_api_t *api,
        size_t size,
        yc_res_dat_extract_api_t *result,
        yc_res_dat_extract_counters_t counters
) {
    const size_t window = 4096;

    const size_t match_min = 3;
    const size_t match_max = 18;

    unsigned char bytes[window];
    memset(bytes, 0x20, window);

    uint16_t offset_r = window - match_max;
    yc_res_dat_extract_buffer_t buffer =
            {.size = window, .bytes = bytes, .match_min = match_min, .offset_ptr = &offset_r};

    size_t end = (*counters.processed_ptr) + size;
    while ((*counters.processed_ptr) < end) {
        unsigned char flags = 0;

        (*counters.processed_ptr)++;
        if (0 == api->fread(&flags, 1, 1, archive)) {
            yc_res_dat_extract_cleanup(archive, api, NULL);
            return YC_RES_DAT_STATUS_IO;
        }

        for (unsigned int i = 0; i < 8 && (*counters.processed_ptr) < end; i++) {
            yc_res_dat_status_t status =
                    (flags & 1) == 0 ?
                    yc_res_extract_block_buffered(archive, api, result, counters, buffer) :
                    yc_res_extract_block_unbuffered(archive, api, result, counters, buffer);

            if (YC_RES_DAT_STATUS_OK != status) { return status; }
            flags >>= 1;
        }
    }

    return YC_RES_DAT_STATUS_OK;
}

yc_res_dat_status_t yc_res_extract_block_unbuffered(
        void *archive,
        const yc_res_io_fs_api_t *api,
        yc_res_dat_extract_api_t *result,
        yc_res_dat_extract_counters_t counters,
        yc_res_dat_extract_buffer_t buffer
) {
    (*counters.processed_ptr)++;
    if (0 == api->fread(&buffer.bytes[(*buffer.offset_ptr)], 1, 1, archive)) {
        yc_res_dat_extract_cleanup(archive, api, NULL);
        return YC_RES_DAT_STATUS_IO;
    }

    unsigned char *byte = malloc(sizeof(unsigned char));

    if (NULL == byte) {
        yc_res_dat_extract_cleanup(archive, api, NULL);
        return YC_RES_DAT_STATUS_MEM;
    }

    *byte = buffer.bytes[(*buffer.offset_ptr)++];
    result->callback(byte, 1, result->context);
    (*counters.written_ptr)++;

    if ((*buffer.offset_ptr) >= buffer.size) { (*buffer.offset_ptr) = 0; }
    return YC_RES_DAT_STATUS_OK;
}

yc_res_dat_status_t yc_res_extract_block_buffered(
        void *archive,
        const yc_res_io_fs_api_t *api,
        yc_res_dat_extract_api_t *result,
        yc_res_dat_extract_counters_t counters,
        yc_res_dat_extract_buffer_t buffer
) {
    unsigned char tmp_byte = 0;

    (*counters.processed_ptr)++;
    if (0 == api->fread(&tmp_byte, 1, 1, archive)) {
        yc_res_dat_extract_cleanup(archive, api, NULL);
        return YC_RES_DAT_STATUS_IO;
    }

    uint16_t offset_next = tmp_byte;

    (*counters.processed_ptr)++;
    if (0 == api->fread(&tmp_byte, 1, 1, archive)) {
        yc_res_dat_extract_cleanup(archive, api, NULL);
        return YC_RES_DAT_STATUS_IO;
    }

    uint16_t length = tmp_byte;

    offset_next |= (0xF0 & length) << 4;
    length &= 0x0F;

    for (size_t j = 0; j < (length + buffer.match_min); j++) {
        unsigned char *byte = malloc(sizeof(unsigned char));

        if (NULL == byte) {
            yc_res_dat_extract_cleanup(archive, api, NULL);
            return YC_RES_DAT_STATUS_MEM;
        }

        *byte = buffer.bytes[offset_next];
        result->callback(byte, 1, result->context);
        (*counters.written_ptr)++;

        buffer.bytes[(*buffer.offset_ptr)++] = buffer.bytes[offset_next++];

        if ((*buffer.offset_ptr) >= buffer.size) { (*buffer.offset_ptr) = 0; }
        if (offset_next >= buffer.size) { offset_next = 0; }
    }

    return YC_RES_DAT_STATUS_OK;
}

void yc_res_dat_extract_cleanup(void *archive, const yc_res_io_fs_api_t *api, unsigned char *bytes) {
    if (NULL != archive) { api->fclose(archive); }
    if (NULL != bytes) { free(bytes); }
}
