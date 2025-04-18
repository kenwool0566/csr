#include <microhttpd.h>
#include "httpserver/util.h"

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
