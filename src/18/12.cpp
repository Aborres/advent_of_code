//jnaranjo Advent of code 18

#include <18/12.h>

#include <utils.h>

#include <string>

namespace Y18 {
  namespace D12 {

    struct Mask {
      uint8 mask;
      uint8 res;
    };

    static uint8 Convert(char c) {
      return c == '#' ? 1 : 0;
    }

    static uint8 GenerateMask(uint8 l1, uint8 l2, uint8 c, uint8 r1, uint8 r2) {
      return (l1 << 4) | (l2 << 3) | (c  << 2) | (r1 << 1) | r2;
    }

    static uint8 ConvertGenerateMask(char l1, char l2, char c, char r1, char r2) {
      return GenerateMask(Convert(l1), Convert(l2), Convert(c), Convert(r1), Convert(r2));
    }

    static uint64 GetSum(uint8* buff, int32 size, int32 padding) {
      int64 count = 0;
      for (int32 i = 0; i < size; ++i) {
        count += buff[i] > 0 ? i - padding : 0;
      }
      return count;
    }

    static void ComputeGenerations(uint8* buff, uint8* next_gen,  uint32 buff_size,
                                   Mask* masks, uint32 count_masks,
                                   uint32 num_generations, uint32 padding, uint32* avg) {
      uint64 prev = 0;

      for (uint64 i = 0; i < num_generations; ++i) {
        for (uint32 p = 2; p < buff_size - 2; ++p) {
          next_gen[p] = 0;

          const uint8 plant = GenerateMask(buff[p - 2], buff[p - 1], buff[p], buff[p + 1], buff[p + 2]);

          for (uint32 m = 0; m < count_masks; ++m) {
            if (plant == masks[m].mask) {
              next_gen[p] = masks[m].res;
              break;
            }
          }
        }

        const uint64 curr = GetSum(buff, (int32)buff_size, (int32)padding);
        *avg = (uint32)(curr - prev);
        prev = curr;

        memcpy(buff, next_gen, buff_size);
      }
    }

    void puzzle() {

      const uint32 count_masks = COUNT_ARR(input_rules);
      Mask* masks = new Mask[count_masks];
      memset(masks, 0, sizeof(Mask) * count_masks);

      for (uint32 i = 0; i < count_masks; ++i) {
        char r1, r2, c, l1, l2, r;
        sscanf(input_rules[i], "%c%c%c%c%c => %c", &l1, &l2, &c, &r1, &r2, &r);
        masks[i].mask = ConvertGenerateMask(l1, l2, c, r1, r2);
        masks[i].res = Convert(r);
      }

      const uint32 padding = 250;
      const uint32 buff_size = (padding * 2) + (uint32)strlen(input);

      uint8* original = new uint8[buff_size];
      memset(original, 0, buff_size);

      uint8* buff = new uint8[buff_size];
      memset(buff, 0, buff_size);

      uint8* next_gen = new uint8[buff_size];
      memset(next_gen, 0, buff_size);

      const uint32 end_buff = buff_size - padding;
      for (uint32 i = padding; i < end_buff; ++i) {
        original[i] = Convert(input[i - padding]);
      }

      uint32 avg = 0;

      memcpy(buff, original, buff_size);
      ComputeGenerations(buff, next_gen, buff_size, masks, count_masks, 20, padding, &avg);

      uint64 fcount = GetSum(buff, (int32)buff_size, (int32)padding);
      LOG("First puzzle: %llu", fcount);

      //120 should be enough to converge with this input
      const uint32 num_generations = 120;
      memcpy(buff, original, buff_size);
      ComputeGenerations(buff, next_gen, buff_size, masks, count_masks, num_generations, padding, &avg);

      fcount = GetSum(buff, (int32)buff_size, (int32)padding);
      LOG("Second puzzle: %llu", ((50000000000 - num_generations) * avg) + fcount);

      delete[] original;
      delete[] buff;
      delete[] next_gen;
      delete[] masks;
    }
  }
}