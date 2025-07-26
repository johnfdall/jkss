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


static void handle_client_message(int sockfd, GameState *state) {
	char buffer[1024];
	struct sockaddr_in client_addr;

	ssize_t bytes = receive_message(sockfd, buffer, sizeof(buffer), &client_addr);

	if (bytes <= 0)
		return;

	message_header_t *header = (message_header_t *)buffer;

	switch (header->type) {
		case MSG_PLAYER_JOIN: {
					      int player_id = GameState_ADD_PLAYER(state, &client_addr);
					      printf("Player %d joined\n", player_id);
					      break;
				      }

		case MSG_PLAYER_INPUT: {
					       input_msg_t *input_msg = (input_msg_t *)buffer;
					       printf("\n");
					       size_t i = 0;
					       while(input_msg->input.entity_ids[i] != 0) {
						       printf("%d", input_msg->input.entity_ids[i]);
						       i++;
					       }
					       printf("-> type: %d -> {%d, %d}\n", 
							       input_msg->input.command_type,
							       input_msg->input.destination.x,
							       input_msg->input.destination.y);
					       printf("incoming sequence number: %d\n", input_msg->input.sequence_number);

					       GameState_UPDATE_INPUT(state, input_msg);
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

        EntityArray entity_array;
        EntityArray_INIT(&entity_array, &entity_arena, 20);

	GameState game_state;
	GameState_INIT(&game_state);
	game_state.entities = entity_array;
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
			GameState_TICK(&game_state);
			GameState_BROADCAST(sockfd, &game_state);
			tick_time = current_time;
		}

		struct timespec ts = {0};
		ts.tv_nsec = 1000000; // 1 millisecond = 1,000,000 nanoseconds
		nanosleep(&ts, NULL);
	}

	close(sockfd);
	return 0;
}
