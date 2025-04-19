#include "gameserver/packet.h"
#include "gameserver/session.h"
#include "shared/cassie.pb-c.h"

void on_get_cur_scene_info(Session *sess, const DecodedPacket *pkt) {
    Vector player_rot = VECTOR__INIT;
    Vector player_pos = VECTOR__INIT;
    player_pos.x = -550;
    player_pos.y = 19364;
    player_pos.z = 4480;

    MotionInfo player_motion = MOTION_INFO__INIT;
    player_motion.pos = &player_pos;
    player_motion.rot = &player_rot;

    SceneActorInfo player_actor = SCENE_ACTOR_INFO__INIT;
    player_actor.avatar_type = AVATAR_TYPE__AVATAR_FORMAL_TYPE;
    player_actor.base_avatar_id = 1221;
    player_actor.map_layer = 2;
    player_actor.uid = 1;

    SceneEntityInfo player_entity = SCENE_ENTITY_INFO__INIT;
    player_entity.entity_case__case = SCENE_ENTITY_INFO__ENTITY_CASE__ACTOR;
    player_entity.actor = &player_actor;
    player_entity.motion = &player_motion;

    SceneEntityInfo *entity_list[1] = { &player_entity };
    
    SceneEntityGroupInfo scene_entity_group_info = SCENE_ENTITY_GROUP_INFO__INIT;
    scene_entity_group_info.state = 1;
    scene_entity_group_info.n_entity_list = 1;
    scene_entity_group_info.entity_list = entity_list;

    SceneEntityGroupInfo *entity_group_list[1] = { &scene_entity_group_info };

    SceneInfo scene_info = SCENE_INFO__INIT;
    scene_info.plane_id = 20101;
    scene_info.entry_id = (20101 * 100) + 1;
    scene_info.floor_id = (20101 * 1000) + 1;
    scene_info.game_mode_type = 2;
    scene_info.n_entity_group_list = 1;
    scene_info.entity_group_list = entity_group_list;
    
    GetCurSceneInfoScRsp rsp = GET_CUR_SCENE_INFO_SC_RSP__INIT;
    rsp.scene = &scene_info;

    SEND(sess, CMD_SCENE_TYPE__CmdGetCurSceneInfoScRsp, rsp);
}
