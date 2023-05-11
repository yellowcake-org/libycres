#ifndef INCLUDE_PRO_TYPES_COMMON_STAT_H
#define INCLUDE_PRO_TYPES_COMMON_STAT_H

typedef enum yc_res_pro_stat {
    YC_RES_PRO_STAT_INVALID = -1,
    YC_RES_PRO_STAT_STRENGTH,
    YC_RES_PRO_STAT_PERCEPTION,
    YC_RES_PRO_STAT_ENDURANCE,
    YC_RES_PRO_STAT_CHARISMA,
    YC_RES_PRO_STAT_INTELLIGENCE,
    YC_RES_PRO_STAT_AGILITY,
    YC_RES_PRO_STAT_LUCK,
    YC_RES_PRO_STAT_MAXIMUM_HIT_POINTS,
    YC_RES_PRO_STAT_MAXIMUM_ACTION_POINTS,
    YC_RES_PRO_STAT_ARMOR_CLASS,
    YC_RES_PRO_STAT_UNARMED_DAMAGE,
    YC_RES_PRO_STAT_MELEE_DAMAGE,
    YC_RES_PRO_STAT_CARRY_WEIGHT,
    YC_RES_PRO_STAT_SEQUENCE,
    YC_RES_PRO_STAT_HEALING_RATE,
    YC_RES_PRO_STAT_CRITICAL_CHANCE,
    YC_RES_PRO_STAT_BETTER_CRITICALS,
    YC_RES_PRO_STAT_DAMAGE_THRESHOLD,
    YC_RES_PRO_STAT_DAMAGE_THRESHOLD_LASER,
    YC_RES_PRO_STAT_DAMAGE_THRESHOLD_FIRE,
    YC_RES_PRO_STAT_DAMAGE_THRESHOLD_PLASMA,
    YC_RES_PRO_STAT_DAMAGE_THRESHOLD_ELECTRICAL,
    YC_RES_PRO_STAT_DAMAGE_THRESHOLD_EMP,
    YC_RES_PRO_STAT_DAMAGE_THRESHOLD_EXPLOSION,
    YC_RES_PRO_STAT_DAMAGE_RESISTANCE,
    YC_RES_PRO_STAT_DAMAGE_RESISTANCE_LASER,
    YC_RES_PRO_STAT_DAMAGE_RESISTANCE_FIRE,
    YC_RES_PRO_STAT_DAMAGE_RESISTANCE_PLASMA,
    YC_RES_PRO_STAT_DAMAGE_RESISTANCE_ELECTRICAL,
    YC_RES_PRO_STAT_DAMAGE_RESISTANCE_EMP,
    YC_RES_PRO_STAT_DAMAGE_RESISTANCE_EXPLOSION,
    YC_RES_PRO_STAT_RADIATION_RESISTANCE,
    YC_RES_PRO_STAT_POISON_RESISTANCE,
    YC_RES_PRO_STAT_AGE,
    YC_RES_PRO_STAT_GENDER,
    YC_RES_PRO_STAT_CURRENT_HIT_POINTS,
    YC_RES_PRO_STAT_CURRENT_POISON_LEVEL,
    YC_RES_PRO_STAT_CURRENT_RADIATION_LEVEL,
    YC_RES_PRO_STAT_COUNT,
} yc_res_pro_stat_t;

#ifndef YC_RES_PRO_STAT_COUNT_PRIMARY
#define YC_RES_PRO_STAT_COUNT_PRIMARY (YC_RES_PRO_STAT_LUCK + 1)
#endif

#ifndef YC_RES_PRO_STAT_COUNT_SPECIAL
#define YC_RES_PRO_STAT_COUNT_SPECIAL (YC_RES_PRO_STAT_POISON_RESISTANCE + 1)
#endif

#ifndef YC_RES_PRO_STAT_COUNT_SAVED
#define YC_RES_PRO_STAT_COUNT_SAVED (YC_RES_PRO_STAT_GENDER + 1)

#endif

#endif //INCLUDE_PRO_TYPES_COMMON_STAT_H
