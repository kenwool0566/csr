#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gameserver/handler.h"
#include "gameserver/packet.h"
#include "shared/cassie.pb-c.h"

// ---------- //
// pidor code //
// ---------- //

// ---------------- CMD_PLAYER_TYPE ----------------

uint8_t *on_player_get_token(const DecodedPacket *pkt, size_t *out_len) {
    PlayerGetTokenScRsp rsp = PLAYER_GET_TOKEN_SC_RSP__INIT;
    rsp.uid = 1;
    rsp.msg = "OK";

    return encode_packet(CMD_PLAYER_TYPE__CmdPlayerGetTokenScRsp, (ProtobufCMessage*)&rsp, out_len);
}

uint8_t *on_player_login(const DecodedPacket *pkt, size_t *out_len) {
    PlayerBasicInfo basic_info = PLAYER_BASIC_INFO__INIT;
    basic_info.nickname = "csr";
    basic_info.level = 10;
    basic_info.mcoin = 1;
    basic_info.hcoin = 1;
    basic_info.scoin = 1;
    basic_info.world_level = 1;
    basic_info.stamina = 300;

    PlayerLoginScRsp rsp = PLAYER_LOGIN_SC_RSP__INIT;
    rsp.basic_info = &basic_info;
    rsp.server_timestamp_ms = (uint64_t)time(NULL) * 1000;
    rsp.stamina = 300;

    return encode_packet(CMD_PLAYER_TYPE__CmdPlayerLoginScRsp, (ProtobufCMessage*)&rsp, out_len);
}

uint8_t *on_player_login_finish(const DecodedPacket *pkt, size_t *out_len) {
    PlayerLoginFinishScRsp rsp = PLAYER_LOGIN_FINISH_SC_RSP__INIT;

    return encode_packet(CMD_PLAYER_TYPE__CmdPlayerLoginFinishScRsp, (ProtobufCMessage*)&rsp, out_len);
}

uint8_t *on_player_heart_beat(const DecodedPacket *pkt, size_t *out_len) {
    PlayerHeartBeatCsReq *req = (PlayerHeartBeatCsReq*)protobuf_c_message_unpack(
        &player_heart_beat_cs_req__descriptor,
        NULL,
        pkt->body_len,
        pkt->body
    );
    
    if (req == NULL) return NULL;

    PlayerHeartBeatScRsp rsp = PLAYER_HEART_BEAT_SC_RSP__INIT;
    rsp.client_time_ms = req->client_time_ms;
    rsp.server_time_ms = (uint64_t)time(NULL) * 1000;

    uint8_t *encoded = encode_packet(CMD_PLAYER_TYPE__CmdPlayerHeartBeatScRsp, (ProtobufCMessage*)&rsp, out_len);
    protobuf_c_message_free_unpacked((ProtobufCMessage*)req, NULL);

    return encoded;
}

uint8_t *on_get_basic_info(const DecodedPacket *pkt, size_t *out_len) {
    GetBasicInfoScRsp rsp = GET_BASIC_INFO_SC_RSP__INIT;
    rsp.isgenderset = 1;
    
    return encode_packet(CMD_PLAYER_TYPE__CmdGetBasicInfoScRsp, (ProtobufCMessage*)&rsp, out_len);
}

uint8_t *on_get_multi_path_avatar_info(const DecodedPacket *pkt, size_t *out_len) {
    GetMultiPathAvatarInfoScRsp rsp = GET_MULTI_PATH_AVATAR_INFO_SC_RSP__INIT;

    return encode_packet(CMD_PLAYER_TYPE__CmdGetMultiPathAvatarInfoScRsp, (ProtobufCMessage*)&rsp, out_len);
}

// ---------------- CMD_AVATAR_TYPE ----------------

uint8_t *on_get_avatar_data(const DecodedPacket *pkt, size_t *out_len) {
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

    return encode_packet(CMD_AVATAR_TYPE__CmdGetAvatarDataScRsp, (ProtobufCMessage*)&rsp, out_len);
}

// ---------------- CMD_LINEUP_TYPE ----------------

uint8_t *on_get_cur_lineup_data(const DecodedPacket *pkt, size_t *out_len) {
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

    GetCurLineupDataScRsp rsp = GET_CUR_LINEUP_DATA_SC_RSP__INIT;
    rsp.lineup = &lineup_info;

    return encode_packet(CMD_LINEUP_TYPE__CmdGetCurLineupDataScRsp, (ProtobufCMessage*)&rsp, out_len);
}

uint8_t *on_get_all_lineup_data(const DecodedPacket *pkt, size_t *out_len) {
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

    return encode_packet(CMD_LINEUP_TYPE__CmdGetAllLineupDataScRsp, (ProtobufCMessage*)&rsp, out_len);
}

// ---------------- CMD_ITEM_TYPE ----------------

uint8_t *on_get_bag(const DecodedPacket *pkt, size_t *out_len) {
    GetBagScRsp rsp = GET_BAG_SC_RSP__INIT;

    return encode_packet(CMD_ITEM_TYPE__CmdGetBagScRsp, (ProtobufCMessage*)&rsp, out_len);
}

// ---------------- CMD_MISSION_TYPE ----------------

// tried parsing from request but didnt work and not gonna bother debugging
static const uint32_t MAIN_MISSION_IDS[] = { 1000101, 1000111, 1000112, 1000113, 1000201, 1000202, 1000203, 1000204, 1000300, 1000301, 1000302, 1000303, 1000304, 1000400, 1000401, 1000402, 1000501, 1000502, 1000503, 1000504, 1000505, 1000510, 1010001, 1010002, 1010101, 1010201, 1010202, 1010204, 1010205, 1010206, 1010301, 1010302, 1010401, 1010402, 1010403, 1010405, 1010501, 1010502, 1010503, 1010601, 1010602, 1010700, 1010701, 1010801, 1010802, 1010901, 1010902, 1011001, 1011002, 1011003, 1011101, 1011102, 1011201, 1011202, 1011301, 1011400, 1011403, 1011501, 1011502, 1011503, 1020101, 1020201, 1020301, 1020302, 1020501, 1020601, 1020701, 1020702, 1020801, 1021001, 1021101, 1021201, 1021501, 1021702, 1030101, 1030201, 1030301, 1030302, 1030303, 1030401, 1030402, 1030501, 1030601, 1030701, 1030702, 1030801, 1030901, 1031001, 1031101, 1031201, 1031301, 1031401, 1031501, 1031601, 1031701, 1031801, 1031901, 1032001, 1032101, 1032201, 1032301, 1032302, 1032401, 1032501, 1032601, 1032602, 1032701, 1032702, 1032703, 1032704, 1032705, 1032801, 1032802, 1032901, 1032902, 1033001, 1033101, 1034101, 1034103, 1034104, 1034105, 1034107, 1034108, 1034109, 1034201, 1034203, 1034204, 1034205, 1034206, 1034207, 1034209, 1034210, 1036001, 1036002, 1036003, 1036102, 1036103, 1036104, 1036106, 1040101, 1040103, 1040104, 1040105, 1040106, 1040108, 1040109, 1040110, 1040111, 1040112, 1040113, 1040114, 1040118, 1040201, 1040203, 1040204, 1040205, 1040206, 1040208, 1040209, 1040211, 1040213, 1040301, 1040302, 1040303, 1040304, 1040305, 1040306, 1040307, 1040308, 1040309, 1040310, 2000100, 2000101, 2000102, 2000103, 2000104, 2000105, 2000108, 2000109, 2000110, 2000111, 2000112, 2000113, 2000116, 2000131, 2000132, 2000201, 2000202, 2000211, 2000301, 2000302, 2000303, 2000304, 2000701, 2000702, 2000704, 2000801, 2000901, 2001001, 2010201, 2010203, 2010206, 2010302, 2010401, 2010402, 2010405, 2010500, 2010501, 2010502, 2010701, 2010702, 2010703, 2010705, 2010706, 2010708, 2010720, 2010735, 2010901, 2010902, 2010904, 2010905, 2010906, 2011101, 2011103, 2011104, 2011105, 2011301, 2011302, 2011400, 2011401, 2011402, 2011403, 2011405, 2011409, 2011411, 2011501, 2011601, 2011901, 2011902, 2011903, 2011905, 2020103, 2020104, 2020201, 2020202, 2020203, 2020301, 2020302, 2020303, 2020305, 2020307, 2020313, 2020401, 2020402, 2020403, 2020404, 2020405, 2020501, 2020507, 2020601, 2020901, 2021001, 2021601, 2021701, 2021703, 2021705, 2021801, 2021803, 2021901, 2022001, 2022002, 2022003, 2022004, 2022005, 2022006, 2022008, 2022101, 2022103, 2022104, 2022105, 2022201, 2022202, 2022203, 2022204, 2022205, 2022206, 2022301, 2022303, 2022304, 2022305, 2022307, 2022308, 2022309, 2030001, 2030101, 2030201, 2030301, 2030401, 2030601, 2040101, 2040201, 2040301, 4010105, 4010106, 4010107, 4010115, 4010116, 4010121, 4010123, 4010125, 4010127, 4010133, 4010134, 4010136, 4010140, 4020101, 4020102, 4020103, 4020104, 4020105, 4020106, 4020107, 4030002, 4030003, 4030004, 4030007, 4030009, 4030010, 4040302, 4040325, 4040334, 4040342, 4050005, 4050007, 4050008, 4050009, 4050010, 4050011, 4050012, 4050013, 4050014, 4050015, 4050016, 4050017, 4050018, 4050019, 4050020, 4050021, 4050022, 4050023, 4050024, 4050025, 4050026, 4050027, 4050028, 4050029, 4050030, 4050031, 4050032, 4050033, 4050034, 4050035, 4050036, 4050037, 4050038, 4050039, 6020101, 6020201, 8000001, 8000102, 8001201, 8001202, 8001203, 8001205, 8001206, 8001207, 8001208, 8001209, 8001211, 8001212, 8001213, 8001215, 8001216, 8001219, 8001220, 8001223, 8001224, 8001226, 8001251, 8001252, 8001253, 8001254, 8001255, 8001263, 8002100, 8002211, 8002221, 8002231, 8003201, 8003202, 8003203, 8003205, 8003206, 8003208, 8003209, 8003211, 8003212, 8003213, 8003215, 8003216, 8003217, 8003218, 8003219, 8003220, 8003240, 8003241, 8003242, 8003243, 8003244, 8003245, 8003246, 8003247, 8003248, 8003249, 8003250, 8003251, 8003280, 8012101, 8012102, 8012103, 8012104, 8012105, 8013101, 8013102, 8013103, 8013104, 8014101, 8014102, 8014103, 8014104, 8014105, 8014106, 8014121, 8014122, 8014123, 8014124, 8014125, 8014126, 8014127, 8014128, 8014129, 8014130, 8014131, 8014132, 8014133, 8014134, 8014135, 8014136, 8014137, 8014138, 8014139, 8014140, 8014161, 8015101, 8015102, 8015103, 8015104, 8015151, 8015152, 8015153, 8015155, 8015157, 8015161, 8015162, 8015163, 8015171, 8015172, 8015173, 8015191, 8015202, 8016101, 8016201, 8016202, 8016203, 8016204, 8016210, 8016301, 8016302, 8017101, 8020219, 8020300, 8020301, 8020302, 8020303, 8020304, 8020305, 8021100, 8021101, 8021102, 8021103, 8021104, 8021106, 8021121, 8021122, 8021123, 8021124, 8021130, 8021131, 8021201, 8021301, 8021403, 8021503, 8022101, 8022102, 8022103, 8022104, 8022108, 8022110, 8022111, 8022112, 8022113, 8022114, 8022115, 8022120, 8022124, 8022201, 8022302, 8022401, 8022402, 8023101, 8023102, 8023103, 8023104, 8023105, 8023111, 8023301, 8023401, 8023701, 8024101, 8024111, 8024112, 8024113, 8024114, 8024115, 8024201, 8024202, 8024203, 8024204, 8024205, 8024206, 8024301, 8024302, 8024303, 8024304, 8024305, 8024306, 8025101, 8025102, 8025103, 8025104, 8025105, 8025111, 8025112, 8025113, 8025114, 8025132, 8025133, 8025134, 8025135, 8025136, 8025137, 8025138, 8025139, 8025201, 8026101, 8026102, 8026103, 8026104, 8026105, 8026110, 8026111, 8026112, 8026113, 8026114, 8026115, 8026116, 8026117, 8026118, 8026201, 8026207, 8026209, 8026301, 8026401, 8027111, 8027119, 8027200, 8027201, 8027202, 8027203, 8027204, 8027205, 8027206, 8027207, 8027208, 8027209, 8030101, 8030300, 8030301, 8030302, 8030303, 8030304, 8030305, 8031101, 8031102, 8031301, 8031401, 8031407, 8031409, 8032100, 8032101, 8032102, 8032103, 8032104, 8032201, 8032301 };
static const size_t MAIN_MISSION_COUNT = sizeof(MAIN_MISSION_IDS) / sizeof(uint32_t);

static const uint32_t SUB_MISSION_IDS[] = { 100020115, 100020116, 100050203, 100050209, 100050223, 100050224, 100050405, 100050406, 100050407, 100050408, 101000106, 101000203, 101000204, 101000206, 101150223, 101150228, 101150229, 101150230, 101150231, 102010104, 102010105, 102010106, 102010107, 102010108, 102010109, 102010110, 102010111, 102010112, 102170219, 103010101, 103020111, 103040105, 103040110, 103040224, 103050101, 103050113, 103050115, 103050117, 103050152, 103060112, 103060129, 103070110, 103110101, 103110107, 103110108, 103110113, 103110151, 103110152, 103110155, 103110156, 103110163, 103110174, 103130107, 103140101, 103140111, 103150128, 103160102, 103160120, 103170102, 103170160, 103230201, 103230204, 103260101, 103260102, 103260202, 103260203, 103260213, 103280102, 103290213, 103290222, 103290225, 103410301, 103410302, 103410501, 103410502, 103410701, 103410728, 103420103, 103420401, 103420415, 103420601, 103420604, 103420901, 103420910, 103421013, 103600201, 103600225, 103610401, 103610410, 104010102, 104010103, 104010314, 104010320, 104010403, 104010502, 104010509, 104010511, 104010514, 104010527, 104010532, 104010548, 104010552, 104010553, 104010559, 104010586, 104010602, 104010605, 104010611, 104010618, 104010624, 104010626, 104010635, 104010640, 104010641, 104010644, 104010921, 104010922, 104010923, 104010937, 104010952, 104010963, 104010964, 104010966, 104010975, 104010976, 104010977, 104010978, 104010979, 104010982, 104011003, 104011016, 104011107, 104011201, 104011210, 104011301, 104011302, 104020301, 104020315, 104020424, 104020432, 104020501, 104020512, 104020513, 104020801, 104020808, 104020816, 104020817, 104020818, 104020820, 104020826, 104021101, 104021118, 104021338, 104021354, 104030108, 104030201, 104030203, 104030204, 104030218, 104030301, 104030310, 104030501, 104030502, 104030503, 104030504, 104030505, 104030506, 104030706, 104030710, 104030724, 104030801, 104030807, 104030904, 104031002, 104031006, 104031007, 104031012, 200020101, 200080104, 200100101, 201020101, 201020102, 201020103, 201020104, 201020129, 201150107, 201150111, 202050702, 202100101, 202100102, 202100103, 202100104, 202100105, 202200601, 202210102, 202210110, 202210415, 202210422, 202210428, 202220201, 202220205, 202220301, 202220311, 202220501, 202220509, 202220603, 202230101, 202230107, 202230301, 202230316, 202230401, 202230501, 202230701, 202230703, 401010503, 401010504, 401010505, 401010506, 401010507, 401010509, 401010510, 401010531, 401010561, 401010563, 401010565, 401010701, 403000300, 404032501, 404032502, 405000502, 405000702, 405000801, 405000802, 405000803, 405000901, 405001001, 405001101, 405001102, 405001103, 405001104, 405001105, 405001201, 405001202, 405001203, 405001204, 405001301, 405001303, 405001401, 405001405, 405001501, 405001502, 405001601, 405001701, 405001801, 405001901, 405002001, 405002003, 405002101, 405002102, 405002201, 405002202, 405002301, 405002302, 405002303, 405002304, 405002305, 405002401, 405002501, 405002502, 405002601, 405002602, 405002701, 405002702, 405002703, 405002801, 405002803, 405002901, 405003001, 405003101, 405003201, 405003301, 405003401, 405003501, 405003601, 405003701, 405003714, 405003801, 405003802, 405003901, 602020101, 800000101, 800000102, 801410213, 801410229, 801410314, 801410317, 801410416, 801410419, 801410519, 801410520, 801520201, 801620102, 801620105, 801620106, 801620108, 801620201, 801620207, 801620304, 801620306, 801620408, 801620412, 801620413, 801620414, 801620415, 801620416, 801620417, 801620418, 801620419, 801620420, 801621001, 801710102, 802030103, 802030203, 802030303, 802030403, 802030505, 802110102, 802110202, 802110208, 802110304, 802110404, 802112103, 802112203, 802112303, 802112401, 802113005, 802113113, 802140303, 802150302, 802210101, 802210311, 802210801, 802212004, 802212404, 802310101, 802310102, 802310202, 802310203, 802310206, 802310303, 802310304, 802310305, 802310403, 802310406, 802310408, 802310503, 802310504, 802310505, 802310506, 802310509, 802310511, 802330102, 802330103, 802340101, 802340110, 802370101, 802420201, 802420301, 802420401, 802420501, 802420601, 802430103, 802430201, 802430301, 802430401, 802430501, 802430601, 802510113, 802510203, 802510204, 802510206, 802510301, 802510308, 802510309, 802510310, 802510403, 802510406, 802510407, 802510408, 802510409, 802510501, 802510502, 802510503, 802510507, 802510509, 802510514, 802511102, 802511103, 802511104, 802511105, 802511201, 802511204, 802511205, 802511206, 802511207, 802511208, 802511209, 802511210, 802511211, 802511213, 802511216, 802511217, 802511221, 802511222, 802511223, 802511301, 802511303, 802511304, 802511306, 802511307, 802511309, 802511311, 802511401, 802511402, 802511404, 802513404, 802610101, 802610203, 802610216, 802610223, 802610224, 802610307, 802610315, 802610403, 802610414, 802611006, 802611503, 802640101, 802640102, 802640103, 802640104, 802640105, 802640106, 802640107, 802640108, 802640109, 802711112, 802720005, 802720102, 802720106, 802720206, 802720210, 802720305, 802720314, 802720404, 802720408, 802720503, 802720507, 802720605, 802720608, 802720702, 802720707, 802720803, 803030102, 803030202, 803030302, 803030402, 803030502, 803030505, 803110124, 803130101, 803130106, 803210001, 803210003, 803210102, 803210103, 803210104, 803210106, 803210107, 803210203, 803210204, 803210209, 803210212, 803210304, 803210306, 803210308, 803210312, 803220201 };
static const size_t SUB_MISSION_COUNT = sizeof(SUB_MISSION_IDS) / sizeof(uint32_t);

uint8_t *on_get_mission_status(const DecodedPacket *pkt, size_t *out_len) {
    GetMissionStatusScRsp rsp = GET_MISSION_STATUS_SC_RSP__INIT;
    rsp.n_finishedmainmissionidlist = MAIN_MISSION_COUNT;
    rsp.finishedmainmissionidlist = malloc(sizeof(uint32_t) * MAIN_MISSION_COUNT);
    memcpy(rsp.finishedmainmissionidlist, MAIN_MISSION_IDS, sizeof(uint32_t) * MAIN_MISSION_COUNT);

    rsp.n_submissionstatuslist = SUB_MISSION_COUNT;
    rsp.submissionstatuslist = malloc(sizeof(Mission*) * SUB_MISSION_COUNT);

    for (size_t i = 0; i < SUB_MISSION_COUNT; ++i) {
        Mission *m = malloc(sizeof(Mission));
        if (m != NULL) {
            *m = (Mission)MISSION__INIT;
            m->id = SUB_MISSION_IDS[i];
            m->progress = 1;
            m->status = MISSION_STATUS__MISSION_FINISH;
            rsp.submissionstatuslist[i] = m;
        }
    }

    uint8_t *encoded = encode_packet(CMD_MISSION_TYPE__CmdGetMissionStatusScRsp, (ProtobufCMessage*)&rsp, out_len);

    free(rsp.finishedmainmissionidlist);
    for (size_t i = 0; i < rsp.n_submissionstatuslist; ++i) {
        free(rsp.submissionstatuslist[i]);
    }
    free(rsp.submissionstatuslist);

    return encoded;
}

// ---------------- CMD_SCENE_TYPE ----------------

uint8_t *on_get_cur_scene_info(const DecodedPacket *pkt, size_t *out_len) {
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

    return encode_packet(CMD_SCENE_TYPE__CmdGetCurSceneInfoScRsp, (ProtobufCMessage*)&rsp, out_len);
}

uint8_t *on_get_scene_map_info(const DecodedPacket *pkt, size_t *out_len) {
    SceneMapInfo scene_map_info = SCENE_MAP_INFO__INIT;
    scene_map_info.floor_id = (20101 * 1000) + 1;
    scene_map_info.entry_id = (20101 * 100) + 1;
    scene_map_info.cur_map_entry_id = (20101 * 100) + 1;

    SceneMapInfo *scene_map_info_list[1] = { &scene_map_info };

    GetSceneMapInfoScRsp rsp = GET_SCENE_MAP_INFO_SC_RSP__INIT;
    rsp.n_scene_map_info = 1;
    rsp.scene_map_info = scene_map_info_list;

    return encode_packet(CMD_SCENE_TYPE__CmdGetSceneMapInfoScRsp, (ProtobufCMessage*)&rsp, out_len);
}

// ---------------- COMMAND SWITCH ----------------

uint8_t *dispatch_command(const DecodedPacket *pkt, size_t *out_len) {
    switch (pkt->cmd_id) {
        // PLAYER
        case CMD_PLAYER_TYPE__CmdPlayerGetTokenCsReq:
            return on_player_get_token(pkt, out_len);
        case CMD_PLAYER_TYPE__CmdPlayerLoginCsReq:
            return on_player_login(pkt, out_len);
        case CMD_PLAYER_TYPE__CmdPlayerLoginFinishCsReq:
            return on_player_login_finish(pkt, out_len);
        case CMD_PLAYER_TYPE__CmdPlayerHeartBeatCsReq:
            return on_player_heart_beat(pkt, out_len);
        case CMD_PLAYER_TYPE__CmdGetBasicInfoCsReq:
            return on_get_basic_info(pkt, out_len);
        case CMD_PLAYER_TYPE__CmdGetMultiPathAvatarInfoCsReq:
            return on_get_multi_path_avatar_info(pkt, out_len);

        // AVATAR
        case CMD_AVATAR_TYPE__CmdGetAvatarDataCsReq:
            return on_get_avatar_data(pkt, out_len);
        
        // LINEUP
        case CMD_LINEUP_TYPE__CmdGetCurLineupDataCsReq:
            return on_get_cur_lineup_data(pkt, out_len);
        case CMD_LINEUP_TYPE__CmdGetAllLineupDataCsReq:
            return on_get_all_lineup_data(pkt, out_len);

        // ITEM
        case CMD_ITEM_TYPE__CmdGetBagCsReq:
            return on_get_bag(pkt, out_len);

        // MISSION
        case CMD_MISSION_TYPE__CmdGetMissionStatusCsReq:
            return on_get_mission_status(pkt, out_len);
        
        // SCENE
        case CMD_SCENE_TYPE__CmdGetCurSceneInfoCsReq:
            return on_get_cur_scene_info(pkt, out_len);
        case CMD_SCENE_TYPE__CmdGetSceneMapInfoCsReq:
            return on_get_scene_map_info(pkt, out_len);

        default:
            return NULL;
    }
}
