#include "gameserver/packet.h"
#include "gameserver/session.h"
#include "shared/cassie.pb-c.h"

void on_get_multi_path_avatar_info(Session *sess, const DecodedPacket *pkt) {
    GetMultiPathAvatarInfoScRsp rsp = GET_MULTI_PATH_AVATAR_INFO_SC_RSP__INIT;

    SEND(sess, CMD_PLAYER_TYPE__CmdGetMultiPathAvatarInfoScRsp, rsp);
}
