#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)
#include <stdio.h>
#include <string.h>

typedef struct __attribute((packed)) {
        uint16_t x;
        uint16_t y;
} PlayerState;

typedef struct __attribute((packed)){
        uint32_t sequence_number;
        uint8_t number_of_players;
        PlayerState players[10];
} GameStatePacket;

void create_network_packet(char *buffer, GameStatePacket *gameState) {
        memcpy(buffer, gameState, sizeof(*gameState));
}

GameStatePacket create_dummy_game_state() {
        GameStatePacket gameStatePacket;
        memset(&gameStatePacket, 0, sizeof(gameStatePacket));
        PlayerState player_one;
        player_one.x = 100;
        player_one.y = 100;

        PlayerState player_two;
        player_two.x = 200;
        player_two.y = 200;

        gameStatePacket.sequence_number = 8;
        gameStatePacket.number_of_players = 2;
        gameStatePacket.players[0] = player_one;
        gameStatePacket.players[1] = player_two;

        return gameStatePacket;
}


int main(int argc, char *argv[]) {
        {
                if (argc < 3) {
                        fprintf(stderr, "usage: client hostname port\n");
                        return 1;
                }

                printf("Configuring remote address...\n");
                struct addrinfo hints;
                memset(&hints, 0, sizeof(hints));
                hints.ai_socktype = SOCK_DGRAM;
                struct addrinfo *peer_address;
                if (getaddrinfo(argv[1], argv[2], &hints, &peer_address)) {
                        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
                        return 1;
                }

                printf("Remote address is: ");
                char address_buffer[100];
                char service_buffer[100];
                getnameinfo(peer_address->ai_addr, 
                                peer_address->ai_addrlen, 
                                address_buffer,
                                sizeof(address_buffer), 
                                service_buffer, 
                                sizeof(service_buffer),
                                NI_NUMERICHOST);
                printf("%s %s\n", address_buffer, service_buffer);

                printf("Creating socket...\n");
                SOCKET socket_peer;
                socket_peer = socket(peer_address->ai_family, peer_address->ai_socktype,
                                peer_address->ai_protocol);
                if (!ISVALIDSOCKET(socket_peer)) {
                        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
                        return 1;
                }

                printf("Connecting...\n");
                if (connect(socket_peer, peer_address->ai_addr, peer_address->ai_addrlen)) {
                        fprintf(stderr, "connect() failed. (%d)\n", GETSOCKETERRNO());
                        return 1;
                }

                freeaddrinfo(peer_address);

                printf("Connected.\n");
                printf("To send data, enter text followed by enter.\n");

                while (1) {
                        fd_set reads;
                        FD_ZERO(&reads);
                        FD_SET(socket_peer, &reads);
                        FD_SET(fileno(stdin), &reads);
                        struct timeval timeout;
                        timeout.tv_sec = 0;
                        timeout.tv_usec = 100000;
                        if (select(socket_peer + 1, &reads, 0, 0, &timeout) < 0) {
                                fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
                                return 1;
                        }
                        if (FD_ISSET(socket_peer, &reads)) {
                                char read[4096];
                                int bytes_received = recv(socket_peer, read, 4096, 0);
                                if (bytes_received < 1) {
                                        printf("Connection closed by peer.\n");
                                        break;
                                }
                                printf("Received (%d bytes): %.*s", bytes_received, bytes_received, read);
                        }
                        if (FD_ISSET(fileno(stdin), &reads)) {
                                GameStatePacket gameStatePacket  = create_dummy_game_state();
                                char read_buffer[sizeof(gameStatePacket)];
                                create_network_packet(read_buffer, &gameStatePacket);

                                printf("Sending: %s", read_buffer);
                                int bytes_sent = send(socket_peer, read_buffer, sizeof(read_buffer), 0);
                                printf("Sent %d bytes.\n", bytes_sent);

                                // sendto(socket_listen,
                                //                 read,
                                //                 bytes_received,
                                //                 0,
                                //                 (struct sockaddr*)&client_address,
                                //                 client_len);
                        }
                }
                printf("Closing socket...\n");
                CLOSESOCKET(socket_peer);
                printf("Finished.\n");
                return 0;
        }
}
