#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct __attribute((packed)) {
        uint16_t x;
        uint16_t y;
} PlayerState;

typedef struct __attribute((packed)){
        uint32_t sequence_number;
        uint8_t number_of_players;
        PlayerState players[10];
} GameStatePacket;

int main() {
        GameStatePacket net_packet;
        memset(&net_packet, 0, sizeof(net_packet));
        PlayerState player_one;
        player_one.x = 100;
        player_one.y = 100;

        PlayerState player_two;
        player_two.x = 200;
        player_two.y = 200;

        net_packet.sequence_number = 1;
        net_packet.number_of_players = 2;
        net_packet.players[0] = player_one;
        net_packet.players[1] = player_two;

        printf("net_packet number: %d\n", net_packet.sequence_number);

        return EXIT_SUCCESS;
}
