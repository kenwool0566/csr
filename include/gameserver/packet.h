#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <stddef.h>
#include "shared/cassie.pb-c.h"

typedef struct {
    uint16_t cmd_id;
    const uint8_t *body;
    size_t body_len;
    int valid; // 1 = valid/succ, 0 = fucked up
} DecodedPacket;

DecodedPacket decode_packet(const uint8_t *buffer, size_t len);
uint8_t *encode_packet(uint16_t cmd_id, const ProtobufCMessage *msg, size_t *packet_len_out);

typedef struct {
    uint32_t head;
    uint32_t conv_id;
    uint32_t token;
    uint32_t data;
    uint32_t tail;
    // int valid;
} NetOperation;

NetOperation netop_from_bytes(const uint8_t *buffer);
uint8_t *netop_to_bytes(const NetOperation *op);
NetOperation create_netop(uint32_t head, uint32_t conv_id, uint32_t token, uint32_t data, uint32_t tail);

#endif // PACKET_H
