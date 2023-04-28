#include <libycres.h>
#include <private.h>

#include <stdlib.h>

void yc_res_pro_critter_parse_flags(uint32_t flags, yc_res_pro_object_critter_t *into);
void yc_res_pro_critter_parse_cleanup(yc_res_pro_object_critter_t *critter);

yc_res_pro_status_t yc_res_pro_object_critter_parse(
        void *file,
        const yc_res_io_fs_api_t *io,
        yc_res_pro_object_t *into
) {
    yc_res_pro_object_critter_t *critter = malloc(sizeof(yc_res_pro_object_critter_t));

    if (NULL == critter) {
        yc_res_pro_critter_parse_cleanup(critter);
        return YC_RES_PRO_STATUS_MEM;
    }

    uint32_t flags_ext = 0;
    if (0 == io->fread(&flags_ext, sizeof(uint32_t), 1, file)) {
        yc_res_pro_critter_parse_cleanup(critter);
        return YC_RES_PRO_STATUS_IO;
    }
    flags_ext = yc_res_byteorder_uint32(flags_ext);

    if (0 == io->fread(&critter->script_id, sizeof(uint32_t), 1, file)) {
        yc_res_pro_critter_parse_cleanup(critter);
        return YC_RES_PRO_STATUS_IO;
    }
    critter->script_id = yc_res_byteorder_uint32(critter->script_id);

    if (0 == io->fread(&critter->sprite_id, sizeof(uint32_t), 1, file)) {
        yc_res_pro_critter_parse_cleanup(critter);
        return YC_RES_PRO_STATUS_IO;
    }
    critter->sprite_id = yc_res_byteorder_uint32(critter->sprite_id);

    if (0 == io->fread(&critter->packet_idx, sizeof(uint32_t), 1, file)) {
        yc_res_pro_critter_parse_cleanup(critter);
        return YC_RES_PRO_STATUS_IO;
    }
    critter->packet_idx = yc_res_byteorder_uint32(critter->packet_idx);

    if (0 == io->fread(&critter->team_idx, sizeof(uint32_t), 1, file)) {
        yc_res_pro_critter_parse_cleanup(critter);
        return YC_RES_PRO_STATUS_IO;
    }
    critter->team_idx = yc_res_byteorder_uint32(critter->team_idx);

    uint32_t flags = 0;
    if (0 == io->fread(&flags, sizeof(uint32_t), 1, file)) {
        yc_res_pro_critter_parse_cleanup(critter);
        return YC_RES_PRO_STATUS_IO;
    }
    flags = yc_res_byteorder_uint32(flags);
    yc_res_pro_critter_parse_flags(flags, critter);

    if (0 == io->fread(&critter->stats, sizeof(uint32_t), YC_RES_PRO_STAT_COUNT_SAVED, file)) {
        yc_res_pro_critter_parse_cleanup(critter);
        return YC_RES_PRO_STATUS_IO;
    }

    if (0 == io->fread(&critter->modifiers, sizeof(int32_t), YC_RES_PRO_STAT_COUNT_SAVED, file)) {
        yc_res_pro_critter_parse_cleanup(critter);
        return YC_RES_PRO_STATUS_IO;
    }

    for (size_t stat_idx = 0; stat_idx < YC_RES_PRO_STAT_COUNT_SAVED; ++stat_idx) {
        critter->stats[stat_idx] = yc_res_byteorder_uint32(critter->stats[stat_idx]);
        critter->modifiers[stat_idx] = yc_res_byteorder_int32(critter->modifiers[stat_idx]);
    }

    if (0 == io->fread(&critter->skills, sizeof(int32_t), YC_RES_PRO_SKILL_COUNT, file)) {
        yc_res_pro_critter_parse_cleanup(critter);
        return YC_RES_PRO_STATUS_IO;
    }

    for (size_t skill_idx = 0; skill_idx < YC_RES_PRO_SKILL_COUNT; ++skill_idx) {
        critter->skills[skill_idx] = yc_res_byteorder_uint32(critter->skills[skill_idx]);
    }

    if (0 == io->fread(&critter->body, sizeof(uint32_t), 1, file)) {
        yc_res_pro_critter_parse_cleanup(critter);
        return YC_RES_PRO_STATUS_IO;
    }
    critter->body = yc_res_byteorder_uint32(critter->body);

    if (0 == io->fread(&critter->kill_reward, sizeof(uint32_t), 1, file)) {
        yc_res_pro_critter_parse_cleanup(critter);
        return YC_RES_PRO_STATUS_IO;
    }
    critter->kill_reward = yc_res_byteorder_uint32(critter->kill_reward);

    if (0 == io->fread(&critter->kill_type, sizeof(uint32_t), 1, file)) {
        yc_res_pro_critter_parse_cleanup(critter);
        return YC_RES_PRO_STATUS_IO;
    }
    critter->kill_type = yc_res_byteorder_uint32(critter->kill_type);


    // absent for Fallout™ 1 .pro files, so don't check if successful
    io->fread(&critter->damage, sizeof(uint32_t), 1, file);
    critter->damage = yc_res_byteorder_uint32(critter->damage);

    into->data.critter = critter;
    return YC_RES_PRO_STATUS_OK;
}

void yc_res_pro_critter_parse_flags(uint32_t flags, yc_res_pro_object_critter_t *into) {
    into->flags.is_barter_available = 0x00000002 == (flags & 0x00000002);
    into->flags.no_corpse = 0x00000800 == (flags & 0x00000800);
}

void yc_res_pro_critter_parse_cleanup(yc_res_pro_object_critter_t *critter) {
    if (NULL != critter) { free(critter); }
}
