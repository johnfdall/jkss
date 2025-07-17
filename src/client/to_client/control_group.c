#include "arena.c"
#include <stdbool.h>
#include <stdint.h>

#define MAIN_CONTROL_GROUP 11
#define MAX_GROUPS 12
#define SECTION_BITS 64
#define MAX_INDEX 128

#pragma once
typedef struct {
        uint64_t sectionOne;
        uint64_t sectionTwo;
} ControlGroup;

typedef struct {
        ControlGroup items[MAX_GROUPS];
        uint8_t count;
} ControlGroupArray;

void ControlGroupToggleUnit(ControlGroup *controlGroup, uint8_t index) {
        if (index < SECTION_BITS) {
                controlGroup->sectionOne = controlGroup->sectionOne ^ (1ULL << index);
        }

        if (index < MAX_INDEX) {
                controlGroup->sectionOne =
                        controlGroup->sectionTwo ^ (1ULL << (index - SECTION_BITS));
        }
}

ControlGroupArray *ControlGroupArray_INIT(Arena *arena) {
        ControlGroupArray *group =
                (ControlGroupArray *)ArenaAllocate(arena, sizeof(ControlGroupArray));

        for (int i = 0; i < MAX_GROUPS; i++) {
                group->items[i].sectionOne = 0;
                group->items[i].sectionTwo = 0;
        }

        return group;
}

bool CheckIfUnitIsInControlGroup(ControlGroup *group, uint8_t index) {
        if (index < SECTION_BITS) {
                return (group->sectionOne & (1ULL << index)) != 0;
        }
        if (index < MAX_INDEX) {
                return (group->sectionTwo & (1ULL << (index - 64))) != 0;
        }
        return false;
}
