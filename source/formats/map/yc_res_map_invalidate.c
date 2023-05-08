#include <libycres.h>
#include <stdlib.h>

void yc_res_map_invalidate_object(yc_res_map_level_object_t *object);

void yc_res_map_invalidate(yc_res_map_t *map) {
    for (size_t elevation_idx = 0; elevation_idx < YC_RES_MAP_ELEVATION_COUNT; ++elevation_idx) {
        yc_res_map_level_t *level = map->levels[elevation_idx];
        if (NULL == level) { continue; }

        if (NULL != level->objects.pointers) {
            for (size_t object_idx = 0; object_idx < level->objects.count; ++object_idx) {
                yc_res_map_level_object_t *object = &level->objects.pointers[object_idx];
                if (NULL != object) { yc_res_map_invalidate_object(object); }
            }

            free(level->objects.pointers);
            level->objects.pointers = NULL;
        }

        level->objects.count = 0;

        if (NULL != level) {
            free(level);
            map->levels[elevation_idx] = NULL;
        }
    }

    if (0 < map->local.count && NULL != map->local.values) {
        free(map->local.values);
        map->local.values = NULL;
        map->local.count = 0;
    }

    if (0 < map->global.count && NULL != map->global.values) {
        free(map->global.values);
        map->global.values = NULL;
        map->global.count = 0;
    }

    if (0 < map->scripts.count && NULL != map->scripts.pointers) {
        for (size_t script_idx = 0; script_idx < map->scripts.count; ++script_idx) {
            yc_res_map_script_t *script = &map->scripts.pointers[script_idx];

            if (YC_RES_PRO_SCRIPT_TYPE_TIMED == yc_res_pro_script_type_from_sid(script->script_id) &&
                NULL != script->data.timed) {
                free(script->data.timed);
                script->data.timed = NULL;
            }

            if (YC_RES_PRO_SCRIPT_TYPE_SPATIAL == yc_res_pro_script_type_from_sid(script->script_id) &&
                NULL != script->data.spatial) {
                free(script->data.spatial);
                script->data.spatial = NULL;
            }
        }
        
        free(map->scripts.pointers);
        map->scripts.pointers = NULL;
        
        map->scripts.count = 0;
    }
}

void yc_res_map_invalidate_object(yc_res_map_level_object_t *object) {
    if (YC_RES_PRO_OBJECT_TYPE_ITEM == yc_res_pro_object_type_from_pid(object->proto_id)
        && NULL != object->patch.item) {
        if (YC_RES_PRO_OBJECT_ITEM_TYPE_WEAPON == object->patch.item->type && NULL != object->patch.item->data.weapon) {
            free(object->patch.item->data.weapon);
            object->patch.item->data.weapon = NULL;
        }

        if (YC_RES_PRO_OBJECT_ITEM_TYPE_AMMO == object->patch.item->type && NULL != object->patch.item->data.ammo) {
            free(object->patch.item->data.ammo);
            object->patch.item->data.ammo = NULL;
        }

        if (YC_RES_PRO_OBJECT_ITEM_TYPE_MISC == object->patch.item->type && NULL != object->patch.item->data.misc) {
            free(object->patch.item->data.misc);
            object->patch.item->data.misc = NULL;
        }

        if (YC_RES_PRO_OBJECT_ITEM_TYPE_KEY == object->patch.item->type && NULL != object->patch.item->data.key) {
            free(object->patch.item->data.key);
            object->patch.item->data.key = NULL;
        }

        free(object->patch.item);
        object->patch.item = NULL;
    }

    if (YC_RES_PRO_OBJECT_TYPE_CRITTER == yc_res_pro_object_type_from_pid(object->proto_id) &&
        NULL != object->patch.critter) {
        free(object->patch.critter);
        object->patch.critter = NULL;
    }

    if (YC_RES_PRO_OBJECT_TYPE_SCENERY == yc_res_pro_object_type_from_pid(object->proto_id) &&
        NULL != object->patch.scenery) {
        if (NULL != object->patch.scenery->data.door
            && YC_RES_PRO_OBJECT_SCENERY_TYPE_DOOR == object->patch.scenery->type) {
            free(object->patch.scenery->data.door);
            object->patch.scenery->data.door = NULL;
        }

        if (YC_RES_PRO_OBJECT_SCENERY_TYPE_STAIRS == object->patch.scenery->type
            && NULL != object->patch.scenery->data.stairs) {
            free(object->patch.scenery->data.stairs);
            object->patch.scenery->data.stairs = NULL;
        }

        if (YC_RES_PRO_OBJECT_SCENERY_TYPE_ELEVATOR == object->patch.scenery->type &&
            NULL != object->patch.scenery->data.elevator) {
            free(object->patch.scenery->data.elevator);
            object->patch.scenery->data.elevator = NULL;
        }

        if ((YC_RES_PRO_OBJECT_SCENERY_TYPE_LADDER_BOTTOM == object->patch.scenery->type ||
             YC_RES_PRO_OBJECT_SCENERY_TYPE_LADDER_TOP == object->patch.scenery->type) &&
            NULL != object->patch.scenery->data.ladder) {
            free(object->patch.scenery->data.ladder);
            object->patch.scenery->data.ladder = NULL;
        }

        free(object->patch.scenery);
        object->patch.scenery = NULL;
    }

    if (YC_RES_PRO_OBJECT_TYPE_MISC == yc_res_pro_object_type_from_pid(object->proto_id) &&
        NULL != object->patch.misc) {
        free(object->patch.misc);
        object->patch.misc = NULL;
    }

    if (NULL != object->inventory) {
        for (size_t object_idx = 0; object_idx < object->capacity; ++object_idx) {
            yc_res_map_level_object_t *item = object->inventory[object_idx];
            if (NULL != item) { yc_res_map_invalidate_object(item); }
        }

        free(object->inventory);
        object->inventory = NULL;
    }

    object->capacity = 0;
    object->occupied = 0;
}
