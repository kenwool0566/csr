#include <stdlib.h>
#include <microhttpd.h>
#include "shared/cassie.pb-c.h"
#include "shared/config.h"
#include "httpserver/util.h"

// @ /query_dispatch
int on_query_dispatch(struct MHD_Connection *connection) {
    RegionInfo region = REGION_INFO__INIT;
    region.name = "csr";
    region.title = "csr";
    region.dispatch_url = DISPATCH_URL;
    region.env_type = "2";
    region.display_name = "csr";
    region.msg = "OK";

    RegionInfo *region_list[1] = { &region };

    Dispatch dispatch = DISPATCH__INIT;
    dispatch.n_region_list = 1;
    dispatch.region_list = region_list;
    dispatch.msg = "OK";
    dispatch.top_sever_region_name = "csr";

    size_t packed_size = protobuf_c_message_get_packed_size((ProtobufCMessage *)&dispatch);
    uint8_t *packed_data = malloc(packed_size);
    protobuf_c_message_pack((ProtobufCMessage *)&dispatch, packed_data);

    size_t b64_len;
    char *b64_encoded = base64_encode(packed_data, packed_size, &b64_len);

    int result = send_response(connection, b64_encoded, MHD_HTTP_OK);

    free(packed_data);
    free(b64_encoded);

    return result;
}

// @ /query_gateway
int on_query_gateway(struct MHD_Connection *connection) {
    GateServer gateserver = GATE_SERVER__INIT;
    gateserver.use_tcp = USE_TCP;
    gateserver.ip = HOST;
    gateserver.port = GAMESERVER_PORT;

    gateserver.lua_url = LUA_URL;
    gateserver.asset_bundle_url = ASSET_BUNDLE_URL;
    gateserver.ex_resource_url = EX_RESOURCE_URL;
    gateserver.ifix_url = IFIX_URL;
    gateserver.ifix_version = IFIX_VERSION;
    gateserver.mdk_res_version = MDK_RES_VERSION;

    gateserver.enable_design_data_version_update = 1;
    gateserver.enable_version_update = 1;
    gateserver.enable_upload_battle_log = 1;
    gateserver.network_diagnostic = 1;
    gateserver.close_redeem_code = 1;
    gateserver.enable_android_middle_package = 1;
    gateserver.enable_watermark = 1;
    gateserver.event_tracking_open = 1;
    gateserver.enable_save_replay_file = 1;

    size_t packed_size = protobuf_c_message_get_packed_size((ProtobufCMessage *)&gateserver);
    uint8_t *packed_data = malloc(packed_size);
    protobuf_c_message_pack((ProtobufCMessage *)&gateserver, packed_data);

    size_t b64_len;
    char *b64_encoded = base64_encode(packed_data, packed_size, &b64_len);

    int result = send_response(connection, b64_encoded, MHD_HTTP_OK);

    free(packed_data);
    free(b64_encoded);

    return result;
}
