#ifndef LIBYCRES_INCLUDE_MAP_TYPES_LEVEL_OBJECT_PATCH_CRITTER_H
#define LIBYCRES_INCLUDE_MAP_TYPES_LEVEL_OBJECT_PATCH_CRITTER_H

typedef struct yc_res_map_level_object_patch_critter_stat {
    uint32_t value;
    yc_res_pro_stat_t stat;
} yc_res_map_level_object_patch_critter_stat_t;

typedef struct yc_res_map_level_object_patch_critter {
    uint32_t packet_idx, team_idx;

    /// These are 'current' stats, see appropriate enum.
    yc_res_map_level_object_patch_critter_stat_t stats[
            YC_RES_PRO_STAT_COUNT - YC_RES_PRO_STAT_COUNT_SAVED
    ];
} yc_res_map_level_object_patch_critter_t;

#endif //LIBYCRES_INCLUDE_MAP_TYPES_LEVEL_OBJECT_PATCH_CRITTER_H
