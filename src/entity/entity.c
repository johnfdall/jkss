#include "entity.h"
#include "control_group.h"
#include <stdint.h>
#include <stdio.h>

void AffectWithGravity(Entity *entity, int screen_height) {
	if (entity->y + entity->radius >= screen_height) {
		return;
	}
	entity->y += 10;
}

void DrawEntity(const Entity entity, ControlGroup *group) {
	DrawCircle(entity.x, entity.y, entity.radius, entity.color);
	char textBuffer[5];
	snprintf(textBuffer, sizeof(textBuffer), "%d", entity.id);
	DrawText(textBuffer, entity.x - 2, entity.y - 5, 10, YELLOW);
	if (ControlGroup_EXISTS(group, entity.id)) {
		DrawCircleLines(entity.x, entity.y, entity.radius + 10, entity.color);
	}
}

bool IsEntityClicked(const Entity entity) {
	const Vector2 mousePos = GetMousePosition();
	return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
		CheckCollisionPointCircle(mousePos, (Vector2){entity.x, entity.y},
				entity.radius);
}
