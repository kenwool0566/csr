#include "gameserver/packet.h"
#include "gameserver/session.h"
#include "shared/cassie.pb-c.h"

void on_get_basic_info(Session *sess, const DecodedPacket *pkt) {
    GetBasicInfoScRsp rsp = GET_BASIC_INFO_SC_RSP__INIT;
    rsp.isgenderset = 1;

    SEND(sess, CMD_PLAYER_TYPE__CmdGetBasicInfoScRsp, rsp);
}
