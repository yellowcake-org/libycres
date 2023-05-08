#include <libycres.h>
#include <private.h>

#include "yc_res_map_parse_object_patch.h"

#include <stdlib.h>

yc_res_map_status_t yc_res_map_parse_object_patch_critter(
        void *file,
        const yc_res_io_fs_api_t *io,
        const yc_res_map_parse_db_api_t *db,
        uint32_t pid,
        yc_res_map_level_object_patch_t *into
) {
    (void) (db);
    (void) (pid);

    into->critter = malloc(sizeof(yc_res_map_level_object_patch_critter_t));
    if (NULL == into->critter) { return YC_RES_MAP_STATUS_MEM; }

    uint32_t _damage_last_turn = 0;
    if (0 == io->fread(&_damage_last_turn, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    _damage_last_turn = yc_res_byteorder_uint32(_damage_last_turn);

    uint32_t _combat_state = 0;
    if (0 == io->fread(&_combat_state, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    _combat_state = yc_res_byteorder_uint32(_combat_state);

    uint32_t _action_points = 0;
    if (0 == io->fread(&_action_points, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    _action_points = yc_res_byteorder_uint32(_action_points);

    uint32_t _damage_flags = 0;
    if (0 == io->fread(&_damage_flags, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    _damage_flags = yc_res_byteorder_uint32(_damage_flags);

    if (0 == io->fread(&into->critter->packet_idx, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    into->critter->packet_idx = yc_res_byteorder_uint32(into->critter->packet_idx);

    if (0 == io->fread(&into->critter->team_idx, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    into->critter->team_idx = yc_res_byteorder_uint32(into->critter->team_idx);

    uint32_t _who_hit_me = 0;
    if (0 == io->fread(&_who_hit_me, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    _who_hit_me = yc_res_byteorder_uint32(_who_hit_me);

    yc_res_map_level_object_patch_critter_stat_t *hp =
            &into->critter->stats[YC_RES_PRO_STAT_CURRENT_HIT_POINTS - YC_RES_PRO_STAT_COUNT_SAVED];
    hp->stat = YC_RES_PRO_STAT_CURRENT_HIT_POINTS;

    if (0 == io->fread(&hp->value, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    hp->value = yc_res_byteorder_uint32(hp->value);

    yc_res_map_level_object_patch_critter_stat_t *pl =
            &into->critter->stats[YC_RES_PRO_STAT_CURRENT_POISON_LEVEL - YC_RES_PRO_STAT_COUNT_SAVED];
    hp->stat = YC_RES_PRO_STAT_CURRENT_POISON_LEVEL;

    if (0 == io->fread(&pl->value, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    pl->value = yc_res_byteorder_uint32(pl->value);

    yc_res_map_level_object_patch_critter_stat_t *rl =
            &into->critter->stats[YC_RES_PRO_STAT_CURRENT_RADIATION_LEVEL - YC_RES_PRO_STAT_COUNT_SAVED];
    hp->stat = YC_RES_PRO_STAT_CURRENT_RADIATION_LEVEL;

    if (0 == io->fread(&rl->value, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    rl->value = yc_res_byteorder_uint32(rl->value);

    return YC_RES_MAP_STATUS_OK;
}
