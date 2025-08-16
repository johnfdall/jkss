#include "client_state.h"

void ClientState_FROM_INPUT(ClientState *client_state, player_input_t *input, Vec2 pos) {
        int idx = 0;
        while(input->entity_ids[idx] != 0) {
                for (size_t i = 0; i < client_state->entities.length; i++) {
                        if(input->entity_ids[idx] == (int32_t)client_state->entities.items[i].id) {
                                client_state->entities.items[i].destination = pos;
                        }
                }
                idx++;
        }
}
