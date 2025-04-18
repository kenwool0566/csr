#ifndef SESSION_H
#define SESSION_H

#include <stdint.h>
#include <arpa/inet.h>

void process_net_operation(const uint8_t *data, const struct sockaddr_in *client_addr, int sockfd);
void process_kcp_payload(const uint8_t *data, ssize_t len, const struct sockaddr_in *client_addr, int sockfd);

#endif // SESSION_H
