#include "arena.h"
#include "control_group.h"
#include <stdbool.h>
#include <stdint.h>


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
