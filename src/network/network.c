#include "network.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int create_udp_socket(void) {
        return socket(AF_INET, SOCK_DGRAM, 0);
}

int bind_socket(int sockfd, int port) {
        // TODO(John Fredrik): I think this is the same 
        // as memset?
        // struct sockaddr_in addr = {0};
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        return bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
}

int set_socket_nonblocking(const int sockfd) {
        //Get the flags currently on the file descriptor
        int flags = fcntl(sockfd, F_GETFL, 0);
        //If there are none, then error out
        if (flags == -1) return -1;
        //Add the 0_NONBLOCK flag
        //Hence the name set_socket_nonblocking
        return fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

ssize_t send_message(const int sockfd, 
                const void* const data, 
                const size_t size, 
                const struct sockaddr_in* const addr) {
        return sendto(sockfd, data, size, 0, 
                        (struct sockaddr*)addr, sizeof(*addr));
}

ssize_t receive_message(const int sockfd, 
                void* buffer, 
                const size_t buffer_size, 
                const struct sockaddr_in * const from_addr) {
        socklen_t addr_len = sizeof(*from_addr);
        return recvfrom(sockfd, buffer, buffer_size, 0, 
                        (struct sockaddr*)from_addr, &addr_len);
}
