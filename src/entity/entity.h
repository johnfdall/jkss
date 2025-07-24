#ifndef ENTITY_H
#define ENTITY_H 

#include <stdint.h>
#include "raylib.h"
#include "control_group.h"

typedef struct {
        int32_t id;
        int x;
        int y;
        float radius;
        Color color;
        int moveSpeed;
        Vector2 direction;
} Entity;

void AffectWithGravity(Entity *entity, int screen_height);
void DrawEntity(Entity entity, int index, ControlGroup *group);
bool IsEntityClicked(Entity entity);

#endif
