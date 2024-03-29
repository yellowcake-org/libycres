#ifndef LIBYCRES_INCLUDE_PRO_TYPES_COMMON_DAMAGE_H
#define LIBYCRES_INCLUDE_PRO_TYPES_COMMON_DAMAGE_H

typedef enum yc_res_pro_damage_type {
    YC_RES_PRO_DAMAGE_TYPE_NORMAL,
    YC_RES_PRO_DAMAGE_TYPE_LASER,
    YC_RES_PRO_DAMAGE_TYPE_FIRE,
    YC_RES_PRO_DAMAGE_TYPE_PLASMA,
    YC_RES_PRO_DAMAGE_TYPE_ELECTRICAL,
    YC_RES_PRO_DAMAGE_TYPE_EMP,
    YC_RES_PRO_DAMAGE_TYPE_EXPLOSION,
    YC_RES_PRO_DAMAGE_TYPE_COUNT
} yc_res_pro_damage_type_t;

#endif //LIBYCRES_INCLUDE_PRO_TYPES_COMMON_DAMAGE_H
