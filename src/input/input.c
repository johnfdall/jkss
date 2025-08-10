#include "input.h"
#include "../network/protocol.h"
#include "../common/types.h"
#include "../network/network.h"
#include <raylib.h>

void Input_RIGHT_CLICK(ClientState *client_state, ControlGroup *control_groups, int sockfd, struct sockaddr_in from_addr) {
	if (IsMouseButtonPressed(1)) {
		client_state->sequence_number++;
		Vector2 mouse_position = GetMousePosition();
		Vec2 mp = {
			.x = mouse_position.x,
			.y = mouse_position.y
		};
		player_input_t input = {0};
		input.destination.x = mouse_position.x; 
		input.destination.y = mouse_position.y; 
		input.player_id = 0;
		input.command_type = CMD_MOVE;
		input.sequence_number = client_state->sequence_number;

		ControlGroup_TO_NETPACKET(control_groups, &input);
		ClientState_FROM_INPUT(client_state, &input, mp);

		input_msg_t msg = {0};
		msg.header.type = MSG_PLAYER_INPUT;
		msg.input = input;
		send_message(sockfd, &msg, sizeof(msg), &from_addr);
	}
}

void Input_BOX_SELECT_START(BoxSelectState *box_state) {
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		box_state->is_selecting = true;
		box_state->start_pos.x = GetMouseX();
		box_state->start_pos.y = GetMouseY();
		box_state->current_pos = box_state->start_pos;
	}
}

void Input_BOX_SELECT_UPDATE(BoxSelectState *box_state) {
	if (box_state->is_selecting && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		box_state->current_pos.x = GetMouseX();
		box_state->current_pos.y = GetMouseY();
	}
}

void Input_BOX_SELECT_END(BoxSelectState *box_state, ClientState *client_state, ControlGroup *control_groups) {
	if (box_state->is_selecting && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
		// Clear control group if not holding shift
		if (!IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_RIGHT_SHIFT)) {
			ControlGroup_CLEAR(control_groups);
		}
		
		// Select all entities within the box
		for (size_t i = 0; i < client_state->entities.length; i++) {
			Entity *entity = &client_state->entities.items[i];
			if (Input_IS_ENTITY_IN_BOX(entity, box_state)) {
				ControlGroup_ADD(control_groups, entity->id);
			}
		}
		
		box_state->is_selecting = false;
	}
}

void Input_DRAW_BOX_SELECT(const BoxSelectState *box_state) {
	if (box_state->is_selecting) {
		// Calculate rectangle bounds
		int32_t min_x = box_state->start_pos.x < box_state->current_pos.x ? box_state->start_pos.x : box_state->current_pos.x;
		int32_t min_y = box_state->start_pos.y < box_state->current_pos.y ? box_state->start_pos.y : box_state->current_pos.y;
		int32_t max_x = box_state->start_pos.x > box_state->current_pos.x ? box_state->start_pos.x : box_state->current_pos.x;
		int32_t max_y = box_state->start_pos.y > box_state->current_pos.y ? box_state->start_pos.y : box_state->current_pos.y;
		
		int32_t width = max_x - min_x;
		int32_t height = max_y - min_y;
		
		// Draw selection rectangle with semi-transparent fill and border
		// DrawRectangle(min_x, min_y, width, height, (Color){0, 255, 0, 50}); // Green with alpha
		DrawRectangleLines(min_x, min_y, width, height, GREEN);
	}
}

bool Input_IS_ENTITY_IN_BOX(const Entity *entity, const BoxSelectState *box_state) {
	if (!box_state->is_selecting) {
		return false;
	}
	
	// Calculate rectangle bounds
	int32_t min_x = box_state->start_pos.x < box_state->current_pos.x ? box_state->start_pos.x : box_state->current_pos.x;
	int32_t min_y = box_state->start_pos.y < box_state->current_pos.y ? box_state->start_pos.y : box_state->current_pos.y;
	int32_t max_x = box_state->start_pos.x > box_state->current_pos.x ? box_state->start_pos.x : box_state->current_pos.x;
	int32_t max_y = box_state->start_pos.y > box_state->current_pos.y ? box_state->start_pos.y : box_state->current_pos.y;
	
	// Check if entity center is within the rectangle
	return (entity->position.x >= min_x && entity->position.x <= max_x &&
	        entity->position.y >= min_y && entity->position.y <= max_y);
}

