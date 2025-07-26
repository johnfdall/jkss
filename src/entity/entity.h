#ifndef ENTITY_H
#define ENTITY_H 

#include <stdint.h>
#include "raylib.h"
#include "control_group.h"

typedef struct {
        int32_t id;
	Point position;
	Point destination;
        float radius;
        Color color;
        int moveSpeed;
        Vector2 direction;
} Entity;

void DrawEntity(Entity entity, ControlGroup *group);
bool IsEntityClicked(Entity entity);

#endif
