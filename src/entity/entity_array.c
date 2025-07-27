#include "entity_array.h"
#include "control_group.h"
#include "entity.h"
#include <assert.h>
#include <stdio.h>

void EntityArray_ADD(EntityArray *array, Entity entity) {
	assert(array->length < array->capacity && "EntityArray is full!");
	array->items[array->length++] = entity;
}

void EntityArray_UPSERT(EntityArray *array, Entity entity) {
	bool exists = false;
	for (size_t i = 0; i < array->length; i++) {
		//Is already in the array
		if(array->items[i].id == entity.id) {
			exists = true;
			array->items[i] = entity;
			break;
		}
	}

	if(!exists) {
		EntityArray_ADD(array, entity);
	}
}

void EntityArray_SETUP(EntityArray *array) {
	Entity entity1 = {
		.id = 1,
		.position = {
			.x = 256,
			.y = 144,
		},
		.destination = {
			.x = 256,
			.y = 144,
		},
		.radius = 30.0,
		.color = RED,
		.moveSpeed = 400,
		.direction = {.x = 20.0, .y = 20.0}
	};

	Entity entity2 = {
		.id = 2,
		.position = {
			.x = 540,
			.y = 200,
		},
		.destination = {
			.x = 540,
			.y = 200,
		},
		.radius = 30.0,
		.color = RED,
		.moveSpeed = 400,
		.direction = {.x = 20.0, .y = 20.0}
	};

	Entity entity3 = {
		.id = 3,
		.position = {
			.x = 868,
			.y = 400,
		},
		.destination = {
			.x = 868,
			.y = 400,
		},
		.radius = 30.0,
		.color = RED,
		.moveSpeed = 400,
		.direction = {.x = 20.0, .y = 20.0}};

	Entity entity4 = {.id = 4,
		.position = {
			.x = 1000,
			.y = 666,
		},
		.destination = {
			.x = 1000,
			.y = 666,
		},
		.radius = 30.0,
		.color = RED,
		.moveSpeed = 400,
		.direction = {.x = 20.0, .y = 20.0}
	};

	Entity entity5 = {
		.id = 5,
		.position = {
			.x = 1280,
			.y = 420,
		},
		.destination = {
			.x = 1280,
			.y = 420,
		},
		.radius = 30.0,
		.color = RED,
		.moveSpeed = 400,
		.direction = {.x = 20.0, .y = 20.0}
	};

	Entity entity6 = {
		.id = 6,
		.position = {
			.x = 1423,
			.y = 864,
		},
		.destination = {
			.x = 1423,
			.y = 864,
		},
		.radius = 30.0,
		.color = RED,
		.moveSpeed = 400,
		.direction = {.x = 20.0, .y = 20.0}
	};

	Entity entity7 = {.id = 7,
		.position = {
			.x = 1792,
			.y = 1008,
		},
		.destination = {
			.x = 1792,
			.y = 1008,
		},
		.radius = 30.0,
		.color = RED,
		.moveSpeed = 400,
		.direction = {.x = 20.0, .y = 20.0}
	};

	Entity entity8 = {
		.id = 8,
		.position = {
			.x = 2048,
			.y = 1159,
		},
		.destination = {
			.x = 2048,
			.y = 1159,
		},
		.radius = 30.0,
		.color = RED,
		.moveSpeed = 400,
		.direction = {.x = 20.0, .y = 20.0}
	};

	Entity entity9 = {
		.id = 9,
		.position = {
			.x = 2304,
			.y = 1296,
		},
		.destination = {
			.x = 2304,
			.y = 1296,
		},
		.radius = 30.0,
		.color = RED,
		.moveSpeed = 400,
		.direction = {.x = 20.0, .y = 20.0}
	};

	Entity entity10 = {
		.id = 10,
		.position = {
			.x = 2200,
			.y = 237,
		},
		.destination = {
			.x = 2200,
			.y = 237,
		},
		.radius = 30.0,
		.color = RED,
		.moveSpeed = 400,
		.direction = {.x = 20.0, .y = 20.0}
	};

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

Entity EntityArray_GET(EntityArray *array, size_t index) {
	assert(index < array->length);
	if (index < array->length) {
		return array->items[index];
	}

	Entity invalidEntity = {};
	return invalidEntity;
}

void EntityArray_TO_NETPACKET(const EntityArray *const entities, game_state_msg_t *const msg) {
	for (size_t i = 0; i < entities->length; i++) {
		msg->entities[i].id = entities->items[i].id;
		msg->entities[i].position = entities->items[i].position;
		msg->entities[i].destination = entities->items[i].destination;
	}
	msg->entity_count = entities->length;
}

void HandleEntityClick(EntityArray *array, ControlGroup *group) {
	for (size_t i = 0; i < array->length; i++) {
		Entity item = EntityArray_GET(array, i);
		if(IsEntityShiftClicked(item)) {
			ControlGroup_TOGGLE(group, item.id);
			break;
		}
		if (IsEntityClicked(item)) {
			ControlGroup_SELECT(group, item.id);
			break;
		};
	}
}

void DrawEntities(EntityArray *array, ControlGroup *group) {
	for (size_t i = 0; i < array->length; i++) {
		Entity item = EntityArray_GET(array, i);
		DrawEntity(item, group);
	}
}

void PrintEntities(EntityArray *array) {
	for (size_t i = 0; i < array->length; i++) {
		Entity item = EntityArray_GET(array, i);
		printf("%d\n", item.id);
	}
}

