//jnaranjo Advent of code 18

#include <18/10.h>

#include <utils.h>

#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>

namespace Y18 {
  namespace D10 {

    struct Point {
      vec2i pos;
      vec2i vel;
    };

    void puzzle() {

      std::vector<Point> points;

      const uint32 count = COUNT_ARR(input);
      for (uint32 i = 0; i < count; ++i) {
        Point p;
        sscanf(input[i], "position = <%d, %d> velocity = <%d, %d>", &p.pos.x, &p.pos.y, &p.vel.x, &p.vel.y);
        points.push_back(p);
      }

      std::vector<Point> result(points.size());

      const int32 big = 9999999;
      uint32 min_distance = big;
      vec2i c_min = {0, 0};
      vec2i c_max = {0, 0};
      uint32 time = 0;

      uint32 it = 0;
      while (it < 20000) {

        vec2i min = { big, big };
        vec2i max = {-big, -big};

        for (uint32 i = 0; i < count; ++i) {

          Point& p = points[i]; 
          p.pos += points[i].vel;

          min.x = std::min(p.pos.x, min.x);
          min.y = std::min(p.pos.y, min.y);

          max.x = std::max(p.pos.x, max.x);
          max.y = std::max(p.pos.y, max.y);
        }

        const uint32 distance = (max.x - min.x) + (max.y - min.y);
        if (distance < min_distance) {
          min_distance = distance;
          result.clear();
          result.assign(points.begin(), points.end());
          c_min = min;
          c_max = max;
          time = it;
        }
        ++it;
      }

      PRINT("(%d): %d\n\n", time, min_distance);

      for (int32 y = c_min.y; y <= c_max.y; ++y) {
        for (int32 x = c_min.x; x <= c_max.x; ++x) {
          bool found = false;
          for (uint32 i = 0; i < count; ++i) {
            if (result[i].pos == vec2i{x, y}) {
              PRINT("#");
              found = true;
              break;
            }
          }
          if (!found)
            PRINT(" ");
        }
        PRINT("\n");
      }
      PRINT("\n");
    }
  }
}

