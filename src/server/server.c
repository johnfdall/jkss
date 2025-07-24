#include "../network/network.h"
#include "../entity/arena.h"
#include "../entity/entity_array.h"
#include "game_state.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#define ISVALIDSOCKET(s) ((s) >= 0)
#define SOCKET int
#define GETSOCKETERRNO() (errno)

static void entity_to_msg(const EntityArray *const entities, game_state_msg_t *const msg) {
	for (size_t i = 0; i < entities->length; i++) {
		msg->entities[i].id = entities->items[i].id;
		msg->entities[i].x = entities->items[i].x;
		msg->entities[i].y = entities->items[i].y;
	}
	msg->entity_count = entities->length;
}

static void broadcast_game_state(int sockfd, const game_state_t *state) {
	game_state_msg_t msg;
	msg.header.type = MSG_GAME_STATE;
	msg.header.sequence = state->tick_count;
	msg.header.data_size = sizeof(game_state_msg_t) - sizeof(message_header_t);
	msg.tick = state->tick_count;
	msg.player_count = state->active_players;
	msg.entity_count = state->entity_count;
	memcpy(msg.players, state->players, sizeof(state->players));
	entity_to_msg(&state->entities, &msg);

	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (state->clients[i].connected) {
			send_message(sockfd, &msg, sizeof(msg), &state->clients[i].addr);
		}
	}
}

static void handle_client_message(int sockfd, game_state_t *state) {
	char buffer[1024];
	struct sockaddr_in client_addr;

	ssize_t bytes = receive_message(sockfd, buffer, sizeof(buffer), &client_addr);

	if (bytes <= 0)
		return;

	message_header_t *header = (message_header_t *)buffer;

	switch (header->type) {
		case MSG_PLAYER_JOIN: {
					      int player_id = add_player(state, &client_addr);
					      printf("Player %d joined\n", player_id);
					      break;
				      }
		case MSG_PLAYER_INPUT: {
					       input_msg_t *input_msg = (input_msg_t *)buffer;
					       update_player_input(state, &input_msg->input);
					       break;
				       }
		default:
				       break;
	}
}

int main() {
	int sockfd = create_udp_socket();
	if (sockfd < 0) {
		perror("Failed to create socket");
		return 1;
	}

	if (bind_socket(sockfd, SERVER_PORT) < 0) {
		perror("Failed to bind socket");
		close(sockfd);
		return 1;
	}

	if (set_socket_nonblocking(sockfd) < 0) {
		perror("Failed to set socket non-blocking");
		close(sockfd);
		return 1;
	}

	Arena entity_arena;
        ArenaInit(&entity_arena, 1024 * 1024 * 1024); // Should be 1 gibby

        EntityArray entityArray;
        EntityArray_INIT(&entityArray, &entity_arena, 20);

	game_state_t game_state;
	init_game_state(&game_state);
	game_state.entities = entityArray;
	EntityArray_SETUP(&game_state.entities);

	printf("Server started on port %d\n", SERVER_PORT);
	printf("Tick rate: %d Hz\n", TICK_RATE);

	struct timespec tick_time;
	timespec_get(&tick_time, TIME_UTC);

	while (1) {
		handle_client_message(sockfd, &game_state);

		struct timespec current_time;
		timespec_get(&current_time, TIME_UTC);

		long elapsed_ms = (current_time.tv_sec - tick_time.tv_sec) * 1000 +
			(current_time.tv_nsec - tick_time.tv_nsec) / 1000000;

		if (elapsed_ms >= TICK_INTERVAL_MS) {
			tick_game_state(&game_state);
			broadcast_game_state(sockfd, &game_state);
			tick_time = current_time;
			printf("Broadcasting tick: %d\n", game_state.tick_count);
		}

		struct timespec ts = {0};
		ts.tv_nsec = 1000000; // 1 millisecond = 1,000,000 nanoseconds
		nanosleep(&ts, NULL);
	}

	close(sockfd);
	return 0;
}
