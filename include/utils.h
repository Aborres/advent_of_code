//jnaranjo Advent of code 18

#ifndef ADVENT_OF_CODE_UTILS
#define ADVENT_OF_CODE_UTILS (1)

#include <stdint.h>
#include <stdio.h>

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#define UNUSED(O) (void)O

#define COUNT_ARR(B) (sizeof(B) / sizeof(B[0]))

#define LOG(F, ...) printf(F"\n", __VA_ARGS__)

#endif //ADVENT_OF_CODE_UTILS
