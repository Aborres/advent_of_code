//jnaranjo Advent of code 18

#include <18/1.h>

#include <set>

namespace Y18 {
  namespace D1 {

    void puzzle() {

      int32 accum = 0; 

      const uint32 buff_size = COUNT_ARR(freq);
      for (uint32 i = 0; i < buff_size; ++i) {
        accum += freq[i];
      }

      LOG("The correct final frequency is: %d", accum);

      std::set<int32> pool;

      accum = 0; 

      uint32 i = 0;
      for(;;) {
        accum += freq[i % buff_size];
        auto it = pool.insert(accum);
        if (!it.second) {
          LOG("The first repeated freq is: %d", accum);
          break;
        }
        ++i;
      }
    }

  }
}

