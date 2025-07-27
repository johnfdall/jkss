#include "entity.h"
#include "control_group.h"
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

void DrawEntity(const Entity entity, ControlGroup *control_group) {
	DrawCircle(entity.position.x, entity.position.y, entity.radius, entity.color);
	char text_buffer[5];
	snprintf(text_buffer, sizeof(text_buffer), "%d", entity.id);
	DrawText(text_buffer, entity.position.x - 2, entity.position.y - 5, 10, YELLOW);
	if (ControlGroup_EXISTS(control_group, entity.id)) {
		DrawCircleLines(entity.position.x, entity.position.y, entity.radius + 10, entity.color);
		DrawLine(entity.position.x, entity.position.y, entity.destination.x, entity.destination.y, RED);
	}
}

void UpdateEntity(Entity *entity, float deltaTime) {
	// Calculate distance to destination
	float dx = entity->destination.x - entity->position.x;
	float dy = entity->destination.y - entity->position.y;
	float distance = sqrtf(dx * dx + dy * dy);
	
	// If we're close enough to the destination, stop moving
	if (distance < 1.0f) {
		entity->position.x = entity->destination.x;
		entity->position.y = entity->destination.y;
		entity->direction.x = 0.0f;
		entity->direction.y = 0.0f;
		return;
	}
	
	// Calculate normalized direction vector
	entity->direction.x = dx / distance;
	entity->direction.y = dy / distance;
	
	// Move towards destination
	float moveDistance = entity->moveSpeed * deltaTime;
	entity->position.x += entity->direction.x * moveDistance;
	entity->position.y += entity->direction.y * moveDistance;
}

bool IsEntityClicked(const Entity entity) {
	const Vector2 mouse_position = GetMousePosition();
	return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
		CheckCollisionPointCircle(mouse_position, 
				          (Vector2){entity.position.x, entity.position.y},
				          entity.radius);
}

bool IsEntityShiftClicked(const Entity entity) {
	const Vector2 mouse_position = GetMousePosition();
	const bool is_left_clicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
	const bool is_shift_held = IsKeyDown(KEY_LEFT_SHIFT);
	return is_left_clicked && is_shift_held &&
		CheckCollisionPointCircle(mouse_position, 
				          (Vector2){entity.position.x, entity.position.y},
				          entity.radius);
}
