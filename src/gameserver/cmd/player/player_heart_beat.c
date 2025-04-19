#include "gameserver/packet.h"
#include "gameserver/session.h"
#include "gameserver/util.h"
#include "shared/cassie.pb-c.h"

void on_player_heart_beat(Session *sess, const DecodedPacket *pkt) {
    PlayerHeartBeatCsReq *req = (PlayerHeartBeatCsReq*)protobuf_c_message_unpack(
        &player_heart_beat_cs_req__descriptor,
        NULL,
        pkt->body_len,
        pkt->body
    );

    if (req == NULL) return;

    PlayerHeartBeatScRsp rsp = PLAYER_HEART_BEAT_SC_RSP__INIT;
    rsp.client_time_ms = req->client_time_ms;
    rsp.server_time_ms = time_ms_u64();

    SEND(sess, CMD_PLAYER_TYPE__CmdPlayerHeartBeatScRsp, rsp);

    protobuf_c_message_free_unpacked((ProtobufCMessage*)req, NULL);
}
