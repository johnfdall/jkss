#include "control_group.h"
#include "arena.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

int16_t ControlGroup_EXISTS_AT(ControlGroup *group, int32_t id) {
	for (size_t i = 0; i < group->length; i++) {
		if (group->ids[i] == id) {
			return i;
		}
	}

	return -1;
}

void ControlGroup_PRINT(const ControlGroup * const control_group) {
	printf("ControlGroup: {Length: %zu, Capcity: %zu}\n", 
			control_group->length,
			control_group->capacity);
	printf("IDs in array: {");
	for (size_t i = 0; i < control_group->length; i++) {
		if(i == control_group->length-1) {
			printf("%d", control_group->ids[i]);
		}
		else {
			printf("%d, ", control_group->ids[i]);
		}
	}
	printf("}\n");
}

void ControlGroup_ADD(ControlGroup *control_group, int32_t id) {
	assert(control_group->length < control_group->capacity && "ControlGroup array is full!");
	//Check if we find a vacant spot.
	for (size_t i = 0; i < control_group->length; i++) {
		if(control_group->ids[i] == 0) {
			control_group->ids[i] = id;
			return;
		}
	}
	control_group->ids[control_group->length++] = id;
}

void ControlGroup_REMOVE_AT(ControlGroup *control_group, size_t index) {
	control_group->ids[index] = 0;
}

void ControlGroup_INIT(ControlGroup *array, Arena *arena, size_t capacity) {
	array->ids = ArenaAllocate(arena, capacity * sizeof(uint32_t));
	array->length = 0;
	array->capacity = capacity;
}

int32_t ControlGroup_GET(ControlGroup *array, size_t index) {
	assert(index < array->length);
	if (index < array->length) {
		return array->ids[index];
	}

	return 0;
}

void ControlGroup_TOGGLE(ControlGroup *control_group, int32_t id) {
	int8_t exists_at = ControlGroup_EXISTS_AT(control_group, id);
	if(exists_at == -1) {
		ControlGroup_ADD(control_group, id);
		ControlGroup_PRINT(control_group);
		return;
	}
	else {
		ControlGroup_REMOVE_AT(control_group, exists_at);
		ControlGroup_PRINT(control_group);
	}
}

bool ControlGroup_EXISTS(ControlGroup *control_group, int32_t id) {
	for (size_t i = 0; i < control_group->length; i++) {
		if (control_group->ids[i] == id) {
			return true;
		}
	}
	return false;
}


void ControlGroup_TO_NETPACKET(ControlGroup *control_group, player_input_t *packet) {
	for (size_t i = 0; i < control_group->length; i++) {
		if (control_group->ids[i] != 0) {
			// Find the first zero slot in packet->entity_ids
			for (size_t j = 0; j < MAX_ENTITIES; j++) {
				if (packet->entity_ids[j] == 0) {
					packet->entity_ids[j] = control_group->ids[i];
					break; // Move to the next control_group id
				}
			}
		}
	}
}
