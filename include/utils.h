//jnaranjo Advent of code 18

#ifndef ADVENT_OF_CODE_UTILS
#define ADVENT_OF_CODE_UTILS (1)

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include <string.h>

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

class RingBuffer {
public:
  RingBuffer(uint32 r) : data(nullptr), buff_size(0), num_elem(0), reserve_size(r)  {
    buff_size = reserve_size;
    num_elem = 0;
    data = new uint32[buff_size];
    memset(data, 0, buff_size * sizeof(uint32));
  }

  ~RingBuffer() {
    delete[] data;
  }

  uint32 insert(int32 pos, uint32 o) {
    const uint32 size = this->size();

    checkResize(size + 1);

    fixIT(pos);

    memcpy(data + pos + 1, data + pos, sizeof(uint32) * (size - pos));
    data[pos] = o;
    ++num_elem;

    return pos;
  }

  void remove(int32& pos) {
    fixIT(pos);
    memcpy(data + pos, data + pos + 1, sizeof(uint32) * (size() - pos));
    --num_elem;
  }

  uint32 size() const {
    return num_elem;
  }

  uint32 buffSize() const {
    return buff_size;
  }

  void checkResize(uint32 new_size) {
    if (new_size > buffSize()) {

      buff_size = new_size + reserve_size;

      uint32* tmp = new uint32[buff_size];
      memcpy(tmp, data, sizeof(uint32) * size());

      delete[] data;
      data = tmp;
    }
  }

  uint32& operator[](int32 it) {
    fixIT(it);
    return data[it];
  }

  void fixIT(int32& it) {
    const int32 size = (int32)this->size();
    if (it < 0) {
      it = size + it;
    } else if (it > size) {
      it -= size;
    } 
  }

private:
  uint32* data;
  uint32 buff_size;
  uint32 num_elem;
  uint32 reserve_size;
};

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