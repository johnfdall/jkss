#ifndef CONTROL_GROUP_H
#define CONTROL_GROUP_H 

#include <stdint.h>
#include "arena.h"
#include <stdbool.h>
#include <assert.h>
#include "../network/protocol.h"

typedef struct {
	int32_t *ids;
	size_t length;
	size_t capacity;
} ControlGroup;

void ControlGroup_ADD(ControlGroup *array, int32_t id);
void ControlGroup_SETUP(ControlGroup *array);
void ControlGroup_INIT(ControlGroup *array, Arena *arena, size_t capacity);
int32_t ControlGroup_GET(ControlGroup *array, size_t index);
void ControlGroup_TOGGLE(ControlGroup *controlGroup, int32_t id);
bool ControlGroup_EXISTS(ControlGroup *group, int32_t id);
int16_t ControlGroup_EXISTS_AT(ControlGroup *group, int32_t id);
void ControlGroup_PRINT(const ControlGroup * const control_group);
void ControlGroup_TO_NETPACKET(ControlGroup *control_group, player_input_t *packet);

#endif
