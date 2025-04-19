#ifndef CMD_LINEUP_H
#define CMD_LINEUP_H

#include "gameserver/session.h"
#include "gameserver/packet.h"

void on_get_all_lineup_data(Session *sess, const DecodedPacket *pkt);
void on_get_cur_lineup_data(Session *sess, const DecodedPacket *pkt);

#endif // CMD_LINEUP_H
