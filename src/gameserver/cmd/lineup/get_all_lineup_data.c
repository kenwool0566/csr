#include "gameserver/packet.h"
#include "gameserver/session.h"
#include "shared/cassie.pb-c.h"

void on_get_all_lineup_data(Session *sess, const DecodedPacket *pkt) {
    SpBarInfo sp_bar_info = SP_BAR_INFO__INIT;
    sp_bar_info.cur_sp = 10000;
    sp_bar_info.max_sp = 10000;

    LineupAvatar lineup_avatar = LINEUP_AVATAR__INIT;
    lineup_avatar.id = 1221;
    lineup_avatar.hp = 10000;
    lineup_avatar.avatar_type = AVATAR_TYPE__AVATAR_FORMAL_TYPE;
    lineup_avatar.sp_bar = &sp_bar_info;

    LineupAvatar *avatar_list[1] = { &lineup_avatar };

    LineupInfo lineup_info = LINEUP_INFO__INIT;
    lineup_info.name = "csr";
    lineup_info.n_avatar_list = 1;
    lineup_info.avatar_list = avatar_list;

    LineupInfo *lineup_list[1] = { &lineup_info };

    GetAllLineupDataScRsp rsp = GET_ALL_LINEUP_DATA_SC_RSP__INIT;
    rsp.n_lineup_list = 1;
    rsp.lineup_list = lineup_list;

    SEND(sess, CMD_LINEUP_TYPE__CmdGetAllLineupDataScRsp, rsp);
}
