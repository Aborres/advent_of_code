//jnaranjo Advent of code 18

#include <18/8.h>

#include <utils.h>

#include <string.h>

#include <vector>

namespace Y18 {
  namespace D8 {

    class RingBuffer {
    public:
      RingBuffer(uint32 r) : data(nullptr), buff_size(0), num_elem(0), reserve_size(r)  {
        buff_size = reserve_size;
        num_elem = 0;
        data = new uint32[buff_size];
        memset(data, 0, buff_size * sizeof(uint32));
      }

      ~RingBuffer() {
        delete[] data;
      }

      uint32 insert(int32 pos, uint32 o) {
        const uint32 size = this->size();

        checkResize(size + 1);

        fixIT(pos);

        memcpy(data + pos + 1, data + pos, sizeof(uint32) * (size - pos));
        data[pos] = o;
        ++num_elem;

        return pos;
      }

      void remove(int32& pos) {
        fixIT(pos);
        memcpy(data + pos, data + pos + 1, sizeof(uint32) * (size() - pos));
        --num_elem;
      }

      uint32 size() const {
        return num_elem;
      }

      uint32 buffSize() const {
        return buff_size;
      }

      void checkResize(uint32 new_size) {
        if (new_size > buffSize()) {

          buff_size = new_size + reserve_size;

          uint32* tmp = new uint32[buff_size];
          memcpy(tmp, data, sizeof(uint32) * size());

          delete[] data;
          data = tmp;
        }
      }

      uint32& operator[](int32 it) {
        fixIT(it);
        return data[it];
      }

      void fixIT(int32& it) {
        const int32 size = (int32)this->size();
        if (it < 0) {
          it = size + it;
        } else if (it > size) {
          it -= size;
        } 
      }

    private:
      uint32* data;
      uint32 buff_size;
      uint32 num_elem;
      uint32 reserve_size;
    };

    static const char* test[] = {
      "9 players; last marble is worth 25 points",    //high score is 32 
      "10 players; last marble is worth 1618 points", //high score is 8317
      "13 players; last marble is worth 7999 points", //high score is 146373
      "17 players; last marble is worth 1104 points", //high score is 2764
      "21 players; last marble is worth 6111 points", //high score is 54718
      "30 players; last marble is worth 5807 points", //high score is 37305
    };

    void puzzle() {
      UNUSED(test);

      uint32 num_players = 0;
      uint32 last_marble = 0;
      sscanf(input, "%d players; last marble is worth %d points", &num_players, &last_marble);

      RingBuffer buffer(100);

      uint64* player_scores = new uint64[num_players];
      memset(player_scores, 0, sizeof(uint64) * num_players);

      uint32 curr_player = 0;
      int32 curr_pos = buffer.insert(0, 0);
      for (uint32 i = 1; i < last_marble; ++i) {

        if (!(i % 23)) {

          player_scores[curr_player] += i;

          curr_pos -= 7;

          player_scores[curr_player] += buffer[curr_pos];
          buffer.remove(curr_pos);

        } else {
          curr_pos = buffer.insert(curr_pos + 2, i);
        }

        ++curr_player;
        curr_player %= num_players;
      }

      uint64 max_points = 0;
      for (uint32 i = 0; i < num_players; ++i) {
        if (player_scores[i] > max_points) {
          max_points = player_scores[i];
          curr_player = i;
        }
      }

      LOG("The Max score was: %llu by player: %u", max_points, curr_player);

      delete[] player_scores;
    }
  }
}

