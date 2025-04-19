#include "gameserver/packet.h"
#include "gameserver/session.h"
#include "shared/cassie.pb-c.h"

void on_player_get_token(Session *sess, const DecodedPacket *pkt) {
    PlayerGetTokenScRsp rsp = PLAYER_GET_TOKEN_SC_RSP__INIT;
    rsp.uid = 1;
    rsp.msg = "OK";

    SEND(sess, CMD_PLAYER_TYPE__CmdPlayerGetTokenScRsp, rsp);
}
