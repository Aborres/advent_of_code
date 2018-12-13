//jnaranjo Advent of code 18

#include <18/13.h>

#include <utils.h>

#include <vector>
#include <set>
#include <algorithm>

namespace Y18 {
  namespace D13 {

    struct Change {
      enum Enum {
        Left = 0,
        Straight,
        Right,
        //-------
        Count,
        None,
      };
    };

    struct Direction {
      enum Enum {
        Right = 0,
        Left,
        Up,
        Down,
        //-----
        Count,
        None
      };
    };
    
    struct Cart {
      vec2i pos;
      Change::Enum change;
      Direction::Enum direction;

      Cart(const vec2i& v, Direction::Enum d) : pos(v), change(Change::Left),
                                                direction(d) {}
      ~Cart(){};
    };

    static INLINE uint32 IT(uint32 x, uint32 y, uint32 w) {
      return x + (y * w);
    }

    static const Direction::Enum changes[3][4] = {
      { Direction::Up,    Direction::Down, Direction::Left,  Direction::Right, },
      { Direction::Right, Direction::Left, Direction::Up,    Direction::Down,  },
      { Direction::Down,  Direction::Up,   Direction::Right, Direction::Left,  },
    };

    static void ComputeLastChange(Cart* c) {

      c->direction = changes[c->change][c->direction];

      uint32 lc = (uint32)(c->change);
      ++lc;
      lc %= (uint32)Change::Count;
      c->change = (Change::Enum)lc;
    }

    static void ComputeDirection(char* buff, Cart* c, uint32 w, Direction::Enum l, Direction::Enum r) {
      const uint32 it = IT(c->pos.x, c->pos.y, w);
      const char next_pos = buff[it];
      if (next_pos == '+') {
        ComputeLastChange(c);
      } else if (next_pos == '\\') {
        c->direction = l;
      } else if (next_pos == '/') {
        c->direction = r;
      } 
    }

    static void ComputeDirection(char* buff, Cart* c, uint32 w) {
      if (c->direction == Direction::Right) {
        c->pos.x += 1;
        ComputeDirection(buff, c, w, Direction::Down, Direction::Up);
      } else if (c->direction == Direction::Left) {
        c->pos.x -= 1;
        ComputeDirection(buff, c, w, Direction::Up, Direction::Down);
      } else if (c->direction == Direction::Up) {
        c->pos.y -= 1;
        ComputeDirection(buff, c, w, Direction::Left, Direction::Right);
      } else if (c->direction == Direction::Down) {
        c->pos.y += 1;
        ComputeDirection(buff, c, w, Direction::Right, Direction::Left);
      }
    }

    void puzzle() {

      char* buff;

      if (!ReadInput("assets/input/18/13_input.txt", &buff))
        ASSERT(false, "invalid path to read");

      uint32 w = 0;
      {
        uint32 it = 0;
        for (;;) {
          if (buff[it] == '\n') {
            w = it + 1;
            break;
          }
          ++it;
        }
      }

      const uint32 count = (uint32)strlen(buff);
      const uint32 h = count / w;

      std::vector<Cart> carts;
      
      for (int32 y = 0; y < (int32)h; ++y) {
        for (int32 x = 0; x < (int32)w; ++x) {
          const uint32 it = IT(x, y, w);
          const char c = buff[it];
          if (c == 'v') {
            carts.push_back(Cart({x, y}, Direction::Down));
          } else if (c == '^') {
            carts.push_back(Cart({x, y}, Direction::Up));
          } else if (c == '>') {
            carts.push_back(Cart({x, y}, Direction::Right));
          } else if (c == '<') {
            carts.push_back(Cart({x, y}, Direction::Left));
          }
        }
      }

      std::vector<uint32> remove_pool;
      std::vector<Cart> temp_pool;

      bool is_first_collision = true;

      for (;;) {

        const uint32 carts_size = (uint32)carts.size();
        for (uint32 i = 0; i < carts_size; ++i) {

          if (std::find(remove_pool.begin(), remove_pool.end(), i) == remove_pool.end()) {

            ComputeDirection(buff, &carts[i], w);

            //Check collisions
            for (uint32 j = 0; j < carts_size; ++j) {
              if (j != i) {
                if (carts[i].pos == carts[j].pos) {
                  if (is_first_collision) {
                    is_first_collision = false;
                    LOG("First collision happened at: %d,%d",
                        carts[i].pos.x, carts[i].pos.y);
                  }
                  remove_pool.push_back(i);
                  remove_pool.push_back(j);
                  break;
                }
              }
            }
          }
        }

        const uint32 remove_count = (uint32)remove_pool.size();
        if (remove_count > 0) {
          for (uint32 c = 0; c < carts_size; ++c) {
            if (std::find(remove_pool.begin(), remove_pool.end(), c) == remove_pool.end()) 
              temp_pool.push_back(carts[c]);
          }
          carts.clear();
          carts.assign(temp_pool.begin(), temp_pool.end());
          remove_pool.clear();
          temp_pool.clear();
        }

        if (carts.size() == 1) {
          break;
        }

        std::sort(carts.begin(), carts.end(), [](const Cart& l, const Cart& r) {
          return (l.pos.x < r.pos.x) && (l.pos.y < r.pos.y);
        });
      }

      LOG("The last cart is at: %d,%d", carts[0].pos.x, carts[0].pos.y);

      delete buff;
    }
  }
}

