#ifndef LIBYCRES_INCLUDE_PRO_TYPES_OBJECT_ITEM_WEAPON_H
#define LIBYCRES_INCLUDE_PRO_TYPES_OBJECT_ITEM_WEAPON_H

typedef enum yc_res_pro_object_item_weapon_animation {
    YC_RES_PRO_OBJECT_ITEM_WEAPON_ANIMATION_NONE = 0x00,
    YC_RES_PRO_OBJECT_ITEM_WEAPON_ANIMATION_KNIFE,
    YC_RES_PRO_OBJECT_ITEM_WEAPON_ANIMATION_CLUB,
    YC_RES_PRO_OBJECT_ITEM_WEAPON_ANIMATION_HAMMER,
    YC_RES_PRO_OBJECT_ITEM_WEAPON_ANIMATION_SPEAR,
    YC_RES_PRO_OBJECT_ITEM_WEAPON_ANIMATION_PISTOL,
    YC_RES_PRO_OBJECT_ITEM_WEAPON_ANIMATION_SMG,
    YC_RES_PRO_OBJECT_ITEM_WEAPON_ANIMATION_SHOTGUN,
    YC_RES_PRO_OBJECT_ITEM_WEAPON_ANIMATION_LASER_RIFLE,
    YC_RES_PRO_OBJECT_ITEM_WEAPON_ANIMATION_MINIGUN,
    YC_RES_PRO_OBJECT_ITEM_WEAPON_ANIMATION_ROCKET_LAUNCHER
} yc_res_pro_object_item_weapon_animation_t;

typedef struct yc_res_pro_object_item_weapon_req {
    uint32_t value;
    yc_res_pro_stat_t stat;
} yc_res_pro_object_item_weapon_req_t;

typedef struct yc_res_pro_object_item_weapon_fire {
    uint32_t max_range, cost;
} yc_res_pro_object_item_weapon_fire_t;

typedef struct yc_res_pro_object_item_weapon {
    yc_res_pro_object_item_weapon_animation_t animation;
    yc_res_pro_object_item_weapon_fire_t fire_primary, fire_secondary;

    uint32_t capacity, burst;
    uint32_t ammo_pid, projectile_pid;

    uint32_t damage_min, damage_max;
    yc_res_pro_damage_type_t damage_type;

    unsigned char sound_id;
    char critical_failure_idx;

    yc_res_pro_perk_t perk;
    yc_res_pro_caliber_t caliber;
    yc_res_pro_object_item_weapon_req_t requirement;
} yc_res_pro_object_item_weapon_t;

#endif //LIBYCRES_INCLUDE_PRO_TYPES_OBJECT_ITEM_WEAPON_H
