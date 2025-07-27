#include "input.h"
#include "../network/protocol.h"
#include "../network/network.h"

void Input_RIGHT_CLICK(ClientState *client_state, ControlGroup *control_groups, int sockfd, struct sockaddr_in from_addr) {
	if (IsMouseButtonPressed(1)) {
		client_state->sequence_number++;
		player_input_t input = {0};
		input.destination.x = GetMouseX(); 
		input.destination.y = GetMouseY(); 
		input.player_id = 0;
		input.command_type = CMD_MOVE;
		input.sequence_number = client_state->sequence_number;

		ControlGroup_TO_NETPACKET(control_groups, &input);
		ClientState_FROM_INPUT(client_state, &input);

		input_msg_t msg = {0};
		msg.header.type = MSG_PLAYER_INPUT;
		msg.input = input;
		send_message(sockfd, &msg, sizeof(msg), &from_addr);
	}
}

