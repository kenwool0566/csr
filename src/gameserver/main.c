#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "gameserver/udpserver.h"
#include "shared/config.h"

int main() {
    srand((unsigned int)time(NULL));
    start_udp_server(HOST, GAMESERVER_PORT);
}
