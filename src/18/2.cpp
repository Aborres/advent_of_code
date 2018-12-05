//jnaranjo Advent of code 18

#include <18/2.h>

#include <utils.h>

#include <vector>
#include <set>
#include <string>
#include <algorithm>

namespace Y18 {
  namespace D2 {

    struct Counter {
      uint32 counted;

      std::set<char> letters;
      std::set<uint32> counter;

      Counter() : counted(0) {}
    };

    void puzzle() {

      const uint32 count = COUNT_ARR(input);
      const uint32 len = strlen(input[0]);

      std::vector<Counter> valid_strings;

      //Preprocess
      for (uint32 i = 0; i < count; ++i) {

        Counter c;
        const char* str = input[i];

        for (uint32 j = 0; j < len; ++j) {

          char actual_c = str[j];
          
          if (!c.letters.insert(actual_c).second)
            continue;

          c.counted = 1;

          for (uint32 k = 0; k < len; ++k) {
            if ((j != k) && (str[k] == actual_c)) {
              ++c.counted;
            }
          }
          if (c.counted > 1) {
            c.counter.insert(c.counted);
          } 
        }
        if (c.counter.size() > 0) {
          valid_strings.push_back(c);
        }
      }
      
      //Checksum
      uint32 checksum[25];
      const uint32 checksum_l = COUNT_ARR(checksum);
      memset(checksum, 0, sizeof(uint32) * checksum_l);

      const uint32 num_strings = valid_strings.size();
      for (uint32 i = 0; i < num_strings; ++i) {
        Counter* c = &valid_strings[i];
        
        for (auto it : c->counter) {
          ++checksum[it];
        }
      }
      
      uint32 total = 1;
      for (uint32 i = 0; i < checksum_l; ++i) {
        total *= std::max(checksum[i], 1u);
      }

      LOG("The final checksum is: %d", total);

      //Part two
      for (uint32 i = 0; i < count; ++i) {
        const char* str = input[i];

        for (uint32 j = 0; j < count; ++j) {
          if (j != i) {

            const char* new_str = input[j];
            uint32 num_diff = 0;
            for (uint32 k = 0; k < len; ++k) {
              if (str[k] != new_str[k]) {
                ++num_diff;
              }
              if (num_diff > 1) {
                break;
              }
            }

            if (num_diff == 1) {

              std::string final_string = "\0";
              final_string.reserve(len);

              for (uint32 s = 0; s < len; ++s) {
                if (str[s] == new_str[s])
                  final_string += str[s];
              }

              LOG("FOUND: %s", final_string.c_str());
              return;
            }
          }
        }
      }
    }
  }
}

