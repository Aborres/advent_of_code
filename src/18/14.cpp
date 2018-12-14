//jnaranjo Advent of code 18

#include <18/14.h>

namespace Y18 {
  namespace D14 {

    static void BreakInsert(uint32 n, RingBuffer* buff) {

      if (n == 0) {
        buff->insertEnd(n);
        return;
      }

      uint32 c = 0;
      while (n > 0) {
        buff->insert(buff->size() - c, n % 10);
        n /= 10;
        ++c;
      }
    }

    void puzzle() {

      RingBuffer buffer(30000000); //Reserve big chunk
      BreakInsert(37, &buffer);

      RingBuffer comp;
      BreakInsert(input, &comp);
      const uint32 count_comp = comp.size();
      
      int32 elf1 = 0;
      int32 elf2 = 1;
      const uint32 limit = input + 10;
      
      bool found = false; //second part

      while(buffer.size() < limit || !found) {

        const uint32 elf1_res = buffer.read(elf1);
        const uint32 elf2_res = buffer.read(elf2);

        const uint32 new_recipe = elf1_res + elf2_res;
        BreakInsert(new_recipe, &buffer);

        elf1 += elf1_res + 1;
        elf2 += elf2_res + 1;

        if (!found) {
          const uint32 count = buffer.size();
          const uint32 comp_size = comp.size();
          const uint32 comp_m_one = comp_size - 1;
          int32 to = count - 1 - comp_size;
          to = to < 0 ? 0 : to;

          for (int32 i = count - 1; i >= to; --i) {
            if (buffer[i] == comp[comp_m_one]) {
              found = true;
              for (int32 j = comp_m_one; j >= 0; --j) {
                if (buffer[i - (comp_m_one - j)] != comp[j]) {
                  found = false;
                  break;
                }
              }
              if (found) {
                LOG("Second part: %d", i - comp_size + 1);
              }
            }
          }
        }
      }  

      PRINT("First part: ");
      const uint32 buffer_len = buffer.size();
      for (uint32 i = input; i < limit; ++i) {
        PRINT("%d", buffer[i]);
      }
    }
  }
}

