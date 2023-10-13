#ifndef LIBYCRES_INCLUDE_PRO_TYPES_OBJECT_CRITTER_H
#define LIBYCRES_INCLUDE_PRO_TYPES_OBJECT_CRITTER_H

#include "body/body.h"
#include "flags/flags.h"
#include "kill/kill.h"

typedef struct yc_res_pro_object_critter {
    uint32_t script_id, sprite_id;
    uint32_t packet_idx, team_idx;

    yc_res_pro_damage_type_t damage;

    yc_res_pro_object_critter_body_t body;
    yc_res_pro_object_critter_flags_t flags;

    uint32_t kill_reward;
    yc_res_pro_object_critter_kill_t kill_type;

    uint32_t skills[YC_RES_PRO_SKILL_COUNT];

    uint32_t stats[YC_RES_PRO_STAT_COUNT_SAVED];
    int32_t modifiers[YC_RES_PRO_STAT_COUNT_SAVED];
} yc_res_pro_object_critter_t;

#endif //LIBYCRES_INCLUDE_PRO_TYPES_OBJECT_CRITTER_H
