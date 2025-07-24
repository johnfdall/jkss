#ifndef ENTITY_ARRAY_H
#define ENTITY_ARRAY_H 

#include "entity.h"

typedef struct {
	Entity *items;
	size_t length;
	size_t capacity;
} EntityArray;

void EntityArray_ADD(EntityArray *array, Entity entity);
void EntityArray_SETUP(EntityArray *array);
void EntityArray_INIT(EntityArray *array, Arena *arena, size_t capacity);
Entity EntityArray_GET(EntityArray *array, size_t index);
void EntityArray_UPSERT(EntityArray *array, Entity entity);
void HandleEntityClick(EntityArray *array, ControlGroup *group);
void DrawEntities(EntityArray *array, ControlGroup *group);
void PrintEntities(EntityArray *array);

#endif
