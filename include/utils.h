//jnaranjo Advent of code 18

#ifndef ADVENT_OF_CODE_UTILS
#define ADVENT_OF_CODE_UTILS (1)

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

template<class T>
struct vec2 {
  T x, y;

  vec2& operator+=(const vec2& o) {
    x += o.x; 
    y += o.y;
    return *this;
  }

  vec2 operator+(const vec2& r) const {
    return vec2{ x + r.x, y + r.y };
  }

  vec2 operator-(const vec2& r) const {
    return vec2{ x - r.x, y - r.y };
  }

  bool operator==(const vec2& o) const {
    return (x == o.x) && (y == o.y);
  }
};


template<class T>
struct vec3 {
  T x, y, z;
};

typedef vec2<int32> vec2i;
typedef vec3<int32> vec3i;

#define UNUSED(O) (void)O

#define COUNT_ARR(B) (sizeof(B) / sizeof(B[0]))

#define LOG(F, ...) printf(F"\n", __VA_ARGS__)

#define ASSERT(C, ...) assert(C && __VA_ARGS__)

#define INLINE inline

INLINE bool ReadInput(const char* path, char** out) {
  FILE* f = fopen(path, "rb");

  if (f) {
    fseek(f, 0, SEEK_END);
    const uint32 size = (uint32)ftell(f);
    fseek(f, 0, SEEK_SET);

    *out = new char[size + 1];
    fread(*out, size, 1, f);
    (*out)[size] = '\0';

    fclose(f);
    return true;
  }
  return false;
}

#endif //ADVENT_OF_CODE_UTILS