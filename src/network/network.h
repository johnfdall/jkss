#ifndef NETWORK_H
#define NETWORK_H

#include <sys/socket.h>
#include <netinet/in.h>
#include "protocol.h"

int create_udp_socket(void);
int bind_socket(int sockfd, int port);
int set_socket_nonblocking(int sockfd);
ssize_t send_message(int sockfd, const void* data, size_t size, 
                const struct sockaddr_in* addr);
ssize_t receive_message(int sockfd, void* buffer, size_t buffer_size, 
                const struct sockaddr_in * const from_addr);

#endif
