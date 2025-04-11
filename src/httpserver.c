#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <microhttpd.h>

#include "cassie.pb-c.h"
#include "base64.h"

// server config
#define HOST "127.0.0.1"
#define PORT 21000
#define USE_TCP 1
#define GAMESERVER_PORT 23301
#define DISPATCH_URL "http://127.0.0.1:21000/query_gateway"

// hotfix
#define LUA_URL ""
#define ASSET_BUNDLE_URL ""
#define EX_RESOURCE_URL ""
#define IFIX_URL ""
#define IFIX_VERSION "0"
#define MDK_RES_VERSION ""

// helper function to send responses
enum MHD_Result send_response(
    struct MHD_Connection *connection,
    const char *msg,
    int status_code
) {
    struct MHD_Response *response;
    response = MHD_create_response_from_buffer(
        strlen(msg), (void*)msg, MHD_RESPMEM_MUST_COPY
    );

    if (!response) return MHD_NO;

    MHD_add_response_header(response, "Content-Type", "text/plain");
    int ret = MHD_queue_response(connection, status_code, response);
    MHD_destroy_response(response);
    return ret;
}

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
    // dispatch.region_list = malloc(sizeof(RegionInfo*));
    // dispatch.region_list[0] = &region;
    dispatch.region_list = region_list;
    dispatch.msg = "OK";
    dispatch.top_sever_region_name = "csr";

    size_t packed_size = protobuf_c_message_get_packed_size((ProtobufCMessage*)&dispatch);
    uint8_t *packed_data = malloc(packed_size);
    protobuf_c_message_pack((ProtobufCMessage*)&dispatch, packed_data);

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

    size_t packed_size = protobuf_c_message_get_packed_size((ProtobufCMessage*)&gateserver);
    uint8_t *packed_data = malloc(packed_size);
    protobuf_c_message_pack((ProtobufCMessage*)&gateserver, packed_data);

    size_t b64_len;
    char *b64_encoded = base64_encode(packed_data, packed_size, &b64_len);

    int result = send_response(connection, b64_encoded, MHD_HTTP_OK);

    free(packed_data);
    free(b64_encoded);

    return result;
}

// @ /account/risky/api/check
int on_risky_check(struct MHD_Connection *connection) {
    const char *response = "{\"data\":{},\"message\":\"OK\",\"retcode\":0}";

    return send_response(connection, response, MHD_HTTP_OK);
}

// @ /hkrpg_global/mdk/shield/api/login
// @ /hkrpg_global/mdk/shield/api/verify
int on_mdk_shield_api(struct MHD_Connection *connection) {
    const char *response = "{\"data\":{\"account\":{\"area_code\":\"**\",\"country\":\"RU\",\"email\":\"pidor@mail.com\",\"is_email_verify\":\"1\",\"token\":\"C\",\"uid\":\"1\"},\"device_grant_required\":false,\"reactivate_required\":false,\"realperson_required\":false,\"safe_mobile_required\":false},\"message\":\"OK\",\"retcode\":0}";

    return send_response(connection, response, MHD_HTTP_OK);
}

// @ /hkrpg_global/combo/granter/login/v2/login
int on_combo_granter_login(struct MHD_Connection *connection) {
    const char *response = "{\"data\":{\"account_type\":1,\"combo_id\":\"1\",\"combo_token\":\"C\",\"data\":\"{\\\"guest\\\":false}\",\"heartbeat\":false,\"open_id\":\"1\"},\"message\":\"OK\",\"retcode\":0}";

    return send_response(connection, response, MHD_HTTP_OK);
}

enum MHD_Result route_request(
    void *cls,
    struct MHD_Connection *connection,
    const char *url,
    const char *method,
    const char *version,
    const char *upload_data,
    size_t *upload_data_size,
    void **con_cls
) {
    if (*con_cls == NULL) { 
        // enable Keep-Alive
        *con_cls = (void *)1;
        return MHD_YES;
    }

    if (strcmp(method, "GET") == 0) {
        if (strcmp(url, "/query_dispatch") == 0) {
            return on_query_dispatch(connection);
        } else if (strcmp(url, "/query_gateway") == 0) {
            return on_query_gateway(connection);
        }
    } else if (strcmp(method, "POST") == 0) {
        // consume POST body
        // if we don't do this, microhttpd whines and closes connection
        if (*upload_data_size != 0) {
            *upload_data_size = 0;
            return MHD_YES;
        }
        if (strcmp(url, "/account/risky/api/check") == 0) {
            return on_risky_check(connection);
        } else if (strcmp(url, "/hkrpg_global/mdk/shield/api/login") == 0) {
            return on_mdk_shield_api(connection);
        } else if (strcmp(url, "/hkrpg_global/mdk/shield/api/verify") == 0) {
            return on_mdk_shield_api(connection);
        } else if (strcmp(url, "/hkrpg_global/combo/granter/login/v2/login") == 0) {
            return on_combo_granter_login(connection);
        }
    }

    return send_response(connection, "NOT FOUND", MHD_HTTP_NOT_FOUND);
}

int main() {
    struct MHD_Daemon *daemon;
    daemon = MHD_start_daemon(
        MHD_USE_SELECT_INTERNALLY, // thread
        PORT,
        NULL, // accept all
        NULL,
        &route_request, // router
        NULL, // shared state
        MHD_OPTION_CONNECTION_TIMEOUT, 10, // keep idle conn for 10s
        MHD_OPTION_END // sentinel
    );

    if (NULL == daemon) {
        fprintf(stderr, "failed to start the server, maybe port is already used?\n");
        return 1;
    }

    printf(
        "httpserver listening on localhost with port %d...\npress enter to stop the server", 
        PORT
    );
    getchar();

    MHD_stop_daemon(daemon);
    return 0;
}
