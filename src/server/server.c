#include "../network/network.h"
#include "game_state.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#define ISVALIDSOCKET(s) ((s) >= 0)
#define SOCKET int
#define GETSOCKETERRNO() (errno)

typedef struct __attribute((packed)) {
        uint16_t x;
        uint16_t y;
} PlayerState;

typedef struct __attribute((packed)) {
        uint32_t sequence_number;
        uint8_t number_of_players;
        PlayerState players[10];
} GameStatePacket;

static void broadcast_game_state(int sockfd, const game_state_t *state) {
        game_state_msg_t msg;
        msg.header.type = MSG_GAME_STATE;
        msg.header.sequence = state->tick_count;
        msg.header.data_size = sizeof(game_state_msg_t) - sizeof(message_header_t);
        msg.tick = state->tick_count;
        msg.player_count = state->active_players;
        memcpy(msg.players, state->players, sizeof(state->players));

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

        if (bytes <= 0) return;

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

        game_state_t game_state;
        init_game_state(&game_state);

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
                }

                usleep(1000); // 1ms
        }

        close(sockfd);
        return 0;
}

// int main() {
//         printf("Configuring local address...\n");
//         struct addrinfo hints;
//         memset(&hints, 0, sizeof(hints));
//         hints.ai_family = AF_INET;
//         hints.ai_socktype = SOCK_DGRAM;
//         hints.ai_flags = AI_PASSIVE;
//         struct addrinfo *bind_address;
//         int result = getaddrinfo(0, "8080", &hints, &bind_address);
//         if (result != 0) {
//                 fprintf(stderr, "getaddrinfo() failed: %s", errno);
//                 return 1;
//         }
//
//         printf("Creating socket...\n");
//         SOCKET socket_listen;
//         socket_listen = socket(bind_address->ai_family,
//                         bind_address->ai_socktype,
//                         bind_address->ai_protocol);
//         if (!ISVALIDSOCKET(socket_listen)) {
//                 fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
//                 return 1;
//         }
//
//         printf("Binding socket to local address...\n");
//         if (bind(socket_listen, bind_address->ai_addr,
//         bind_address->ai_addrlen)) {
//                 fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
//                 return 1;
//         }
//         freeaddrinfo(bind_address);
//
//         fd_set master;
//         FD_ZERO(&master);
//         FD_SET(socket_listen, &master);
//         SOCKET max_socket = socket_listen;
//         printf("Waiting for connections...\n");
//
//         while(1) {
//                 fd_set reads;
//                 reads = master;
//                 if (select(max_socket+1, &reads, 0, 0, 0) < 0) {
//                         fprintf(stderr, "select() failed. (%d)\n",
//                         GETSOCKETERRNO()); return 1;
//                 }
//                 if (FD_ISSET(socket_listen, &reads)) {
//                         struct sockaddr_storage client_address;
//                         socklen_t client_len = sizeof(client_address);
//                         char read[45];
//                         int bytes_received = recvfrom(socket_listen,
//                                         read,
//                                         45,
//                                         0,
//                                         (struct sockaddr *)&client_address,
//                                         &client_len);
//
//                         if (bytes_received < 1) {
//                                 fprintf(stderr, "connection closed. (%d)\n",
//                                                 GETSOCKETERRNO());
//                                 return 1;
//                         }
//
//                         printf("Remote address is: ");
//                         char address_buffer[100];
//                         char service_buffer[100];
//                         getnameinfo((struct sockaddr *)&client_address,
//                                         client_len,
//                                         address_buffer,
//                                         sizeof(address_buffer),
//                                         service_buffer,
//                                         sizeof(service_buffer),
//                                         NI_NUMERICHOST);
//
//                         GameStatePacket *gameStatePacket = (GameStatePacket
//                         *)read;
//
//                         print_game_state_packet(gameStatePacket);
//
//                         // printf("Received from %s %s: \n%s\n",
//                         //                 address_buffer,
//                         //                 service_buffer,
//                         //                 read);
//                         // int j;
//                         // for (j = 0; j < bytes_received; ++j)
//                         //         read[j] = toupper(read[j]);
//                         // sendto(socket_listen,
//                         //                 read,
//                         //                 bytes_received,
//                         //                 0,
//                         //                 (struct sockaddr*)&client_address,
//                         //                 client_len);
//                 } //if FD_ISSET
//         } //while(1)
//         printf("Closing listening socket...\n");
//         close(socket_listen);
//         printf("Finished.\n");
//         return 0;
// }
