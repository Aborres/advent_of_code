//jnaranjo Advent of code 18

#include <18/6.h>

#include <set>
#include <algorithm>
#include <limits>

namespace Y18 {
  namespace D6 {

    static uint32 Manhattan(const vec2i& a, const vec2i& b) {
      return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }

    void puzzle() {
      
      const int32 big_n = std::numeric_limits<int32>::max();
      vec2i min = { big_n,  big_n };
      vec2i max = {-big_n, -big_n };

      //Find boundaries
      const uint32 num_points = COUNT_ARR(input);
      for (uint32 i = 0; i < num_points; ++i) {
        const vec2i& p = input[i];
        
        min.x = std::min(min.x, p.x);
        min.y = std::min(min.y, p.y);

        max.x = std::max(max.x, p.x);
        max.y = std::max(max.y, p.y);
      }

      const uint32 area_w = (max.x - min.x) + 1;
      const uint32 area_h = (max.y - min.y) + 1;
      const uint32 area_size = area_w * area_h;

      vec3i* area = new vec3i[area_size];

      for (uint32 i = 0; i < area_size; ++i) {
        area[i] = vec3i{ big_n, big_n, 0 };
      }

      //Compute distances
      for (uint32 i = 0; i < num_points; ++i) {
        for (int32 y = 0; y < (int32)area_h; ++y) {
          for (int32 x = 0; x < (int32)area_w; ++x) {

            const uint32 it = x + (y * area_w);

            const vec2i& p = input[i];
            const vec2i& a_p = { x + min.x, y + min.y };
            
            const uint32 d = Manhattan(p, a_p);
            const uint32 c_d = area[it].x;

            if (d < c_d) {
              area[it].x = d;
              area[it].y = i;
            } else if (d == c_d) {
              area[it].y = -1;
            }

            area[it].z += d;
          }
        }
      }
      
      //Remove points with infinite area 
      std::set<uint32> valid_points;
      for (uint32 i = 0; i < num_points; ++i) {
        valid_points.insert(i);
      }

      for (int32 y = 0; y < (int32)area_h; ++y) {
        for (int32 x = 0; x < (int32)area_w; ++x) {

          const uint32 it = x + (y * area_w);
          const vec2i& p = { x + min.x, y + min.y };

          if ((area[it].y > -1) && 
             ((p.x <= min.x) || (p.x >= max.x) || (p.y <= min.y) || (p.y >= max.y))) {
            valid_points.erase(area[it].y);
          }
        }
      }

      const uint32 limit = 10000;

      uint32 total_counter = 0;
      uint32 size_biggest_area = 0;
      for (auto& i : valid_points) {

        uint32 counter = 0;
        for (uint32 a = 0; a < area_size; ++a) {

          const vec3i p = area[a];
          if (p.y == (int32)i) {
            ++counter;
          }

          size_biggest_area += area[a].z < limit ? 1 : 0; 
        }

        if (counter > total_counter) {
          total_counter = counter;
        }
      }

      LOG("The biggest area was: %u", total_counter);

      size_biggest_area /= valid_points.size();
      LOG("Second Part: The biggest area was: %u", size_biggest_area);

      delete[] area;
    }
  }
}