#include "arena.c"
#include "control_group.c"
#include "entity.c"
#include <assert.h>
#include <raylib.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
        Entity *items;
        size_t length;
        size_t capacity;
} EntityArray;

void EntityArray_ADD(EntityArray *array, Entity entity) {
        assert(array->length < array->capacity && "EntityArray is full!");
        array->items[array->length++] = entity;
}

void EntityArray_SETUP(EntityArray *array) {
        Entity entity1 = {.id = 1,
                .x = 256,
                .y = 144,
                .radius = 30.0,
                .color = RED,
                .moveSpeed = 400,
                .direction = {.x = 20.0, .y = 20.0}};

        Entity entity2 = {.id = 2,
                .x = 540,
                .y = 200,
                .radius = 30.0,
                .color = RED,
                .moveSpeed = 400,
                .direction = {.x = 20.0, .y = 20.0}};

        Entity entity3 = {.id = 3,
                .x = 868,
                .y = 400,
                .radius = 30.0,
                .color = RED,
                .moveSpeed = 400,
                .direction = {.x = 20.0, .y = 20.0}};

        Entity entity4 = {.id = 4,
                .x = 1000,
                .y = 666,
                .radius = 30.0,
                .color = RED,
                .moveSpeed = 400,
                .direction = {.x = 20.0, .y = 20.0}};

        Entity entity5 = {.id = 5,
                .x = 1280,
                .y = 420,
                .radius = 30.0,
                .color = RED,
                .moveSpeed = 400,
                .direction = {.x = 20.0, .y = 20.0}};

        Entity entity6 = {.id = 6,
                .x = 1423,
                .y = 864,
                .radius = 30.0,
                .color = RED,
                .moveSpeed = 400,
                .direction = {.x = 20.0, .y = 20.0}};

        Entity entity7 = {.id = 7,
                .x = 1792,
                .y = 1008,
                .radius = 30.0,
                .color = RED,
                .moveSpeed = 400,
                .direction = {.x = 20.0, .y = 20.0}};

        Entity entity8 = {.id = 8,
                .x = 2048,
                .y = 1159,
                .radius = 30.0,
                .color = RED,
                .moveSpeed = 400,
                .direction = {.x = 20.0, .y = 20.0}};

        Entity entity9 = {.id = 9,
                .x = 2304,
                .y = 1296,
                .radius = 30.0,
                .color = RED,
                .moveSpeed = 400,
                .direction = {.x = 20.0, .y = 20.0}};

        Entity entity10 = {.id = 10,
                .x = 2200,
                .y = 237,
                .radius = 30.0,
                .color = RED,
                .moveSpeed = 400,
                .direction = {.x = 20.0, .y = 20.0}};

        EntityArray_ADD(array, entity1);
        EntityArray_ADD(array, entity2);
        EntityArray_ADD(array, entity3);
        EntityArray_ADD(array, entity4);
        EntityArray_ADD(array, entity5);
        EntityArray_ADD(array, entity6);
        EntityArray_ADD(array, entity7);
        EntityArray_ADD(array, entity8);
        EntityArray_ADD(array, entity9);
        EntityArray_ADD(array, entity10);
}

void EntityArray_INIT(EntityArray *array, Arena *arena, size_t capacity) {
        array->items = (Entity *)ArenaAllocate(arena, capacity * sizeof(Entity));
        array->length = 0;
        array->capacity = capacity;
}

Entity EntityArray_GET(EntityArray *array, int32_t index) {
        assert(index >= 0 && index < array->length);
        if (index >= 0 && index < array->length) {
                return array->items[index];
        }

        Entity invalidEntity = {};
        return invalidEntity;
}

void HandleEntityClick(EntityArray *array, ControlGroup *group) {
        for (int32_t i = 0; i < array->length; i++) {
                Entity item = EntityArray_GET(array, i);
                if (IsEntityClicked(item)) {
                        ControlGroupToggleUnit(group, (uint8_t)i);
                };
        }
}

void DrawEntities(EntityArray *array, ControlGroup *group) {
        for (int32_t i = 0; i < array->length; i++) {
                Entity item = EntityArray_GET(array, i);
                DrawEntity(item, i, group);
        }
}

void PrintEntities(EntityArray *array) {
        for (int32_t i = 0; i < array->length; i++) {
                Entity item = EntityArray_GET(array, i);
                printf("%d\n", item.id);
        }
}
