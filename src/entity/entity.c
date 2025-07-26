#include "entity.h"
#include "control_group.h"
#include <stdint.h>
#include <stdio.h>

void DrawEntity(const Entity entity, ControlGroup *control_group) {
	DrawCircle(entity.position.x, entity.position.y, entity.radius, entity.color);
	char text_buffer[5];
	snprintf(text_buffer, sizeof(text_buffer), "%d", entity.id);
	DrawText(text_buffer, entity.position.x - 2, entity.position.y - 5, 10, YELLOW);
	if (ControlGroup_EXISTS(control_group, entity.id)) {
		DrawCircleLines(entity.position.x, entity.position.y, entity.radius + 10, entity.color);
	}
}

bool IsEntityClicked(const Entity entity) {
	const Vector2 mouse_position = GetMousePosition();
	return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
		CheckCollisionPointCircle(mouse_position, 
				          (Vector2){entity.position.x, entity.position.y},
				          entity.radius);
}
