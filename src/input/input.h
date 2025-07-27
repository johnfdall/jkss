#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <raylib.h>
#include "../common/types.h"
#include "../client/client_state.h"
#include "../entity/entity.h"

void Input_RIGHT_CLICK(ClientState *client_state, ControlGroup *control_groups, int sockfd, struct sockaddr_in from_addr);
void Input_BOX_SELECT_START(BoxSelectState *box_state);
void Input_BOX_SELECT_UPDATE(BoxSelectState *box_state);
void Input_BOX_SELECT_END(BoxSelectState *box_state, ClientState *client_state, ControlGroup *control_groups);
void Input_DRAW_BOX_SELECT(const BoxSelectState *box_state);
bool Input_IS_ENTITY_IN_BOX(const Entity *entity, const BoxSelectState *box_state);

#endif
