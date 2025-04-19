#include "gameserver/packet.h"
#include "gameserver/session.h"
#include "shared/cassie.pb-c.h"

void on_player_login_finish(Session *sess, const DecodedPacket *pkt) {
    PlayerLoginFinishScRsp rsp = PLAYER_LOGIN_FINISH_SC_RSP__INIT;

    SEND(sess, CMD_PLAYER_TYPE__CmdPlayerLoginFinishScRsp, rsp);
}
