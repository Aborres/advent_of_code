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
      Change::Enum last_change;
      Direction::Enum direction;
    };

    static INLINE uint32 IT(uint32 x, uint32 y, uint32 w) {
      return x + (y * w);
    }

    static void ComputeLastChange(Cart* c) {
      if (c->last_change != Change::Straight) {
        if (c->last_change == Change::Right) {
          switch (c->direction) {
            case Direction::Right: {
              c->direction = Direction::Down;
              break;
            }
            case Direction::Left: {
              c->direction = Direction::Up;
              break;
            }
            case Direction::Up: {
              c->direction = Direction::Right;
              break;
            }
            case Direction::Down: {
              c->direction = Direction::Left;
              break;
            }
            default: {
              LOG("UNDEFINED DIRECTION");
            }
          }
        } else if (c->last_change == Change::Left) {
          switch (c->direction) {
            case Direction::Right: {
              c->direction = Direction::Up;
              break;
            }
            case Direction::Left: {
              c->direction = Direction::Down;
              break;
            }
            case Direction::Up: {
              c->direction = Direction::Left;
              break;
            }
            case Direction::Down: {
              c->direction = Direction::Right;
              break;
            }
            default: {
              LOG("UNDEFINED DIRECTION");
            }
          }
        }
      }

      uint32 lc = (uint32)(c->last_change);
      ++lc;
      lc %= (uint32)Change::Count;
      c->last_change = (Change::Enum)lc;
    }

    static void ComputeRight(char* buff, Cart* c, uint32 w) {
      c->pos.x += 1;
      const uint32 it = IT(c->pos.x, c->pos.y, w);
      const char next_pos = buff[it];
      if (next_pos == '+') {
        ComputeLastChange(c);
      } else if (next_pos == '\\') {
        c->direction = Direction::Down;
      } else if (next_pos == '/') {
        c->direction = Direction::Up;
      } 
    }

    static void ComputeLeft(char* buff, Cart* c, uint32 w) {
      c->pos.x -= 1;
      const uint32 it = IT(c->pos.x, c->pos.y, w);
      const char next_pos = buff[it];
      if (next_pos == '+') {
        ComputeLastChange(c);
      } else if (next_pos == '\\') {
        c->direction = Direction::Up;
      } else if (next_pos == '/') {
        c->direction = Direction::Down;
      } 
    }

    static void ComputeUp(char* buff, Cart* c, uint32 w) {
      c->pos.y -= 1;
      const uint32 it = IT(c->pos.x, c->pos.y, w);
      const char next_pos = buff[it];
      if (next_pos == '+') {
        ComputeLastChange(c);
      } else if (next_pos == '\\') {
        c->direction = Direction::Left;
      } else if (next_pos == '/') {
        c->direction = Direction::Right;
      } 
    }

    static void ComputeDown(char* buff, Cart* c, uint32 w) {
      c->pos.y += 1;
      const uint32 it = IT(c->pos.x, c->pos.y, w);
      const char next_pos = buff[it];
      if (next_pos == '+') {
        ComputeLastChange(c);
      } else if (next_pos == '\\') {
        c->direction = Direction::Right;
      } else if (next_pos == '/') {
        c->direction = Direction::Left;
      } 
    }

    static void ComputeDirection(char* buff, Cart* c, uint32 w) {
      switch (c->direction) {
        case Direction::Right: {
          ComputeRight(buff, c, w);
          break;
        }
        case Direction::Left: {
          ComputeLeft(buff, c, w);
          break;
        }
        case Direction::Up: {
          ComputeUp(buff, c, w);
          break;
        }
        case Direction::Down: {
          ComputeDown(buff, c, w);
          break;
        }
        default: {
          LOG("UNDEFINED DIRECTION");
        }
      }
    }

    void puzzle() {

      char* buff;

      if (!ReadInput("assets/input/18/13_input.txt", &buff))
        ASSERT(false, "invalid path to read");

      const uint32 count = (uint32)strlen(buff);

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
      
      const uint32 h = count / w;

      std::vector<Cart> carts;
      
      for (int32 y = 0; y < (int32)h; ++y) {
        for (int32 x = 0; x < (int32)w; ++x) {
          const uint32 it = IT(x, y, w);
          if (buff[it] == 'v') {
            Cart c;
            c.pos = { x, y };
            c.direction = Direction::Down;
            c.last_change = Change::Left;
            carts.push_back(c);
          } else if (buff[it] == '^') {
            Cart c;
            c.pos = { x, y };
            c.direction = Direction::Up;
            c.last_change = Change::Left;
            carts.push_back(c);
          } else if (buff[it] == '>') {
            Cart c;
            c.pos = { x, y };
            c.direction = Direction::Right;
            c.last_change = Change::Left;
            carts.push_back(c);
          } else if (buff[it] == '<') {
            Cart c;
            c.pos = { x, y };
            c.direction = Direction::Left;
            c.last_change = Change::Left;
            carts.push_back(c);
          }
        }
      }

      std::vector<uint32> remove_pool;
      while (true) {

        for (uint32 i = 0; i < carts.size(); ++i) {
          Cart* c = &carts[i];
          ComputeDirection(buff, c, w);
        }

        for (uint32 i = 0; i < carts.size(); ++i) {
          const Cart& a = carts[i];
          for (uint32 j = 0; j < carts.size(); ++j) {
            if (j != i) {
              const Cart& b = carts[j];
              if (a.pos == b.pos) {
                remove_pool.push_back(i);
              }
            }
          }
        }

        if (remove_pool.size() > 0) {
          std::vector<Cart> temp;
          const uint32 remove_count = (uint32)remove_pool.size();
          for (uint32 c = 0; c < carts.size(); ++c) {
            bool found = false;
            for (uint32 r = 0; r < remove_count; ++r) {
              if (c == remove_pool[r]) {
                found = true;
                break;
              }
            }
            if (!found)
              temp.push_back(carts[c]);
          }
          carts.clear();
          carts.assign(temp.begin(), temp.end());
          remove_pool.clear();
        }

        if (carts.size() == 1) {
          const vec2i& c = carts[0].pos;
          LOG("%d, %d", c.x, c.y);
        }

        std::sort(carts.begin(), carts.end(), [](const Cart& l, const Cart& r) {
          return (l.pos.x < r.pos.x) && (l.pos.y < r.pos.y);
        });
      }

      delete buff;
    }

  }
}

