//jnaranjo Advent of code 18

#include <18/5.h>

#include <string.h>

#include <cmath>
#include <set>
#include <vector>

namespace Y18 {
  namespace D5 {

    static void ReadInput(const char* path, char** out) {
      FILE* f = fopen(path, "rb");

      if (f) {
        fseek(f, 0, SEEK_END);
        const uint32 size = (uint32)ftell(f);
        fseek(f, 0, SEEK_SET);

        *out = new char[size + 1];
        fread(*out, size, 1, f);
        (*out)[size] = '\0';

        fclose(f);
      }
    }

    static bool IsCapital(uint8 c) {
      return c < 91; //[;
    }

    static bool ShouldReact(uint8 curr, uint8 next) {
      const uint32 step_size = 32;

      const bool curr_c = IsCapital(curr);
      const bool next_c = IsCapital(next);
      
      if ((curr_c != next_c) && (std::abs(curr - next) == step_size)) {
        return true;
      }
      return false;
    }

    static bool IsPolymere(uint8 curr, uint8 p) {
      const bool curr_c = IsCapital(curr);

      if ((curr_c && (curr == p)) || (!curr_c && ((curr - 32) == p))) {
        return true;
      }
      return false;
    }

    static void DeleteFrom(uint8* buff, uint32 pos, uint32 num, uint32 size) {
      memcpy(buff + pos, buff + pos + num, size - pos - num);
    }

    static void ComputeReaction(uint8* input, uint32 count) {

      bool done = false;
      uint32 last_init = 0;
      while (!done) {

        done = true;

        for (uint32 i = last_init; i < count; ++i) {
          const uint8 curr = input[i];
          const uint8 next = input[i + 1];
          
          if (ShouldReact(curr, next)) {
            DeleteFrom(input, i, 2, count);
            done = false;
            count -= 2;
            break;
          }
        }
      }

      input[count] = '\0';
    }

    void puzzle() {

      uint8* const_input = nullptr;
      ReadInput("assets/input/5.txt", (char**)&const_input);

      const uint32 const_count = (uint32)strlen((char*)const_input);

      uint32 count = const_count;
      uint8* input = new uint8[count];
      memcpy(input, const_input, count);

      //Part One
      ComputeReaction(input, count);

      LOG("Final polymer length is: %u", (uint32)strlen((char*)input));

      count = const_count;
      std::set<uint8> polymers;

      for (uint32 i = 0; i < count; ++i) {
        const uint8 c = const_input[i];
        if (IsCapital(c)) {
          polymers.insert(c);
        }
      }

      std::vector<uint32> results;

      const uint32 count_polymers = (uint32)polymers.size();
      for (auto& polymer : polymers) {

        count = const_count;
        memcpy(input, const_input, count);
        bool done = false;

        uint32 last_init = 0;
        while (!done) {

          done = true;

          for (uint32 i = last_init; i < count; ++i) {
            const uint8 curr = input[i];
            
            if (IsPolymere(curr, polymer)) {
              DeleteFrom(input, i, 1, count);
              done = false;
              --count;
              last_init = i;
              break;
            }
          }
        }

        ComputeReaction(input, count);
        results.push_back((uint32)strlen((char*)input));
      }
      
      uint32 min = const_count + 1000;
      const uint32 count_results = (uint32)results.size();
      for (uint32 i = 0; i < count_results; ++i) {
        if (results[i] < min) {
          min = results[i];
        }
      }

      LOG("The smallest count was: %d", min);

      delete[] input;
      delete[] const_input;
    }
  }
}

