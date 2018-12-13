//jnaranjo Advent of code 18

#include <18/03.h>

#include <utils.h>

#include <string.h>

#include <vector>
#include <set>
#include <algorithm>

namespace Y18 {
  namespace D3 {

//Simple version...?
#if 1

    struct Square {
      vec2i o;
      vec2i s;
    };

    void puzzle() {

      const uint32 count = COUNT_ARR(input);

      std::vector<Square> squares;
      squares.reserve(count);

      std::set<uint32> ids;

      //PreProcess
      for (uint32 i = 0; i < count; ++i) {
        Square s;
        sscanf(input[i], "%d,%d, %d,%d", &s.o.x, &s.o.y, &s.s.x, &s.s.y);

        squares[i] = s;
        ids.insert(i);
      }

      const uint32 cloth_size = 1000 * 1000;
      vec2i cloth[cloth_size];
      memset(cloth, 0, sizeof(vec2i) * cloth_size);

      for (uint32 i = 0; i < count; ++i) {

        const Square& s = squares[i];
        const int32 f_y = s.o.y + s.s.y;
        const int32 f_x = s.o.x + s.s.x;

        bool cleared = false;
        for (int32 y = s.o.y; y < f_y; ++y) {
          for (int32 x = s.o.x; x < f_x; ++x) {

            const uint32 it = x + (y * 1000);

            if (cloth[it].x > 0 && !cleared) {
              cleared = true;
              ids.erase(i);
              ids.erase(cloth[it].y);
            }

            cloth[it].x += 1;
            if (cloth[it].y > 0) {
              ids.erase(cloth[it].y);
            }

            cloth[it].y = i;
          }
        }
      }

      uint32 fcount = 0;

      for (uint32 i = 0; i < cloth_size; ++i) {
        fcount += cloth[i].x > 1 ? 1 : 0;
      }

      LOG("The number of inches of overlap is: %u", fcount);

      //His Ids start at 1 (+1)
      LOG("The only pach that doesn't overlap is: %u", *ids.begin() + 1); 
    }

#else
    struct Square {
      vec2i o;
      vec2i s;

      const vec2i max() const {
        return vec2i{o.x + s.x, o.y + s.y};   
      }

      const vec2i min() const {
        return o;   
      }

      bool valid() const {
        const vec2i max = this->max();
        const vec2i min = this->min();
        return (min.x < max.x) && (min.y < max.y);
      }

      bool intersect(const Square& other, Square* in) const {

        const vec2i min = this->min();
        const vec2i max = this->max();

        const vec2i other_min = other.min();
        const vec2i other_max = other.max();

        vec2i in_min, in_max;

        in_min.x = std::max(min.x, other_min.x);
        in_min.y = std::max(min.y, other_min.y);

        in_max.x = std::min(max.x, other_max.x);
        in_max.y = std::min(max.y, other_max.y);

        in->o = in_min;
        in->s = vec2i{in_max.x - in_min.x, in_max.y - in_min.y}; 

        return in->valid();
      }
    };

    struct Collision {
      uint32 a;
      uint32 b;
    };

    static bool IsInList(const std::vector<Collision>& collisions, 
                         uint32 a, uint32 b) {

      const uint32 count = collisions.size();
      for (uint32 i = 0; i < count; ++i) {
        const Collision& c = collisions[i];
        if ((c.a == a && c.b == b) || (c.a == b && c.b == a))
          return true;
      } 
      return false;
    }

    //This is terribly over-engineered. I was expecting the second part to be a:
    //reorganise the squares in the cloth so they fit...
    void puzzle() {

      const uint32 count = COUNT_ARR(input);

      std::vector<Collision> collisions;
      std::vector<Square> squares;
      std::vector<Square> intersections;
      std::set<uint32> ids;
      squares.reserve(count);

      //PreProcess
      for (uint32 i = 0; i < count; ++i) {
        Square s;
        sscanf(input[i], "%d,%d, %d,%d", &s.o.x, &s.o.y, &s.s.x, &s.s.y);

        squares[i] = s;
        ids.insert(i);
      }

      for (uint32 i = 0; i < count; ++i) {
        const Square& first = squares[i];

        for (uint32 j = 0; j < count; ++j) {
          if (i != j) {
            const Square& second = squares[j];
            
            Square intersection;
            if (first.intersect(second, &intersection)) {
              if (!IsInList(collisions, i, j)) {
                collisions.push_back(Collision{i, j});
                intersections.push_back(intersection);
                ids.erase(i);
                ids.erase(j);
              }
            }
          }
        }
      }

      //Count inches based on intersections
      const uint32 cloth_size = 1000 * 1000;
      uint32 cloth[cloth_size];
      memset(cloth, 0, sizeof(uint32) * cloth_size);

      std::vector<vec2i> inches;
      const uint32 count_inters = intersections.size();

      for (uint32 i = 0; i < count_inters; ++i) {

        const Square& s = intersections[i];
        const int32 f_y = s.o.y + s.s.y;
        const int32 f_x = s.o.x + s.s.x;

        for (int32 y = s.o.y; y < f_y; ++y) {
          for (int32 x = s.o.x; x < f_x; ++x) {
            cloth[x + (y * 1000)] = 1;
          }
        }
      }

      uint32 fcount = 0;

      for (uint32 i = 0; i < cloth_size; ++i) {
        fcount += cloth[i];
      }

      LOG("The number of inches of overlap is: %u", fcount);

      //His Ids start at 1 (+1)
      LOG("The only pach that doesn't overlap is: %u", *ids.begin() + 1); 
    }

#endif

  }
}

