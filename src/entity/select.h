#ifndef SELECT_H
#define SELECT_H 

#include <stdint.h>
#include "arena.h"
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define MAX_SELECT_GROUP_SIZE 100
#define UNSET_VALUE 255

typedef struct {
        uint8_t items[MAX_SELECT_GROUP_SIZE];
        uint8_t count;
} SelectGroup;

void SelectGroup_ADD(SelectGroup *group, uint8_t index);

SelectGroup *SelectGroup_INIT(Arena *arena);
bool SelectGroup_CHECK(SelectGroup *group, uint8_t index);

#endif
