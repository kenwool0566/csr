#ifndef CMD_MISSION_H
#define CMD_MISSION_H

#include "gameserver/session.h"
#include "gameserver/packet.h"

void on_get_mission_status(Session *sess, const DecodedPacket *pkt);

#endif // CMD_MISSION_H
