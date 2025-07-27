#ifndef ENTITY_H
#define ENTITY_H 

#include <stdint.h>
#include "raylib.h"
#include "control_group.h"

typedef struct {
        int32_t id;
	Vec2 position;
	Vec2 destination;
        Vec2 direction;
        float radius;
        Color color;
        int moveSpeed;
} Entity;

void DrawEntity(Entity entity, ControlGroup *group);
void UpdateEntity(Entity *entity, float deltaTime);
bool IsEntityClicked(Entity entity);
bool IsEntityShiftClicked(const Entity entity);

#endif
