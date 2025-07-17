#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "../network/protocol.h"
#include <netinet/in.h>

typedef struct {
        struct sockaddr_in addr;
        uint32_t last_seen_tick;
        uint8_t connected;
} client_info_t;

typedef struct {
        player_state_t players[MAX_PLAYERS];
        client_info_t clients[MAX_PLAYERS];
        uint32_t tick_count;
        uint32_t active_players;
} game_state_t;

void init_game_state(game_state_t* state);
int add_player(game_state_t* state, const struct sockaddr_in* client_addr);
void remove_player(game_state_t* state, uint32_t player_id);
void update_player_input(game_state_t* state, const player_input_t* input);
void tick_game_state(game_state_t* state);

#endif
