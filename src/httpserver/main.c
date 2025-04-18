#include <stdio.h>
#include <string.h>
#include <microhttpd.h>
#include "httpserver/router.h"
#include "shared/config.h"

int main(void) {
    struct MHD_Daemon *daemon;
    daemon = MHD_start_daemon(
        MHD_USE_SELECT_INTERNALLY, // thread
        HTTPSERVER_PORT,
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

    printf("httpserver listening on %s:%d\npress enter to stop", HOST, HTTPSERVER_PORT);

    getchar();
    MHD_stop_daemon(daemon);
    
    return 0;
}
