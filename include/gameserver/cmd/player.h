#ifndef CMD_PLAYER_H
#define CMD_PLAYER_H

#include "gameserver/session.h"
#include "gameserver/packet.h"

void on_get_basic_info(Session *sess, const DecodedPacket *pkt);
void on_player_login_finish(Session *sess, const DecodedPacket *pkt);
void on_get_multi_path_avatar_info(Session *sess, const DecodedPacket *pkt);
void on_player_get_token(Session *sess, const DecodedPacket *pkt);
void on_player_login(Session *sess, const DecodedPacket *pkt);
void on_player_heart_beat(Session *sess, const DecodedPacket *pkt);

#endif // CMD_PLAYER_H
