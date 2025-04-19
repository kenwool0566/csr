#include <stdio.h>
#include "shared/cassie.pb-c.h"
#include "gameserver/packet.h"
#include "gameserver/session.h"
#include "gameserver/cmd/avatar.h"
#include "gameserver/cmd/item.h"
#include "gameserver/cmd/lineup.h"
#include "gameserver/cmd/mission.h"
#include "gameserver/cmd/player.h"
#include "gameserver/cmd/scene.h"

void dispatch_command(Session *sess, const DecodedPacket *pkt) {
    switch (pkt->cmd_id) {
        // AVATAR
        case CMD_AVATAR_TYPE__CmdGetAvatarDataCsReq:
            on_get_avatar_data(sess, pkt);
            break;

        // ITEM
        case CMD_ITEM_TYPE__CmdGetBagCsReq:
            on_get_bag(sess, pkt);
            break;

        // LINEUP
        case CMD_LINEUP_TYPE__CmdGetAllLineupDataCsReq:
            on_get_all_lineup_data(sess, pkt);
            break;
        case CMD_LINEUP_TYPE__CmdGetCurLineupDataCsReq:
            on_get_cur_lineup_data(sess, pkt);
            break;

        // MISSION
        case CMD_MISSION_TYPE__CmdGetMissionStatusCsReq:
            on_get_mission_status(sess, pkt);
            break;

        // PLAYER
        case CMD_PLAYER_TYPE__CmdGetBasicInfoCsReq:
            on_get_basic_info(sess, pkt);
            break;
        case CMD_PLAYER_TYPE__CmdGetMultiPathAvatarInfoCsReq:
            on_get_multi_path_avatar_info(sess, pkt);
            break;
        case CMD_PLAYER_TYPE__CmdPlayerGetTokenCsReq:
            on_player_get_token(sess, pkt);
            break;
        case CMD_PLAYER_TYPE__CmdPlayerHeartBeatCsReq:
            on_player_heart_beat(sess, pkt);
            break;
        case CMD_PLAYER_TYPE__CmdPlayerLoginFinishCsReq:
            on_player_login_finish(sess, pkt);
            break;
        case CMD_PLAYER_TYPE__CmdPlayerLoginCsReq:
            on_player_login(sess, pkt);
            break;

        // SCENE
        case CMD_SCENE_TYPE__CmdGetCurSceneInfoCsReq:
            on_get_cur_scene_info(sess, pkt);
            break;
        case CMD_SCENE_TYPE__CmdGetSceneMapInfoCsReq:
            on_get_scene_map_info(sess, pkt);
            break;

        default:
            printf("unhandled: %u\n", pkt->cmd_id);
            break;
    }
}
