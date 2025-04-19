#ifndef SESSION_H
#define SESSION_H

#include <stdint.h>
#include <arpa/inet.h>
#include "gameserver/ikcp.h"
#include "shared/cassie.pb-c.h"

typedef struct {
    struct sockaddr_in addr;
    int sockfd;
    uint32_t conv_id;
    uint32_t token;
    ikcpcb *kcp;
} Session;

void process_net_operation(const uint8_t *data, const struct sockaddr_in *client_addr, int sockfd);
void process_kcp_payload(const uint8_t *data, ssize_t len, const struct sockaddr_in *client_addr, int sockfd);
void send_packet(Session *sess, uint32_t cmd_id, ProtobufCMessage *msg);

#define SEND(sess, cmd, msg_struct) send_packet(sess, cmd, (ProtobufCMessage *)&msg_struct)

#endif // SESSION_H
