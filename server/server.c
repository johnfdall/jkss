#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define ISVALIDSOCKET(s) ((s) >= 0)
#define SOCKET int
#define GETSOCKETERRNO() (errno)

typedef struct __attribute((packed)) {
        uint16_t x;
        uint16_t y;
} PlayerState;

typedef struct __attribute((packed)){
        uint32_t sequence_number;
        uint8_t number_of_players;
        PlayerState players[10];
} GameStatePacket;

void print_game_state_packet(const GameStatePacket * const gameState){
        printf("Received Game State Packet...\n");
        printf("Sequence_number: %d | number_of_players: %d \n", 
                        gameState->sequence_number, 
                        gameState->number_of_players);
}

int main() {
        printf("Configuring local address...\n");
        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = AI_PASSIVE;
        struct addrinfo *bind_address;
        int result = getaddrinfo(0, "8080", &hints, &bind_address);
        if (result != 0) {
                fprintf(stderr, "getaddrinfo() failed: %s", errno);
                return 1;
        }

        printf("Creating socket...\n");
        SOCKET socket_listen;
        socket_listen = socket(bind_address->ai_family,
                        bind_address->ai_socktype, 
                        bind_address->ai_protocol);
        if (!ISVALIDSOCKET(socket_listen)) {
                fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
                return 1;
        }

        printf("Binding socket to local address...\n");
        if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
                fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
                return 1;
        }
        freeaddrinfo(bind_address);

        fd_set master;
        FD_ZERO(&master);
        FD_SET(socket_listen, &master);
        SOCKET max_socket = socket_listen;
        printf("Waiting for connections...\n");

        while(1) {
                fd_set reads;
                reads = master;
                if (select(max_socket+1, &reads, 0, 0, 0) < 0) {
                        fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
                        return 1;
                }
                if (FD_ISSET(socket_listen, &reads)) {
                        struct sockaddr_storage client_address;
                        socklen_t client_len = sizeof(client_address);
                        char read[45];
                        int bytes_received = recvfrom(socket_listen, 
                                        read, 
                                        45, 
                                        0,
                                        (struct sockaddr *)&client_address, 
                                        &client_len);

                        if (bytes_received < 1) {
                                fprintf(stderr, "connection closed. (%d)\n",
                                                GETSOCKETERRNO());
                                return 1;
                        }

                        printf("Remote address is: ");
                        char address_buffer[100];
                        char service_buffer[100];
                        getnameinfo((struct sockaddr *)&client_address, 
                                        client_len, 
                                        address_buffer,
                                        sizeof(address_buffer), 
                                        service_buffer, 
                                        sizeof(service_buffer),
                                        NI_NUMERICHOST);

                        GameStatePacket *gameStatePacket = (GameStatePacket *)read;

                        print_game_state_packet(gameStatePacket);

                        // printf("Received from %s %s: \n%s\n", 
                        //                 address_buffer, 
                        //                 service_buffer, 
                        //                 read);
                        // int j;
                        // for (j = 0; j < bytes_received; ++j)
                        //         read[j] = toupper(read[j]);
                        // sendto(socket_listen, 
                        //                 read, 
                        //                 bytes_received, 
                        //                 0,
                        //                 (struct sockaddr*)&client_address, 
                        //                 client_len);
                } //if FD_ISSET
        } //while(1)
        printf("Closing listening socket...\n");
        close(socket_listen);
        printf("Finished.\n");
        return 0;
}
