#include "gameserver/packet.h"
#include "gameserver/session.h"
#include "gameserver/util.h"
#include "shared/cassie.pb-c.h"

void on_player_login(Session *sess, const DecodedPacket *pkt) {
    PlayerBasicInfo basic_info = PLAYER_BASIC_INFO__INIT;
    basic_info.nickname = "csr";
    basic_info.level = 70;
    basic_info.mcoin = 1;
    basic_info.hcoin = 1;
    basic_info.scoin = 1;
    basic_info.world_level = 6;
    basic_info.stamina = 300;

    PlayerLoginScRsp rsp = PLAYER_LOGIN_SC_RSP__INIT;
    rsp.basic_info = &basic_info;
    rsp.server_timestamp_ms = time_ms_u64();
    rsp.stamina = 300;

    SEND(sess, CMD_PLAYER_TYPE__CmdPlayerLoginScRsp, rsp);
}
