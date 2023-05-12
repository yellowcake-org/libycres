#include <libycres.h>
#include <private.h>

#include <stdlib.h>

yc_res_map_status_t yc_res_map_parse_object_patch_critter(
        void *file,
        const yc_res_io_fs_api_t *api,
        const yc_res_map_parse_db_api_t *fetchers,
        uint32_t pid,
        yc_res_map_level_object_patch_t *into
) {
    (void) (fetchers);
    (void) (pid);

    into->critter = malloc(sizeof(yc_res_map_level_object_patch_critter_t));
    if (NULL == into->critter) { return YC_RES_MAP_STATUS_MEM; }

    uint32_t damage_last_turn = 0;
    if (0 == api->fread(&damage_last_turn, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    damage_last_turn = yc_res_byteorder_uint32(damage_last_turn);

    uint32_t combat_state = 0;
    if (0 == api->fread(&combat_state, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    combat_state = yc_res_byteorder_uint32(combat_state);

    uint32_t action_points = 0;
    if (0 == api->fread(&action_points, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    action_points = yc_res_byteorder_uint32(action_points);

    uint32_t damage_flags = 0;
    if (0 == api->fread(&damage_flags, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    damage_flags = yc_res_byteorder_uint32(damage_flags);

    if (0 == api->fread(&into->critter->packet_idx, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    into->critter->packet_idx = yc_res_byteorder_uint32(into->critter->packet_idx);

    if (0 == api->fread(&into->critter->team_idx, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    into->critter->team_idx = yc_res_byteorder_uint32(into->critter->team_idx);

    uint32_t who_hit_me = 0;
    if (0 == api->fread(&who_hit_me, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    who_hit_me = yc_res_byteorder_uint32(who_hit_me);

    yc_res_map_level_object_patch_critter_stat_t *hit_points =
            &into->critter->stats[YC_RES_PRO_STAT_CURRENT_HIT_POINTS - YC_RES_PRO_STAT_COUNT_SAVED];
    hit_points->stat = YC_RES_PRO_STAT_CURRENT_HIT_POINTS;

    if (0 == api->fread(&hit_points->value, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    hit_points->value = yc_res_byteorder_uint32(hit_points->value);

    yc_res_map_level_object_patch_critter_stat_t *poison_level =
            &into->critter->stats[YC_RES_PRO_STAT_CURRENT_POISON_LEVEL - YC_RES_PRO_STAT_COUNT_SAVED];
    hit_points->stat = YC_RES_PRO_STAT_CURRENT_POISON_LEVEL;

    if (0 == api->fread(&poison_level->value, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    poison_level->value = yc_res_byteorder_uint32(poison_level->value);

    yc_res_map_level_object_patch_critter_stat_t *radiation_level =
            &into->critter->stats[YC_RES_PRO_STAT_CURRENT_RADIATION_LEVEL - YC_RES_PRO_STAT_COUNT_SAVED];
    hit_points->stat = YC_RES_PRO_STAT_CURRENT_RADIATION_LEVEL;

    if (0 == api->fread(&radiation_level->value, sizeof(uint32_t), 1, file)) { return YC_RES_MAP_STATUS_IO; }
    radiation_level->value = yc_res_byteorder_uint32(radiation_level->value);

    return YC_RES_MAP_STATUS_OK;
}
