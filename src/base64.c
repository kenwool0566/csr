#include <stdlib.h>
#include <stdint.h>

static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

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
