#include "client_state.h"

void ClientState_FROM_INPUT(ClientState *client_state, player_input_t *input) {
	int idx = 0;
	while(input->entity_ids[idx] != 0) {
		for (size_t i = 0; i < client_state->entities.length; i++) {
			if(input->entity_ids[idx] == client_state->entities.items[i].id) {
				client_state->entities.items[i].destination = input->destination;
			}
		}
		idx++;
	}
}
