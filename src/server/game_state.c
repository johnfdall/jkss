#include "game_state.h"
#include <stdio.h>
#include <string.h>

void init_game_state(game_state_t* state) {
        memset(state, 0, sizeof(*state));
        state->tick_count = 0;
        state->active_players = 0;
}

int add_player(game_state_t* state, const struct sockaddr_in* const client_addr) {
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

void remove_player(game_state_t* state, uint32_t player_id) {
        if (player_id < MAX_PLAYERS && state->clients[player_id].connected) {
                state->clients[player_id].connected = 0;
                state->players[player_id].active = 0;
                state->active_players--;
        }
}

static void print_player_input(const player_input_t * const input) {
        printf("Player: %d moved to (%f, %f) \n", input->player_id, input->move_x, input->move_y);
}

void update_player_input(game_state_t* state, const player_input_t* input) {
        print_player_input(input);
        uint32_t id = input->player_id;
        if (id < MAX_PLAYERS && state->players[id].active) {
                // Simple movement update - customize as needed
                state->players[id].vel_x = input->move_x * 100.0f; // pixels/sec
                state->players[id].vel_y = input->move_y * 100.0f;
                state->clients[id].last_seen_tick = state->tick_count;
        }
}

void tick_game_state(game_state_t* state) {
        float dt = 1.0f / TICK_RATE;

        for (int i = 0; i < MAX_PLAYERS; i++) {
                if (state->players[i].active) {
                        // Update positions
                        state->players[i].x += state->players[i].vel_x * dt;
                        state->players[i].y += state->players[i].vel_y * dt;

                        // Apply friction
                        state->players[i].vel_x *= 0.9f;
                        state->players[i].vel_y *= 0.9f;

                        // Disconnect inactive clients (timeout after 5 seconds)
                        if (state->tick_count - state->clients[i].last_seen_tick > 
                                        TICK_RATE * 5) {
                                remove_player(state, i);
                        }
                }
        }

        state->tick_count++;
}
