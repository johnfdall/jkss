#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <netinet/in.h>
#include "../common/types.h"

#define MAX_PLAYERS 100
#define MAX_ENTITIES 100
#define SERVER_PORT 8080
#define TICK_RATE 60
#define TICK_INTERVAL_MS (1000 / TICK_RATE)

// Message types
typedef enum {
        MSG_PLAYER_JOIN,
        MSG_PLAYER_LEAVE,
        MSG_PLAYER_INPUT,
        MSG_GAME_STATE
} message_type_t;

// Player input structure
typedef struct {
        uint32_t player_id;
	uint32_t sequence_number;
	int32_t entity_ids[MAX_ENTITIES];
	Point destination;
	CommandType command_type;
} player_input_t;

// Player state
typedef struct {
        uint32_t id;
        float x, y;
        float vel_x, vel_y;
        uint8_t active;
} player_state_t;

// Network message header
typedef struct {
	uint32_t client_id;
        message_type_t type;
        uint16_t data_size;
} message_header_t;

typedef struct {
    uint32_t id;
	Point position; 
	Point destination;
} entity_state_t;

// Game state message
typedef struct {
        message_header_t header;
	uint32_t last_processed_sequence;
        uint32_t tick;
        uint32_t player_count;
        player_state_t players[2];
        entity_state_t entities[MAX_ENTITIES];
	uint32_t entity_count;
} game_state_msg_t;

// Input message
typedef struct {
        message_header_t header;
        player_input_t input;
} input_msg_t;


#endif
