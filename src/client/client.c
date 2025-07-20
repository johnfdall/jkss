#include "raylib.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#define SOCKET int
#define GETSOCKETERRNO() (errno)
#include <stdio.h>
#include "../network/network.h"
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080


void draw_entity(const entity_state_t * const entity) {
        DrawCircle(entity->x, entity->y, 10, RED);
}

void draw_entities(const game_state_msg_t * const game_msg) {
        for (uint32_t i = 0; i < game_msg->entity_count; i++) {
                draw_entity(&game_msg->entities[i]);
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
        char buffer[4096];
        while (!WindowShouldClose()) {
                struct sockaddr_in from_addr;
                ssize_t bytes = receive_message(
                                sockfd, 
                                buffer, 
                                sizeof(buffer), 
                                &from_addr);

                if (bytes > 0) {
                        message_header_t* header = (message_header_t*)buffer;
                        if (header->type == MSG_GAME_STATE) {
                                game_state_msg_t* state_msg = (game_state_msg_t*)buffer;
                                draw_entities(state_msg);
                                printf("Tick: %u, Players: %u\n", 
                                                state_msg->tick, 
                                                state_msg->player_count);

                        }
                }
                else {
                        //TODO(John Fredrik): Handle this properly, or not at all
                        printf("error in bytes D: %zd\n", bytes);
                }

                //Test sending some Input every frame
                player_input_t input = {0};
                input.move_x = 10.0;
                input.move_x = 10.0;
                input.player_id = 0;

                input_msg_t msg = {0};
                msg.header.type = MSG_PLAYER_INPUT;
                msg.input = input;

                send_message(sockfd, &msg, sizeof(msg), &from_addr);
                BeginDrawing();
                ClearBackground(BLACK);
                EndDrawing();
        }

        close(sockfd);
        CloseWindow();
        return EXIT_SUCCESS;
}
