#include "raylib.h"
#include "../network/protocol.h"

void draw_entity(const entity_state_t * const entity) {
        DrawCircle(entity->x, entity->y, 10, RED);
}

void draw_entities(const game_state_msg_t * const game_msg) {
        for (uint32_t i = 0; i < game_msg->entity_count; i++) {
                draw_entity(&game_msg->entities[i]);
        }
}
