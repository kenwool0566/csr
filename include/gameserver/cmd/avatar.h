#ifndef CMD_AVATAR_H
#define CMD_AVATAR_H

#include "gameserver/session.h"
#include "gameserver/packet.h"

void on_get_avatar_data(Session *sess, const DecodedPacket *pkt);

#endif // CMD_AVATAR_H
