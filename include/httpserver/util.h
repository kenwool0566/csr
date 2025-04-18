#ifndef UTIL_H
#define UTIL_H

#include <microhttpd.h>

enum MHD_Result send_response(struct MHD_Connection *connection, const char *msg, uint status_code);
char *base64_encode(const uint8_t *data, size_t input_len, size_t *output_len);

#endif
