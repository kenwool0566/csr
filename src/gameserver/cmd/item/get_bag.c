#include "gameserver/packet.h"
#include "gameserver/session.h"
#include "shared/cassie.pb-c.h"

void on_get_bag(Session *sess, const DecodedPacket *pkt) {
    GetBagScRsp rsp = GET_BAG_SC_RSP__INIT;

    SEND(sess, CMD_ITEM_TYPE__CmdGetBagScRsp, rsp);
}
