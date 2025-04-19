#ifndef CMD_ITEM_H
#define CMD_ITEM_H

#include "gameserver/session.h"
#include "gameserver/packet.h"

void on_get_bag(Session *sess, const DecodedPacket *pkt);

#endif // CMD_ITEM_H
