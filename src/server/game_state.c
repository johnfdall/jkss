#include "game_state.h"
#include "../network/network.h"
#include "../entity/entity_array.h"
#include "../common/utils.h"
#include <string.h>
#include <stdio.h>

void GameState_INIT(GameState* state) {
	memset(state, 0, sizeof(*state));
	state->tick_count = 0;
	state->active_players = 0;
}

int GameState_ADD_PLAYER(GameState* state, const struct sockaddr_in* const client_addr) {
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (!state->clients[i].connected) {
			state->clients[i].addr = *client_addr;
			state->clients[i].connected = 1;
			state->clients[i].last_seen_tick = state->tick_count;

			state->players[i].id = i;
			state->players[i].active = 1;
			state->players[i].x = 0.0f;
			state->players[i].y = 0.0f;
			state->players[i].vel_x = 0.0f;
			state->players[i].vel_y = 0.0f;

			state->active_players++;
			return i;
		}
	}
	return -1; // Server full
}

void GameState_REMOVE_PLAYER(GameState* state, uint32_t player_id) {
	if (player_id < MAX_PLAYERS && state->clients[player_id].connected) {
		state->clients[player_id].connected = 0;
		state->players[player_id].active = 0;
		state->active_players--;
	}
}

void GameState_UPDATE_INPUT(GameState* state, input_msg_t* msg) {
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if(state->clients[i].client_id == msg->header.client_id){
			state->clients[i].last_processed_sequence = msg->input.sequence_number;
			state->clients[i].last_seen_tick = state->tick_count;
			break;
		}
	}

	int idx = 0;
	while(msg->input.entity_ids[idx] != 0) {
		for (size_t i = 0; i < state->entities.length; i++) {
			if((uint32_t)msg->input.entity_ids[idx] == state->entities.items[i].id) {
				state->entities.items[i].destination = decompress_position(msg->input.destination);
			}
		}
		idx++;
	}
}

void GameState_TICK(GameState* state) {
	float dt = 1.0f / TICK_RATE;
	UpdateEntities(&state->entities, dt);

	state->tick_count++;
}

void GameState_BROADCAST(int sockfd, const GameState *state) {
	game_state_msg_t msg;
	msg.header.type		= MSG_GAME_STATE;
	msg.header.data_size	= sizeof(game_state_msg_t) - sizeof(message_header_t);
	msg.tick		= state->tick_count;
	msg.player_count	= state->active_players;
	msg.entity_count	= state->entity_count;
	memcpy(msg.players, state->players, sizeof(state->players));
	EntityArray_TO_NETPACKET(&state->entities, &msg);

	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (state->clients[i].connected) {
			msg.last_processed_sequence = state->clients[i].last_processed_sequence;
			msg.header.client_id = state->clients[i].client_id;
			send_message(sockfd, &msg, sizeof(msg), &state->clients[i].addr);
		}
	}
}
