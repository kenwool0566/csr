#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <microhttpd.h>

// helper function to send responses (plaintext)
enum MHD_Result send_response(
    struct MHD_Connection *connection,
    const char *msg,
    uint status_code
) {
    struct MHD_Response *response = MHD_create_response_from_buffer(
        strlen(msg),
        (void *)msg,
        MHD_RESPMEM_MUST_COPY
    );

    if (!response) return MHD_NO;

    MHD_add_response_header(response, "Content-Type", "text/plain");
    int ret = MHD_queue_response(connection, status_code, response);
    MHD_destroy_response(response);
    return ret;
}

// self explanatory
static const char base64_table[] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char *base64_encode(const uint8_t *data, size_t input_len, size_t *output_len) {
    size_t out_len = 4 * ((input_len + 2) / 3);
    char *encoded = malloc(out_len + 1);
    if (!encoded) return NULL;

    size_t i, j;
    for (i = 0, j = 0; i < input_len;) {
        size_t rem = input_len - i;

        uint32_t octet_a = i < input_len ? data[i++] : 0;
        uint32_t octet_b = i < input_len ? data[i++] : 0;
        uint32_t octet_c = i < input_len ? data[i++] : 0;

        uint32_t triple = (octet_a << 16) | (octet_b << 8) | octet_c;

        encoded[j++] = base64_table[(triple >> 18) & 0x3F];
        encoded[j++] = base64_table[(triple >> 12) & 0x3F];

        if (rem > 1) {
            encoded[j++] = base64_table[(triple >> 6) & 0x3F];
        } else {
            encoded[j++] = '=';
        }

        if (rem > 2) {
            encoded[j++] = base64_table[triple & 0x3F];
        } else {
            encoded[j++] = '=';
        }
    }

    encoded[out_len] = '\0';
    if (output_len) *output_len = out_len;
    return encoded;
}
