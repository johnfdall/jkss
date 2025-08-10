#ifndef CLIENT_STATE_H
#define CLIENT_STATE_H 

#include <stdint.h>
#include "../entity/entity_array.h"

typedef struct {
	uint32_t id;
	uint32_t tick_count;
	EntityArray entities;
	uint32_t sequence_number;
} ClientState;


void ClientState_FROM_INPUT(ClientState *client_state, player_input_t *input, Vec2 pos);

#endif
