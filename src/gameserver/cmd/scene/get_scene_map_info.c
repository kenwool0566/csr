#include "gameserver/packet.h"
#include "gameserver/session.h"
#include "shared/cassie.pb-c.h"

void on_get_scene_map_info(Session *sess, const DecodedPacket *pkt) {
    SceneMapInfo scene_map_info = SCENE_MAP_INFO__INIT;
    scene_map_info.floor_id = (20101 * 1000) + 1;
    scene_map_info.entry_id = (20101 * 100) + 1;
    scene_map_info.cur_map_entry_id = (20101 * 100) + 1;

    SceneMapInfo *scene_map_info_list[1] = { &scene_map_info };

    GetSceneMapInfoScRsp rsp = GET_SCENE_MAP_INFO_SC_RSP__INIT;
    rsp.n_scene_map_info = 1;
    rsp.scene_map_info = scene_map_info_list;

    SEND(sess, CMD_SCENE_TYPE__CmdGetSceneMapInfoScRsp, rsp);
}
