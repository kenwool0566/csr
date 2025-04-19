#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "gameserver/session.h"
#include "gameserver/ikcp.h"
#include "gameserver/packet.h"
#include "gameserver/util.h"
#include "gameserver/handler.h"
#include "shared/uthash.h"
#include "shared/cassie.pb-c.h"

typedef struct {
    uint32_t conv_id;
    Session *session;
    UT_hash_handle hh;
} SessionEntry;

static SessionEntry *sessions = NULL;
static uint32_t global_conv_id = 0;

uint32_t next_conv_id() {
    return ++global_conv_id;
}

void add_session_entry(uint32_t conv_id, Session *sess) {
    SessionEntry *entry = malloc(sizeof(SessionEntry));
    entry->conv_id = conv_id;
    entry->session = sess;
    HASH_ADD_INT(sessions, conv_id, entry);
}

Session *get_session_entry(uint32_t conv_id) {
    SessionEntry *entry;
    HASH_FIND_INT(sessions, &conv_id, entry);
    return entry ? entry->session : NULL;
}

int kcp_output_callback(const char *buf, int len, ikcpcb *kcp, void *user) {
    Session *sess = (Session *)user;

    ssize_t sent = sendto(
        sess->sockfd,
        buf,
        len,
        0,
        (struct sockaddr *)&sess->addr,
        sizeof(sess->addr)
    );

    if (sent < 0) {
        fprintf(stderr, "sendto failed\n");
        return -1;
    }

    return 0;
}

Session *create_session(const struct sockaddr_in *addr, int sockfd, uint32_t conv_id, uint32_t token) {
    Session *sess = malloc(sizeof(Session));
    if (!sess) return NULL;

    memcpy(&sess->addr, addr, sizeof(struct sockaddr_in));
    sess->sockfd = sockfd;
    sess->conv_id = conv_id;
    sess->token = token;
    sess->kcp = ikcp_create(conv_id, token, (void *)sess);
    ikcp_nodelay(sess->kcp, 1, 10, 2, 1);
    sess->kcp->output = kcp_output_callback;

    return sess;
}

void free_session(Session *sess) {
    if (sess) {
        free(sess);
    }
}

void drop_session_entry(uint32_t conv_id) {
    SessionEntry *entry;
    HASH_FIND_INT(sessions, &conv_id, entry);
    if (entry) {
        HASH_DEL(sessions, entry);
        free_session(entry->session);
        free(entry);
    }
}

void establish_session(uint32_t data, const struct sockaddr_in *client_addr, int sockfd) {
    uint32_t conv_id = next_conv_id();
    uint32_t token = rand_u32();

    Session *sess = create_session(client_addr, sockfd, conv_id, token);
    add_session_entry(conv_id, sess);

    NetOperation net_op = create_netop(0x145, conv_id, token, data, 0x14514545);

    uint8_t *buffer = netop_to_bytes(&net_op);

    if (buffer == NULL) {
        fprintf(stderr, "netop to bytes failed\n");
        return;
    }

    ssize_t sent = sendto(
        sockfd,
        buffer,
        20,
        0,
        (struct sockaddr *)client_addr,
        sizeof(*client_addr)
    );

    if (sent < 0) {
        fprintf(
            stderr, "establish session(%u) for %s:%d failed\n",
            conv_id,
            inet_ntoa(client_addr->sin_addr),
            ntohs(client_addr->sin_port)
        );
    } else {
        printf(
            "new session(%u), %s:%d\n",
            conv_id,
            inet_ntoa(client_addr->sin_addr),
            ntohs(client_addr->sin_port)
        );
    }

    free(buffer);
}

// void drop_session(uint32_t conv_id, uint32_t token, const struct sockaddr_in *client_addr) {
//     Session *sess = get_session_entry(conv_id);

//     if (!sess) {
//         fprintf(stderr, "no session found for conv=%u\n", conv_id);
//         return;
//     }

//     if (sess->token != token) {
//         fprintf(stderr,
//             "token mismatch for conv=%u from %s:%d (expected: 0x%X, got: 0x%X)\n",
//             conv_id,
//             inet_ntoa(client_addr->sin_addr),
//             ntohs(client_addr->sin_port),
//             sess->token,
//             token
//         );
//         return;
//     }

//     printf("dropping session conv=%u from %s:%d\n",
//         conv_id,
//         inet_ntoa(sess->addr.sin_addr),
//         ntohs(sess->addr.sin_port)
//     );

//     drop_session_entry(conv_id);
// }

void process_net_operation(const uint8_t *data, const struct sockaddr_in *client_addr, int sockfd) {
    NetOperation op = netop_from_bytes(data);

    if (op.head == 0xFF && op.tail == 0xFFFFFFFF) {
        establish_session(op.data, client_addr, sockfd);
    } else if (op.head == 0x194 && op.tail == 0x19419494) {
        // this is never reached for some reason so we'll drop in the logout cmd instead
        // drop_session(op.conv_id, op.token, client_addr);
        printf("this message should never appear i suppose!");
    } else {
        fprintf(stderr, "unknown magic pair: 0x%X <-> 0x%X\n", op.head, op.tail);
    }
}

void kcp_send_flush_update(ikcpcb *kcp, const char *data, int len) {
    int sent = ikcp_send(kcp, data, len);
    if (sent < 0) {
        fprintf(stderr, "kcp send failed\n");
        return;
    }

    ikcp_flush(kcp);
    ikcp_update(kcp, (uint32_t)time_ms_u64());
}

void send_packet(Session *sess, uint32_t cmd_id, ProtobufCMessage *msg) {
    if (!sess || !sess->kcp || !msg) return;

    size_t len = 0;
    uint8_t *data = encode_packet(cmd_id, msg, &len);
    if (!data) return;

    kcp_send_flush_update(sess->kcp, (char *)data, (int)len);
    free(data);
    printf("sent cmd: %u\n", cmd_id);
}

void process_kcp_payload(const uint8_t *data, ssize_t len, const struct sockaddr_in *client_addr, int sockfd) {
    uint32_t conv_id = ikcp_getconv(data);
    Session *sess = get_session_entry(conv_id);
    
    if (!sess) {
        fprintf(
            stderr, "got kcp with unk conv_id %u from %s:%d\n",
            conv_id,
            inet_ntoa(client_addr->sin_addr),
            ntohs(client_addr->sin_port)
        );
        return;
    }

    ikcpcb *kcp = sess->kcp;

    int input_result = ikcp_input(kcp, (const char *)data, len);

    if (input_result < 0) {
        fprintf(stderr, "kcp input len %lu failed for session(%u)\n", len,conv_id);
        return;
    }

    ikcp_update(kcp, (uint32_t)time_ms_u64());

    char buffer[1024];
    int recv_len;
    
    while ((recv_len = ikcp_recv(kcp, buffer, sizeof(buffer))) > 0) {
        // printf("kcp received from session(%u) with len %d\n", conv_id, recv_len);
        DecodedPacket pkt = decode_packet(buffer, recv_len);
        if (!pkt.valid) continue;
    
        if (pkt.cmd_id == CMD_PLAYER_TYPE__CmdPlayerLogoutCsReq) {
            printf("session(%u) logged out\n", conv_id);
            drop_session_entry(conv_id);
            printf("dropped session(%u)\n", conv_id);
            break;
        }
        
        printf("got cmd: %u\n", pkt.cmd_id);
        dispatch_command(sess, &pkt);
    }

    ikcp_update(kcp, (uint32_t)time_ms_u64());
}
