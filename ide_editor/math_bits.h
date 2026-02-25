#ifndef MATH_BITS_H
#define MATH_BITS_H

#include <stdint.h>

// 8-bit math
#define MAX_UINT8  ((uint8_t)0xFF)
#define MAX_INT8   ((int8_t)0x7F)
#define MIN_INT8   ((int8_t)0x80)

// 16-bit math
#define MAX_UINT16 ((uint16_t)0xFFFF)
#define MAX_INT16  ((int16_t)0x7FFF)
#define MIN_INT16  ((int16_t)0x8000)

// 32-bit math
#define MAX_UINT32 ((uint32_t)0xFFFFFFFF)
#define MAX_INT32  ((int32_t)0x7FFFFFFF)
#define MIN_INT32  ((int32_t)0x80000000)

// 64-bit math
#define MAX_UINT64 ((uint64_t)0xFFFFFFFFFFFFFFFFULL)
#define MAX_INT64  ((int64_t)0x7FFFFFFFFFFFFFFFLL)
#define MIN_INT64  ((int64_t)0x8000000000000000LL)

// Utility macros for overflow-safe addition, subtraction, multiplication
#define ADD8(a,b)  ((uint8_t)(((uint16_t)(a)+(uint16_t)(b))&0xFF))
#define ADD16(a,b) ((uint16_t)(((uint32_t)(a)+(uint32_t)(b))&0xFFFF))
#define ADD32(a,b) ((uint32_t)(((uint64_t)(a)+(uint64_t)(b))&0xFFFFFFFF))
#define ADD64(a,b) ((uint64_t)((a)+(b)))

#define SUB8(a,b)  ((uint8_t)(((uint16_t)(a)-(uint16_t)(b))&0xFF))
#define SUB16(a,b) ((uint16_t)(((uint32_t)(a)-(uint32_t)(b))&0xFFFF))
#define SUB32(a,b) ((uint32_t)(((uint64_t)(a)-(uint64_t)(b))&0xFFFFFFFF))
#define SUB64(a,b) ((uint64_t)((a)-(b)))

#define MUL8(a,b)  ((uint8_t)(((uint16_t)(a)*(uint16_t)(b))&0xFF))
#define MUL16(a,b) ((uint16_t)(((uint32_t)(a)*(uint32_t)(b))&0xFFFF))
#define MUL32(a,b) ((uint32_t)(((uint64_t)(a)*(uint64_t)(b))&0xFFFFFFFF))
#define MUL64(a,b) ((uint64_t)((a)*(b)))

#define DIV8(a,b)  ((uint8_t)((b)?((a)/(b)):0))
#define DIV16(a,b) ((uint16_t)((b)?((a)/(b)):0))
#define DIV32(a,b) ((uint32_t)((b)?((a)/(b)):0))
#define DIV64(a,b) ((uint64_t)((b)?((a)/(b)):0))

void print_math_limits(void);

#endif // MATH_BITS_H
