#ifndef GS_HANDLER_H
#define GS_HANDLER_H

#include <stdint.h>
#include "gameserver/packet.h"

uint8_t *dispatch_command(const DecodedPacket *pkt, size_t *out_len);

#endif // GS_HANDLER_H
