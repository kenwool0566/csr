#ifndef DISPATCHHANDLER_H
#define DISPATCHHANDLER_H

#include <microhttpd.h>

int on_query_dispatch(struct MHD_Connection *connection);
int on_query_gateway(struct MHD_Connection *connection);

#endif // DISPATCHHANDLER_H
