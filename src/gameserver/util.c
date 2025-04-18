#include <stdlib.h>
#include <stdint.h>
#include <time.h>

uint32_t rand_u32() {
    return ((uint32_t)rand() << 16) | (uint32_t)rand();
}

uint32_t time_ms_u32() {
    return (uint32_t)time(NULL) * 1000;
}
