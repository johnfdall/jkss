#include "arena.c"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define MAX_SELECT_GROUP_SIZE 100
#define UNSET_VALUE 255

#pragma once
typedef struct {
        uint8_t items[MAX_SELECT_GROUP_SIZE];
        uint8_t count;
} SelectGroup;

void SelectGroup_ADD(SelectGroup *group, uint8_t index) {
        assert(group->count < MAX_SELECT_GROUP_SIZE && "SelectGroup is full");
        group->items[group->count++] = index;
}

SelectGroup *SelectGroup_INIT(Arena *arena) {
        SelectGroup *group = (SelectGroup *)ArenaAllocate(arena, sizeof(SelectGroup));
        group->count = 0;
        memset(group->items, UNSET_VALUE, sizeof(group->items));
        return group;
}

bool SelectGroup_CHECK(SelectGroup *group, uint8_t index) {
        for (int i = 0; i <= group->count; i++) {
                if (group->items[i] == index) {
                        return true;
                }
        }
        return false;
}
