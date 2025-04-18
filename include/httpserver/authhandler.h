#ifndef AUTHHANDLER_H
#define AUTHHANDLER_H

#include <microhttpd.h>

int on_risky_check(struct MHD_Connection *connection);
int on_mdk_shield_api(struct MHD_Connection *connection);
int on_combo_granter_login(struct MHD_Connection *connection);

#endif // AUTHHANDLER_H
