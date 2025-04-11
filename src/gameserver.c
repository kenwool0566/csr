#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#include "netpacket.h"
#include "cassie.pb-c.h"
#include "gshandler.h"

// make sure this matches in httpserver
#define PORT 23301
#define BUFFER_SIZE 1024

void *handle_client(void *arg) {
    int client_sock = *(int*)arg;
    free(arg); // cleanup pointer passed from accept
    uint8_t buffer[BUFFER_SIZE];
    ssize_t received;

    while ((received = recv(client_sock, buffer, BUFFER_SIZE, 0)) > 0) {
        DecodedPacket pkt = decode_packet(buffer, received);
        if (!pkt.valid) continue; // we skip if invalid

        size_t packet_len = 0;
        uint8_t *encoded = dispatch_command(&pkt, &packet_len);

        if (encoded) { // also skip if NULL
            printf("handled cmd: %u\n", pkt.cmd_id);
            send(client_sock, encoded, packet_len, 0);
            free(encoded);
        } else {
            printf("unhandled: %u\n", pkt.cmd_id);
        }
    }

    close(client_sock);
    return NULL;
}

int main() {
    // tcp sock
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "failed to start the server, maybe port is already used?\n");
        close(server_sock);
        return 1;
    }
    // up to 2 clients in backlog
    listen(server_sock, 2);

    printf("gameserver listening on localhost with port %d...\npress ctrl+c to stop the server", PORT);
    fflush(stdout);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int *client_sock = malloc(sizeof(int));

        *client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
        if (*client_sock < 0) {
            perror("failed to accept client");
            free(client_sock);
            continue;
        }

        // handle each client in its own thread
        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, client_sock) != 0) {
            perror("pthread_create failed");
            close(*client_sock);
            free(client_sock);
            continue;
        }
        pthread_detach(tid);
    }

    close(server_sock);
    return 0;
}
