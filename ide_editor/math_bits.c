#include "math_bits.h"
#include <stdio.h>

void print_math_limits(void) {
    printf("8-bit:   0 to %u (unsigned), %d to %d (signed)\n", MAX_UINT8, MIN_INT8, MAX_INT8);
    printf("16-bit:  0 to %u (unsigned), %d to %d (signed)\n", MAX_UINT16, MIN_INT16, MAX_INT16);
    printf("32-bit:  0 to %u (unsigned), %d to %d (signed)\n", MAX_UINT32, MIN_INT32, MAX_INT32);
    printf("64-bit:  0 to %llu (unsigned), %lld to %lld (signed)\n",
        (unsigned long long)MAX_UINT64, (long long)MIN_INT64, (long long)MAX_INT64);
}
