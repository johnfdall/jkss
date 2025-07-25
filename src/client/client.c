#include "../entity/arena.h"
#include "../input/input.h"
#include "../entity/control_group.h"
#include "../entity/entity.h"
#include "../entity/entity_array.h"
#include "../network/network.h"
#include "client_state.h"
#include "raylib.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define SOCKET int
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

static void EntityArray_FROM_NETWORK_MSG(EntityArray *array,
		game_state_msg_t *network_msg) {
	for (uint32_t i = 0; i < network_msg->entity_count; i++) {
		entity_state_t incoming_entity = network_msg->entities[i];

		Entity client_entity = {.id = incoming_entity.id,
			.x = incoming_entity.x,
			.y = incoming_entity.y,
			.radius = 30.0,
			.color = RED,
			.moveSpeed = 400,
			.direction = {.x = 20.0, .y = 20.0}};

		EntityArray_UPSERT(array, client_entity);
	}
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Usage: %s <server_ip>\n", argv[0]);
		return 1;
	}

	SOCKET sockfd = create_udp_socket();
	if (sockfd < 0) {
		perror("Failed to create socket");
		return 1;
	}

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib - Hello");
	SetTargetFPS(60);

	struct sockaddr_in server_addr = {0};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

	message_header_t join_msg;
	join_msg.type = MSG_PLAYER_JOIN;
	join_msg.sequence = 0;
	join_msg.data_size = 0;

	send_message(sockfd, &join_msg, sizeof(join_msg), &server_addr);
	printf("Sent join request to server\n");

	// Setup game state
	Arena entity_arena;
	ArenaInit(&entity_arena, 1024 * 1024 * 1024); // Should be 1 gibby
	ControlGroup controlGroups;
	ControlGroup_INIT(&controlGroups, &entity_arena, 100);
	EntityArray entityArray;
	EntityArray_INIT(&entityArray, &entity_arena, 20);


	ClientState clientState = {0};
	// TODO(John Fredrik): This should come from some kind of
	//"joined server" message with initial client state data
	clientState.id = 1;
	clientState.entities = entityArray;

	char buffer[4096];
	while (!WindowShouldClose()) {
		struct sockaddr_in from_addr;
		ssize_t bytes = receive_message(sockfd, buffer, sizeof(buffer), &from_addr);

		if (bytes > 0) {
			message_header_t *header = (message_header_t *)buffer;
			if (header->type == MSG_GAME_STATE) {
				game_state_msg_t *state_msg = (game_state_msg_t *)buffer;
				clientState.tick_count = state_msg->tick;
				EntityArray_FROM_NETWORK_MSG(&clientState.entities, state_msg);
			}
		} else {
			fprintf(stderr, "recv() failed: errno=%d (%s)\n", errno, strerror(errno));
		}

		// Inputs
		// Right-click
		if (IsMouseButtonPressed(1)) {
			//TODO (John Fredrik): Send move command with all entities in control group
			player_input_t input = {0};
			input.move_x = GetMouseX();
			input.move_y = GetMouseY();
			input.player_id = 0;
			input.command_type = CMD_MOVE;
			ControlGroup_TO_NETPACKET(&controlGroups, &input);

			input_msg_t msg = {0};
			msg.header.type = MSG_PLAYER_INPUT;
			msg.input = input;

			send_message(sockfd, &msg, sizeof(msg), &from_addr);
		}

		HandleEntityClick(&clientState.entities, &controlGroups);
		DrawEntities(&clientState.entities, &controlGroups);
		BeginDrawing();
		ClearBackground(BLACK);
		EndDrawing();
	}

	close(sockfd);
	CloseWindow();
	return EXIT_SUCCESS;
}
