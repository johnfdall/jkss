#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <raylib.h>
#include "../common/types.h"
#include "../client/client_state.h"

void Input_RIGHT_CLICK(ClientState *client_state, ControlGroup *control_groups, int sockfd, struct sockaddr_in from_addr);

#endif
