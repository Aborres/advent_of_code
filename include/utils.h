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
  RingBuffer(uint32 r = 128) : data_(nullptr), buff_size_(0),
                               num_elem_(0), reserve_size_(r)  {
    buff_size_ = reserve_size_;
    num_elem_ = 0;
    data_ = new uint32[buff_size_];
    memset(data_, 0, buff_size_ * sizeof(uint32));
  }

  ~RingBuffer() {
    delete[] data_;
  }

  uint32 insert(int32 pos, uint32 o) {
    const uint32 size = this->size();

    checkResize(size + 1);

    writeIT(pos);

    if ((uint32)pos < size)
      memcpy(data_ + pos + 1, data_ + pos, sizeof(uint32) * (size - pos));

    data_[pos] = o;
    ++num_elem_;

    return pos;
  }
  
  uint32 insertFront(uint32 o) {
    return insert(0, o);
  }

  uint32 insertEnd(uint32 o) {
    return insert(size(), o);
  }

  uint32 remove(int32 pos) {
    readIT(pos);
    memcpy(data_ + pos, data_ + pos + 1, sizeof(uint32) * (size() - pos));
    --num_elem_;
    return pos;
  }

  void clear() {
    memset(data_, 0, size() * sizeof(uint32));
    num_elem_ = 0;
  }

  uint32 size() const {
    return num_elem_;
  }

  uint32 buffSize() const {
    return buff_size_;
  }

  void checkResize(uint32 new_size) {
    if (new_size > buffSize()) {

      buff_size_ = new_size + reserve_size_;

      uint32* tmp = new uint32[buff_size_];
      memcpy(tmp, data_, sizeof(uint32) * size());

      delete[] data_;
      data_ = tmp;
    }
  }

  uint32& operator[](int32 it) {
    readIT(it);
    return data_[it];
  }

  const uint32& operator[](int32 it) const {
    readIT(it);
    return data_[it];
  }

  uint32 read(int32& it) const {
    readIT(it);
    return data_[it];
  }

  void readIT(int32& it) const {
    const int32 size = (int32)this->size();
    if (size == 0) {
      it = 0;
    } else if (it < 0) {
      it = size + it;
    } else if (it >= size) {
      while (it >= size) {
        it -= size;
      }
    } 
  }

  void writeIT(int32& it) const {
    const int32 size = (int32)this->size();
    if (size == 0) {
      it = 0;
    } else if (it < 0) {
      it = size + it;
    } else if (it > size) {
      while (it > size) {
        it -= size;
      }
    } 
  }

private:
  uint32* data_;
  uint32 buff_size_;
  uint32 num_elem_;
  uint32 reserve_size_;
};

#define UNUSED(O) (void)O

#define COUNT_ARR(B) (sizeof(B) / sizeof(B[0]))

#define PRINT(F, ...) printf(F, __VA_ARGS__)

#define LOG(F, ...) PRINT(F"\n", __VA_ARGS__)

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