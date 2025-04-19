#ifndef GS_HANDLER_H
#define GS_HANDLER_H

#include <stdint.h>
#include "gameserver/packet.h"
#include "gameserver/session.h"

void dispatch_command(Session *sess, const DecodedPacket *pkt);

#endif // GS_HANDLER_H
