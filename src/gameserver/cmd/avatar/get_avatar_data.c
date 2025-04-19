#include "gameserver/packet.h"
#include "gameserver/session.h"
#include "shared/cassie.pb-c.h"

void on_get_avatar_data(Session *sess, const DecodedPacket *pkt) {
    Avatar yunli = AVATAR__INIT;
    yunli.rank = 6;
    yunli.promotion = 6;
    yunli.level = 80;
    yunli.exp = 0;
    yunli.base_avatar_id = 1221;

    Avatar *avatar_list[1] = { &yunli };

    GetAvatarDataScRsp rsp = GET_AVATAR_DATA_SC_RSP__INIT;
    rsp.n_avatar_list = 1;
    rsp.avatar_list = avatar_list;
    rsp.is_get_all = 1;

    SEND(sess, CMD_AVATAR_TYPE__CmdGetAvatarDataScRsp, rsp);
}
