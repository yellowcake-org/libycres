#include <libycres.h>
#include <private.h>

#include <stdlib.h>
#include <memory.h>

void yc_res_dat_extract_cleanup(
        void *archive,
        const yc_res_io_fs_api_t *io,
        unsigned char *bytes
);

yc_res_dat_status_t yc_res_dat_extract(
        const char *filename,
        const yc_res_io_fs_api_t *io,
        yc_res_dat_file_t *file,
        yc_res_dat_extract_result_t *result
) {
    void *archive = io->fopen(filename, "rb");

    if (NULL == archive) {
        yc_res_dat_extract_cleanup(archive, io, NULL);
        return YC_RES_DAT_STATUS_IO;
    }

    if (0 != io->fseek(archive, file->offset, SEEK_SET)) {
        yc_res_dat_extract_cleanup(archive, io, NULL);
        return YC_RES_DAT_STATUS_IO;
    }

    if (false == file->is_compressed) {
        for (size_t byte_idx = 0; byte_idx < file->count_plain; ++byte_idx) {
            unsigned char *byte = malloc(sizeof(unsigned char));

            if (NULL == byte) {
                yc_res_dat_extract_cleanup(archive, io, NULL);
                return YC_RES_DAT_STATUS_MEM;
            }

            if (0 == io->fread(byte, 1, 1, archive)) {
                yc_res_dat_extract_cleanup(archive, io, byte);
                return YC_RES_DAT_STATUS_IO;
            }

            result->callback(byte, 1, result->context);
        }
    } else {
        size_t processed = 0, written = 0;

        while (processed < file->count_compressed) {
            int16_t count = 0;

            processed += 2;
            if (0 == io->fread(&count, 2, 1, archive)) {
                yc_res_dat_extract_cleanup(archive, io, NULL);
                return YC_RES_DAT_STATUS_IO;
            }

            count = yc_res_byteorder_int16(count);

            if (count == 0) { break; }

            if (count < 0) {
                size_t end = -count;
                size_t limit = file->count_plain - written;
                size_t chunk_size = (limit < end ? limit : end);

                if (chunk_size > 0) {
                    unsigned char *bytes = malloc(chunk_size);

                    if (NULL == bytes) {
                        yc_res_dat_extract_cleanup(archive, io, NULL);
                        return YC_RES_DAT_STATUS_MEM;
                    }

                    processed += chunk_size;
                    if (0 == io->fread(bytes, chunk_size, 1, archive)) {
                        yc_res_dat_extract_cleanup(archive, io, bytes);
                        return YC_RES_DAT_STATUS_IO;
                    }

                    result->callback(bytes, chunk_size, result->context);
                    written += chunk_size;
                }
            } else {
                const size_t SIZE = 4096;

                const size_t MATCH_MIN = 3;
                const size_t MATCH_MAX = 18;

                unsigned char buffer[SIZE];
                memset(buffer, 0x20, SIZE);

                size_t end = processed + count;
                uint16_t offset_r = SIZE - MATCH_MAX;

                while (processed < end) {
                    unsigned char flags = 0;

                    processed++;
                    if (0 == io->fread(&flags, 1, 1, archive)) {
                        yc_res_dat_extract_cleanup(archive, io, NULL);
                        return YC_RES_DAT_STATUS_IO;
                    }

                    for (unsigned int i = 0; i < 8 && processed < end; i++) {
                        if ((flags & 1) != 0) {
                            unsigned char *byte = malloc(sizeof(unsigned char));

                            if (NULL == byte) {
                                yc_res_dat_extract_cleanup(archive, io, NULL);
                                return YC_RES_DAT_STATUS_MEM;
                            }

                            processed++;
                            if (0 == io->fread(byte, 1, 1, archive)) {
                                yc_res_dat_extract_cleanup(archive, io, byte);
                                return YC_RES_DAT_STATUS_IO;
                            }

                            buffer[offset_r] = *byte;

                            written++;
                            result->callback(byte, 1, result->context);

                            offset_r++;
                            if (offset_r >= SIZE) { offset_r = 0; }
                        } else {
                            unsigned char tmp_byte = 0;

                            processed++;
                            if (0 == io->fread(&tmp_byte, 1, 1, archive)) {
                                yc_res_dat_extract_cleanup(archive, io, NULL);
                                return YC_RES_DAT_STATUS_IO;
                            }

                            uint16_t offset_w = tmp_byte;

                            processed++;
                            if (0 == io->fread(&tmp_byte, 1, 1, archive)) {
                                yc_res_dat_extract_cleanup(archive, io, NULL);
                                return YC_RES_DAT_STATUS_IO;
                            }

                            uint16_t length = tmp_byte;

                            offset_w |= (0xF0 & length) << 4;
                            length &= 0x0F;

                            for (size_t j = 0; j < (length + MATCH_MIN); j++) {
                                unsigned char *byte = malloc(sizeof(unsigned char));

                                if (NULL == byte) {
                                    yc_res_dat_extract_cleanup(archive, io, NULL);
                                    return YC_RES_DAT_STATUS_MEM;
                                }

                                *byte = buffer[offset_w];
                                buffer[offset_r] = buffer[offset_w];

                                written++;
                                result->callback(byte, 1, result->context);

                                offset_w++;
                                offset_r++;

                                if (offset_r >= SIZE) { offset_r = 0; }
                                if (offset_w >= SIZE) { offset_w = 0; }
                            }
                        }

                        flags >>= 1;
                    }
                }
            }
        }

        if (written != file->count_plain) {
            yc_res_dat_extract_cleanup(archive, io, NULL);
            return YC_RES_DAT_STATUS_CORR;
        }
    }

    yc_res_dat_extract_cleanup(archive, io, NULL);
    return YC_RES_DAT_STATUS_OK;
}

void yc_res_dat_extract_cleanup(
        void *archive,
        const yc_res_io_fs_api_t *io,
        unsigned char *bytes
) {
    if (NULL != archive) {
        io->fclose(archive);
    }

    if (NULL != bytes) {
        free(bytes);
    }
}
