#include <stdlib.h>
#include <stdint.h>
#include <time.h>

uint32_t rand_u32() {
    return ((uint32_t)rand() << 16) | (uint32_t)rand();
}

uint64_t time_ms_u64() {
    return ((uint64_t)time(NULL)) * 1000;
}
