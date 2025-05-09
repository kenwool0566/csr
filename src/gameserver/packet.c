#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "gameserver/packet.h"
#include "shared/cassie.pb-c.h"

#define HEAD_MAGIC 0x9D74C714
#define TAIL_MAGIC 0xD7A152C8

DecodedPacket decode_packet(const uint8_t *buffer, size_t len) {
    DecodedPacket result = {0};

    if (len < 16) return result;

    uint32_t head_magic =
        (buffer[0] << 24) |
        (buffer[1] << 16) |
        (buffer[2] << 8) |
        buffer[3];

    if (head_magic != HEAD_MAGIC) return result;

    uint16_t cmd_id = (buffer[4] << 8) | buffer[5];
    uint16_t head_size = (buffer[6] << 8) | buffer[7];
    uint32_t body_size =
        (buffer[8] << 24) |
        (buffer[9] << 16) |
        (buffer[10] << 8) |
        buffer[11];

    size_t body_start = 12 + head_size;
    size_t body_end = body_start + body_size;

    if (body_end + 4 > len) return result;

    uint32_t tail_magic =
        (buffer[body_end] << 24) |
        (buffer[body_end + 1] << 16) |
        (buffer[body_end + 2] << 8) |
        buffer[body_end + 3];

    if (tail_magic != TAIL_MAGIC) return result;

    result.cmd_id = cmd_id;
    result.body = buffer + body_start;
    result.body_len = body_size;
    result.valid = 1;

    return result;
}

uint8_t HEAD_MAGIC_BYTES[4] = {0x9D, 0x74, 0xC7, 0x14};
uint8_t TAIL_MAGIC_BYTES[4] = {0xD7, 0xA1, 0x52, 0xC8};

uint8_t *encode_packet(uint16_t cmd_id, const ProtobufCMessage *msg, size_t *packet_len_out) {
    size_t body_len = protobuf_c_message_get_packed_size(msg);
    size_t packet_len = 12 + body_len + 4;
    uint8_t *buffer = malloc(packet_len);
    if (!buffer) return NULL;

    size_t offset = 0;
    memcpy(buffer + offset, HEAD_MAGIC_BYTES, 4);
    offset += 4;

    buffer[offset++] = (cmd_id >> 8) & 0xFF;
    buffer[offset++] = cmd_id & 0xFF;

    buffer[offset++] = 0;
    buffer[offset++] = 0;

    buffer[offset++] = (body_len >> 24) & 0xFF;
    buffer[offset++] = (body_len >> 16) & 0xFF;
    buffer[offset++] = (body_len >> 8) & 0xFF;
    buffer[offset++] = body_len & 0xFF;

    protobuf_c_message_pack(msg, buffer + offset);
    offset += body_len;

    memcpy(buffer + offset, TAIL_MAGIC_BYTES, 4);

    if (packet_len_out) *packet_len_out = packet_len;
    return buffer;
}

NetOperation netop_from_bytes(const uint8_t *buffer) {
    NetOperation op = {0};

    op.head = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
    op.conv_id = (buffer[4] << 24) | (buffer[5] << 16) | (buffer[6] << 8) | buffer[7];
    op.token = (buffer[8] << 24) | (buffer[9] << 16) | (buffer[10] << 8) | buffer[11];
    op.data = (buffer[12] << 24) | (buffer[13] << 16) | (buffer[14] << 8) | buffer[15];
    op.tail = (buffer[16] << 24) | (buffer[17] << 16) | (buffer[18] << 8) | buffer[19];

    return op;
}

uint8_t *netop_to_bytes(const NetOperation *op) {
    if (!op) return NULL;

    uint8_t *buffer = malloc(20);
    if (!buffer) return NULL;

    buffer[0]  = (op->head >> 24) & 0xFF;
    buffer[1]  = (op->head >> 16) & 0xFF;
    buffer[2]  = (op->head >> 8)  & 0xFF;
    buffer[3]  = op->head & 0xFF;

    buffer[4]  = (op->conv_id >> 24) & 0xFF;
    buffer[5]  = (op->conv_id >> 16) & 0xFF;
    buffer[6]  = (op->conv_id >> 8)  & 0xFF;
    buffer[7]  = op->conv_id & 0xFF;

    buffer[8]  = (op->token >> 24) & 0xFF;
    buffer[9]  = (op->token >> 16) & 0xFF;
    buffer[10] = (op->token >> 8)  & 0xFF;
    buffer[11] = op->token & 0xFF;

    buffer[12] = (op->data >> 24) & 0xFF;
    buffer[13] = (op->data >> 16) & 0xFF;
    buffer[14] = (op->data >> 8)  & 0xFF;
    buffer[15] = op->data & 0xFF;

    buffer[16] = (op->tail >> 24) & 0xFF;
    buffer[17] = (op->tail >> 16) & 0xFF;
    buffer[18] = (op->tail >> 8)  & 0xFF;
    buffer[19] = op->tail & 0xFF;

    return buffer;
}

NetOperation create_netop(uint32_t head, uint32_t conv_id, uint32_t token, uint32_t data, uint32_t tail) {
    NetOperation op;

    op.head = head;
    op.conv_id = conv_id;
    op.token = token;
    op.data = data;
    op.tail = tail;

    return op;
}
