#ifndef HANDLER_H
#define HANDLER_H

#include "gameserver/packet.h"
#include "gameserver/session.h"

void dispatch_command(Session *sess, const DecodedPacket *pkt);

#endif // HANDLER_H
