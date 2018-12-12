//jnaranjo Advent of code 18

#include <18/11.h>

#include <algorithm>

namespace Y18 {
  namespace D11 {

    static const uint32 test = 8;

    static int32 ComputeCellFuel(uint32 x, uint32 y, uint32 serial_number) {
      const uint32 rack_id = x + 10;
      const int32 power_level = ((rack_id * y) + serial_number) * rack_id;
      const int32 res = (power_level / 100) % 10;
      return res - 5;
    }

    static int32 GetFuel(int32* buff, int32 x, int32 y, uint32 limit, uint32 w) {
      int32 p = 0;

      const int32 x1 = x + limit;
      const int32 y1 = y + limit;
      
      int32 y0 = y;
      do {
        const uint32 it = x1 + (y0 * w);
        p += buff[it];
        ++y0;
      } while (y0 <= y1);

      int32 x0 = x;
      while(x0 < x1) {
        const uint32 it = x0 + (y1 * w);
        p += buff[it];
        ++x0; 
      }

      return p;
    }

    void puzzle() {

      vec2i cell = {0,0};
      int32 max_fuel = -999999;

      const int32 limit = 300;
      int32 buff[limit * limit];
      for (uint32 y = 1; y <= limit; ++y) {
        for (uint32 x = 1; x <= limit; ++x) {
          const uint32 x1 = x - 1;
          const uint32 y1 = y - 1;
          buff[x1 + (y1 * limit)] = ComputeCellFuel(x, y, input);
        }
      }

      for (int32 y = 0; y < limit - 3; ++y) {
        for (int32 x = 0; x < limit - 3; ++x) {

          int32 p = 0;
          for (uint32 i = 0; i < 3; ++i) {
            p += GetFuel(buff, x, y, i, limit);
          }

          if (p > max_fuel) {
            max_fuel = p;
            cell.x = x + 1;
            cell.y = y + 1;
          }
        }
      }

      LOG("First part: %d,%d", cell.x, cell.y);

      uint32 size = 0;
      max_fuel = -999999;

      for (int32 y = 0; y < limit; ++y) {
        for (int32 x = 0; x < limit; ++x) {

          int32 p = 0;

          const int32 end = limit - std::max(x, y);
          for (int32 i = 0; i < end; ++i) {

            p += GetFuel(buff, x, y, i, limit);

            if (p > max_fuel) {
              max_fuel = p;
              cell.x = x + 1;
              cell.y = y + 1;
              size = i + 1;
            }
          }
        }
      }

      LOG("Second part: %d,%d,%u", cell.x, cell.y, size);
    }
  }
}