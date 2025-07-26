#include "entity.h"
#include "control_group.h"
#include <stdint.h>
#include <stdio.h>

void DrawEntity(const Entity entity, ControlGroup *group) {
	DrawCircle(entity.position.x, entity.position.y, entity.radius, entity.color);
	char textBuffer[5];
	snprintf(textBuffer, sizeof(textBuffer), "%d", entity.id);
	DrawText(textBuffer, entity.position.x - 2, entity.position.y - 5, 10, YELLOW);
	if (ControlGroup_EXISTS(group, entity.id)) {
		DrawCircleLines(entity.position.x, entity.position.y, entity.radius + 10, entity.color);
	}
}

bool IsEntityClicked(const Entity entity) {
	const Vector2 mousePos = GetMousePosition();
	return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
		CheckCollisionPointCircle(mousePos, 
				(Vector2){entity.position.x, entity.position.y},
				entity.radius);
}
