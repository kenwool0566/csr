#ifndef NETPACKET_H
#define NETPACKET_H

#include <stdint.h>
#include <stddef.h>
#include <protobuf-c/protobuf-c.h>

typedef struct {
    uint16_t cmd_id;
    const uint8_t *body;
    size_t body_len;
    int valid; // 1 valid, 0 invalid
} DecodedPacket;

DecodedPacket decode_packet(const uint8_t *buffer, size_t len);
uint8_t *encode_packet(uint16_t cmd_id, const ProtobufCMessage *msg, size_t *packet_len_out);

#endif // NETPACKET_H
