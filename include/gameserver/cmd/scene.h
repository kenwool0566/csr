#ifndef CMD_SCENE_H
#define CMD_SCENE_H

#include "gameserver/session.h"
#include "gameserver/packet.h"

void on_get_scene_map_info(Session *sess, const DecodedPacket *pkt);
void on_get_cur_scene_info(Session *sess, const DecodedPacket *pkt);

#endif // CMD_SCENE_H
