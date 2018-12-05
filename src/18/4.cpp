//jnaranjo Advent of code 18

#include <18/4.h>

#include <utils.h>

#include <string.h>

#include <map>
#include <vector>
#include <algorithm>

namespace Y18 {
  namespace D4 {

    struct Date {
      union {
        struct {
          uint32 y;
          uint32 m;
          uint32 d;
          uint32 h;
          uint32 min;
        };
        uint32 data[5];
      };
    };

    struct Guard {
      uint32 asleep_time;
      uint8 record[60];

      std::vector<Date> dates;
      
      Guard() : asleep_time(0), dates() {
        memset(record, 0, 60);
      };
      ~Guard() {};

      void computeRecord() {
      
        const uint32 num_cicles = (uint32)dates.size();

        for (uint32 i = 0; i < num_cicles; i += 2) {

          const Date& from = dates[i];
          const Date& to = dates[i + 1];

          for (uint32 t = from.min; t < to.min; ++t) {
            record[t] += 1;
          }
        }
      }

      void computeMaxAsleepTime() {
        asleep_time = 0;

        const uint32 num_cicles = (uint32)dates.size();
        for (uint32 i = 0; i < num_cicles; i += 2) {
          asleep_time += dates[i + 1].min - dates[i].min;
        }
      }
    };

    struct ParsingT {
      Date date;
      char text[32];
    };

    void puzzle() {

      const uint32 count = COUNT_ARR(input);

      //PreProcess
      std::vector<ParsingT> parsing_buff;
      parsing_buff.reserve(count);

      for (uint32 i = 0; i < count; ++i) {
        ParsingT p;
        sscanf(input[i], "[%d-%d-%d %d:%d]", &p.date.y, &p.date.m, &p.date.d, 
                                             &p.date.h, &p.date.min);
        memcpy(p.text, input[i] + 19, strlen(input[i] - 19));
        parsing_buff.push_back(p);
      }

      std::sort(parsing_buff.begin(), parsing_buff.end(), 
        [](const ParsingT& l, const ParsingT& r) {

          const Date& l_d = l.date;
          const Date& r_d = r.date;

          for (uint32 i = 0; i < 5; ++i) {

            if (l_d.data[i] == r_d.data[i])
              continue;
            
            return l_d.data[i] < r_d.data[i];
          }

          return l_d.min < r_d.min;
        }
      );
      
      std::map<uint32, Guard> times;

      uint32 current_id = 0;
      for (uint32 i = 0; i < count; ++i) {

        const ParsingT& p = parsing_buff[i];

        const char first_c = p.text[0];

        if (first_c == 'G') {
          sscanf(p.text, "Guard #%d begins shift", &current_id);
        } else if (first_c == 'w') {
          Guard& guard = times[current_id];
          guard.dates.push_back(p.date);
        } else if (first_c == 'f') {
          Guard& guard = times[current_id];
          guard.dates.push_back(p.date);
        } else {
          LOG("Undefined first character: %d", first_c);
        }
      }

      //Distrubute guards over the hour
      uint32 biggest = 0;
      uint32 min = 0;
      uint32 guard_id = 0;
      for (auto& it : times) {
        Guard* guard = &it.second;
        guard->computeRecord();

        for (uint32 i = 0; i < 60; ++i) {
          if (guard->record[i] > biggest) {
            biggest = guard->record[i];
            min = i;
            guard_id = it.first;
          }
        }
      }

      LOG("Part Two: The ID(%d) multiplied by the min(%d) is: %d",
          guard_id, min, guard_id * min);

      //Compute asleep times
      guard_id = 0; 
      uint32 max_time_asleep = 0;
      
      for (auto& it : times) {

        Guard* guard = &it.second;
        guard->computeMaxAsleepTime();

        if (guard->asleep_time > max_time_asleep) {
          max_time_asleep = guard->asleep_time;
          guard_id = it.first;
        }
      }

      //Find most asleep minute in most asleep guard
      biggest = 0;
      min = 0;

      Guard* guard = &times[guard_id];

      for (uint32 i = 0; i < 60; ++i) {
        if (guard->record[i] > biggest) {
          biggest = guard->record[i];
          min = i;
        }
      }

      LOG("Part One: The ID(%d) multiplied by the min(%d) is: %d",
          guard_id, min, guard_id * min);

    }
  }
}

