//jnaranjo Advent of code 18

#include <18/15.h>

#include <utils.h>

#include <vector>
#include <algorithm>

namespace Y18 {
  namespace D15 {

    enum Type {
      Goblin = -1,
      None = 0,
      Elf = 1,
      Wall,
      Space,
    };

    struct Soldier {
      Type type; 
      int32 life;
      vec2i pos;
      Soldier* enemy;
      Soldier(Type t, const vec2i& p) : type(t), life(200), pos(p), enemy(nullptr) {};
      ~Soldier() {};
    };

    static uint32 IT(uint32 x, uint32 y, uint32 w) {
      return x + (y * w);
    }

    auto FindSoldier(const std::vector<Soldier*>& soldiers, const vec2i& pos) {
        return std::find_if(soldiers.begin(), soldiers.end(), [&pos](const Soldier* e) {
          return pos == e->pos; 
        });
    }

    static void RemoveFromPool(std::vector<Soldier*>* pool, const vec2i& pos) {
      pool->erase(FindSoldier(*pool, pos));
    }

    static bool InternalSMove(Type* buff, const uint32 w, Soldier* s,
                             int32 x, int32 y, const std::vector<Soldier*>& soldiers) {
      const vec2i& pos = s->pos;

      const vec2i e_pos = {pos.x + x, pos.y + y};
      const uint32 it = IT(e_pos.x, e_pos.y, w);
      if (buff[it] == -s->type) {
        auto e = FindSoldier(soldiers, e_pos);
        s->enemy = *e;
        return true;
      } 
      return false;
    }

    static void Move(Type* buff, const uint32 w, 
                     Soldier* s, const vec2i& new_pos) {

      vec2i* pos = &s->pos;
      buff[IT(pos->x, pos->y, w)] = Type::Space;

      if (pos->x != new_pos.x) {

        int32 n_x = new_pos.x - pos->x;
        n_x /= std::abs(n_x);

        if (buff[IT(pos->x + n_x, pos->y, w)] == Type::Space) {
          pos->x += n_x; 
          buff[IT(pos->x, pos->y, w)] = s->type;
          return;
        }
      } 
      
      if (pos->y != new_pos.y) {

        int32 n_y = new_pos.y - pos->y;
        n_y /= std::abs(n_y);

        if (buff[IT(pos->x, pos->y + n_y, w)] == Type::Space) {
          pos->y += n_y; 
          buff[IT(pos->x, pos->y, w)] = s->type;
          return;
        }
      }
    }

    static bool CanAttack(Type* buff, const uint32 w, Soldier* s, 
                           const std::vector<Soldier*>& soldiers) {

      if (InternalSMove(buff, w, s, -1, 0, soldiers)) {
        return true;
      }

      if (InternalSMove(buff, w, s, 1, 0, soldiers)) {
        return true;
      }

      if (InternalSMove(buff, w, s, 0, -1, soldiers)) {
        return true;
      }

      if (InternalSMove(buff, w, s, 0, 1, soldiers)) {
        return true;
      }

      return false;
    }

    static bool FreeSpace(Type* buff, uint32 w, const vec2i& pos,
                          int32 x, int32 y, vec2i* out) {
      (*out) = {pos.x + x, pos.y + y};
      if (buff[IT(out->x, out->y, w)] == Type::Space)
        return true;

      return false;
    }
    
    static void Attack(Type* buff, uint32 w, Soldier* s, 
                       std::vector<Soldier*>* elves, 
                       std::vector<Soldier*>* goblins, 
                       std::vector<Soldier*>* soldiers,
                       std::vector<Soldier*>* delete_pool) {

      s->enemy->life -= 3;

      if (s->enemy->life <= 0) {

        const vec2i& e_pos = s->enemy->pos;
        if (s->enemy->type == Type::Elf) {
          RemoveFromPool(elves, e_pos);
        } else {
          RemoveFromPool(goblins, e_pos);
        }
        RemoveFromPool(soldiers, e_pos);
        delete_pool->push_back(s->enemy);

        buff[IT(e_pos.x, e_pos.y, w)] = Type::Space;
        s->enemy = nullptr;
      }
    }

    const char* test[] = {
      "#######",
      "#.G...#",
      "#...EG#",
      "#.#.#G#",
      "#..G#E#",
      "#.....#",
      "#######",
    };

#define INPUT test 

    void puzzle() {

      const uint32 w = (uint32)strlen(INPUT[0]);
      const uint32 h = COUNT_ARR(INPUT);
      const uint32 buff_size = w * h;

      std::vector<Soldier*> goblins;
      std::vector<Soldier*> elves;
      std::vector<Soldier*> soldiers;
      std::vector<Soldier*> delete_pool;
      
      Type* buff = new Type[buff_size];

      for (uint32 y = 0; y < h; ++y) {
        for (uint32 x = 0; x < w; ++x) {

          const char c = INPUT[y][x];
          Type cell = Type::Wall;

          if (c == '.') {
            cell = Type::Space;
          } else if (c == 'G') {
            cell = Type::Goblin;
            Soldier* s = new Soldier(cell, {(int32)x,(int32)y});
            goblins.push_back(s);
            soldiers.push_back(s);
          } else if (c == 'E') {
            cell = Type::Elf;
            Soldier* s = new Soldier(cell, {(int32)x,(int32)y});
            elves.push_back(s);
            soldiers.push_back(s);
          }
          
          buff[IT(x, y, w)] = cell;
        }
      }

      uint32 count_turns = 0;
      for (;;) {

        //Print
        LOG("ROUNDS: %d", count_turns);
        for (uint32 y = 0; y < h; ++y) {
          for (uint32 x = 0; x < w; ++x) {
            const Type t = buff[IT(x, y, w)];
            const char c = t == Type::Elf ? 'E' : t == Type::Goblin ? 'G' : t == Type::Wall ? '#' : '.';
            PRINT("%c", c);
          }
          PRINT("\n");
        }
        {
          const uint32 size = goblins.size();
          for (uint32 i = 0; i < size; ++i) {
            Soldier* s = goblins[i];
            PRINT("G(%d, %d): %d ", s->pos.x, s->pos.y, s->life);
          }
          PRINT("\n");
        }

        {
          const uint32 size = elves.size();
          for (uint32 i = 0; i < size; ++i) {
            Soldier* s = elves[i];
            PRINT("E(%d, %d): %d ", s->pos.x, s->pos.y, s->life);
          }
          PRINT("\n");
        }


        std::sort(soldiers.begin(), soldiers.end(), [](const Soldier* l, const Soldier* r) {
          return (l->pos.x > r->pos.x) && (l->pos.y < r->pos.y);
        });

        //Check if combat has ended
        if (goblins.size() == 0 || elves.size() == 0)
          break;

        //Move
        uint32 i = 0;
        while (i < soldiers.size()) {

          Soldier* s = soldiers[i];

          std::vector<Soldier*>* enemies = &elves;
          if (s->type == Type::Elf) {
            enemies = &goblins;
          } 

          //Attack
          if (CanAttack(buff, w, s, *enemies)) {
            Attack(buff, w, s, &elves, &goblins, &soldiers, &delete_pool);

          } else {
            std::vector<vec2i> movements;
            const uint32 count_enemies = (uint32)enemies->size();
            for (uint32 j = 0; j < count_enemies; ++j) {
              Soldier* enemy = (*enemies)[j];
              vec2i n_pos = {0, 0};

              if (FreeSpace(buff, w, enemy->pos, -1, 0, &n_pos)) {
                movements.push_back(n_pos);
              } 
              
              if (FreeSpace(buff, w, enemy->pos, 1, 0, &n_pos)) {
                movements.push_back(n_pos);
              } 
              
              if (FreeSpace(buff, w, enemy->pos, 0, -1, &n_pos)) {
                movements.push_back(n_pos);
              } 
              
              if (FreeSpace(buff, w, enemy->pos, 0, 1, &n_pos)) {
                movements.push_back(n_pos);
              }
            }

            const uint32 count_free_spaces = (uint32)movements.size();
            if (count_free_spaces > 0) {
              uint32 min_distance = 99999999;
              for (uint32 k = 0; k < count_free_spaces; ++k) {
                const vec2i& new_pos = movements[k];
                const uint32 d = Manhattan(s->pos, new_pos);
                if (d < min_distance) {
                  min_distance = d;
                }
              }

              if (movements.size() > 1) {
                movements.erase(std::remove_if(movements.begin(), movements.end(), 
                  [min_distance, s](const vec2i& p) {
                  return Manhattan(s->pos, p) > min_distance; 
                }));

                std::sort(movements.begin(), movements.end(), [](const vec2i& l, const vec2i& r) {
                  return (l.x > r.x) && (l.y < r.y);
                });
              }

              const vec2i& dest = movements[0];
              Move(buff, w, s, dest);

              if (CanAttack(buff, w, s, *enemies)) {
                Attack(buff, w, s, &elves, &goblins, &soldiers, &delete_pool);
              }
            }
          }

          ++i;
        }
        ++count_turns;
      }

      LOG("Done in: %d turns.", count_turns);

      const uint32 count_soldiers = (uint32)soldiers.size();
      for (uint32 i = 0; i < count_soldiers; ++i) {
        delete soldiers[i];
      }
      
      const uint32 count_delete = (uint32)delete_pool.size();
      for (uint32 i = 0; i < count_delete; ++i) {
        delete delete_pool[i];
      }
    }
  }
}

