#ifndef CONTROL_GROUP_H
#define CONTROL_GROUP_H 

#include <stdint.h>
#include "arena.h"
#include <stdbool.h>

#define MAIN_CONTROL_GROUP 11
#define MAX_GROUPS 12
#define SECTION_BITS 64
#define MAX_INDEX 128

typedef struct {
        uint64_t sectionOne;
        uint64_t sectionTwo;
} ControlGroup;

typedef struct {
        ControlGroup items[MAX_GROUPS];
        uint8_t count;
} ControlGroupArray;

void ControlGroupToggleUnit(ControlGroup *controlGroup, uint8_t index);
ControlGroupArray *ControlGroupArray_INIT(Arena *arena);
bool CheckIfUnitIsInControlGroup(ControlGroup *group, uint8_t index);

#endif
