#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "gameserver/ikcp.h"
#include "gameserver/packet.h"
#include "gameserver/session.h"

#define BUFFER_SIZE 1024

// void print_hex_buffer(uint8_t *buffer, size_t length) {
//     printf("[");
//     for (size_t i = 0; i < length; ++i) {
//         printf("0x%X", buffer[i]);
//         if (i < length - 1) {
//             printf(", ");
//         }
//     }
//     printf("]\n");
// }

int start_udp_server(const char *host, uint16_t port) {
    int sockfd;
    uint8_t buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "failed to create socket\n");
        return 0;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, host, &server_addr.sin_addr);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "failed to bind socket, maybe port is already used?\n");
        close(sockfd);
        return 0;
    }

    printf("gameserver listening on %s:%d\npress ctrl+c to stop\n", host, port);

    while (1) {
        ssize_t n = recvfrom(
            sockfd,
            buffer,
            BUFFER_SIZE,
            0,
            (struct sockaddr *)&client_addr,
            &addr_len
        );

        if (n < 0) {
            fprintf(stderr, "recvfrom failed\n");
            break;
        }
        
        // printf("received from %s:%d: ", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        // print_hex_buffer(buffer, n);

        if (n == 20) {
            process_net_operation(buffer, &client_addr, sockfd);
        } else if (n >= 28) {
            process_kcp_payload(buffer, n, &client_addr, sockfd);
        } else {
            printf("unknown recvfrom len: %ld", n);
        }
    }

    close(sockfd);
    
    return 0;
}
