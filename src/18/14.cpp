//jnaranjo Advent of code 18

#include <18/14.h>

#include <utils.h>

#include <vector>

namespace Y18 {
  namespace D14 {

    static void BreakInsert(uint32 n, RingBuffer* buff) {

      RingBuffer temp;
      while (n > 0) {
        temp.insertFront(n % 10);
        n /= 10;
      }

      const uint32 size = temp.size();
      for (uint32 i = 0; i < size; ++i) {
        buff->insertEnd(temp[i]);
      }
    }

    const static uint32 input = 825401;
    const static uint32 tests = 2018;

#define INPUT tests

    void puzzle() {

      RingBuffer buffer;
      BreakInsert(37, &buffer);
      
      int32 elf1 = 0;
      int32 elf2 = 1;
      while(buffer.size() < INPUT + 10) {

        //const uint32 count = buffer.size();
        //for (uint32 i = 0; i < count; ++i) {
        //  printf("%d", buffer[i]);
        //}
        //printf("\n");

        const uint32 elf1_res = buffer.read(elf1);
        const uint32 elf2_res = buffer.read(elf2);

        const uint32 new_recipe = elf1_res + elf2_res;
        BreakInsert(new_recipe, &buffer);

        elf1 += elf1_res + 1;
        elf2 += elf2_res + 1;
      }

      const uint32 buffer_len = buffer.size();
      for (uint32 i = INPUT; i < INPUT + 10; ++i) {
        printf("%d", buffer[i]);
      }
    }

  }
}

