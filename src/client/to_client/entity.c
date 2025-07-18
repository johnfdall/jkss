#include "control_group.c"
#include "raylib.h"
#include <stdint.h>
#include <stdio.h>

typedef struct {
        int32_t id;
        int x;
        int y;
        float radius;
        Color color;
        int moveSpeed;
        Vector2 direction;
} Entity;

void AffectWithGravity(Entity *entity, int screen_height) {
        if (entity->y + entity->radius >= screen_height) {
                return;
        }
        entity->y += 10;
}

void DrawEntity(Entity entity, int index, ControlGroup *group) {
        DrawCircle(entity.x, entity.y, entity.radius, entity.color);
        char textBuffer[5];
        snprintf(textBuffer, sizeof(textBuffer), "%d", entity.id);
        DrawText(textBuffer, entity.x - 2, entity.y - 5, 10, YELLOW);
        if (CheckIfUnitIsInControlGroup(group, (uint8_t)index)) {
                DrawCircleLines(entity.x, entity.y, entity.radius + 10, entity.color);
        }
}

bool IsEntityClicked(Entity entity) {
        Vector2 mousePos = GetMousePosition();
        return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                CheckCollisionPointCircle(mousePos, (Vector2){entity.x, entity.y},
                                entity.radius);
}
