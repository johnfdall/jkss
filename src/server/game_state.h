#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "../network/protocol.h"
#include "../entity/entity_array.h"

typedef struct {
        struct sockaddr_in addr;
        uint32_t last_seen_tick;
        uint8_t connected;
} ClientInfo;

typedef struct {
        player_state_t players[2];
        ClientInfo clients[MAX_PLAYERS];
        EntityArray entities;
        uint8_t entity_count;
        uint32_t tick_count;
        uint32_t active_players;
} GameState;

int GameState_ADD_PLAYER(GameState* state, const struct sockaddr_in* client_addr);
void GameState_INIT(GameState* state);
void GameState_REMOVE_PLAYER(GameState* state, uint32_t player_id);
void GameState_UPDATE_INPUT(GameState* state, const player_input_t* input);
void GameState_TICK(GameState* state);
void GameState_BROADCAST(int sockfd, const GameState *state);

#endif
