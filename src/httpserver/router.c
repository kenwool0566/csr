#include <string.h>
#include <microhttpd.h>
#include "httpserver/authhandler.h"
#include "httpserver/dispatchhandler.h"
#include "httpserver/util.h"

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
