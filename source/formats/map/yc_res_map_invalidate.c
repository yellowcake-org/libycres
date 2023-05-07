#include <libycres.h>
#include <stdlib.h>

void yc_res_map_invalidate_object(yc_res_map_level_object_t *object);

void yc_res_map_invalidate(yc_res_map_t *map) {
    for (size_t elevation_idx = 0; elevation_idx < YC_RES_MAP_ELEVATION_COUNT; ++elevation_idx) {
        yc_res_map_level_t *level = map->levels[elevation_idx];

        if (NULL != level->objects.pointers) {
            for (size_t object_idx = 0; object_idx < level->objects.count; ++object_idx) {
                yc_res_map_invalidate_object(&level->objects.pointers[object_idx]);
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

    map->local.count = 0;
    if (NULL != map->local.values) {
        free(map->local.values);
        map->local.values = NULL;
    }

    map->global.count = 0;
    if (NULL != map->global.values) {
        free(map->global.values);
        map->global.values = NULL;
    }

    if (NULL != map->scripts.pointers) {
        for (size_t script_idx = 0; script_idx < map->scripts.count; ++script_idx) {
            yc_res_map_script_t *script = &map->scripts.pointers[script_idx];

            if (NULL != script->data.timed) {
                free(script->data.timed);
                script->data.timed = NULL;
            }

            if (NULL != script->data.spatial) {
                free(script->data.spatial);
                script->data.spatial = NULL;
            }
        }

        map->scripts.count = 0;

        free(map->scripts.pointers);
        map->scripts.pointers = NULL;
    }
}

void yc_res_map_invalidate_object(yc_res_map_level_object_t *object) {
    if (NULL != object->patch.item) {
        if (NULL != object->patch.item->data.weapon) {
            free(object->patch.item->data.weapon);
            object->patch.item->data.weapon = NULL;
        }

        if (NULL != object->patch.item->data.ammo) {
            free(object->patch.item->data.ammo);
            object->patch.item->data.ammo = NULL;
        }

        if (NULL != object->patch.item->data.misc) {
            free(object->patch.item->data.misc);
            object->patch.item->data.misc = NULL;
        }

        if (NULL != object->patch.item->data.key) {
            free(object->patch.item->data.key);
            object->patch.item->data.key = NULL;
        }

        free(object->patch.item);
        object->patch.item = NULL;
    }

    if (NULL != object->patch.critter) {
        free(object->patch.critter);
        object->patch.critter = NULL;
    }

    if (NULL != object->patch.scenery) {
        if (NULL != object->patch.scenery->data.door) {
            free(object->patch.scenery->data.door);
            object->patch.scenery->data.door = NULL;
        }

        if (NULL != object->patch.scenery->data.stairs) {
            free(object->patch.scenery->data.stairs);
            object->patch.scenery->data.stairs = NULL;
        }

        if (NULL != object->patch.scenery->data.elevator) {
            free(object->patch.scenery->data.elevator);
            object->patch.scenery->data.elevator = NULL;
        }

        if (NULL != object->patch.scenery->data.ladder) {
            free(object->patch.scenery->data.ladder);
            object->patch.scenery->data.ladder = NULL;
        }

        free(object->patch.scenery);
        object->patch.scenery = NULL;
    }

    if (NULL != object->patch.misc) {
        free(object->patch.misc);
        object->patch.misc = NULL;
    }

    if (NULL != object->inventory) {
        for (size_t object_idx = 0; object_idx < object->count; ++object_idx) {
            yc_res_map_invalidate_object(&object->inventory[object_idx]);
        }

        free(object->inventory);
        object->inventory = NULL;
    }

    object->count = 0;
}
