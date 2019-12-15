#include <tiny-intcode.h>

#include <cstdio>
#include <cstdlib>
#include <map>
#include <vector>


struct ivec2 {
  int64_t x, y;

  ivec2& operator += (ivec2 a) { x += a.x; y += a.y; return *this; }
};
bool operator == (ivec2 a, ivec2 b) { return a.x == b.x && a.y == b.y; }
bool operator <  (ivec2 a, ivec2 b) { return (a.x != b.x) ? (a.x < b.x) : (a.y < b.y); }

ivec2 operator + (ivec2 a, ivec2 b) { return ivec2{ a.x + b.x, a.y + b.y }; }


constexpr ivec2 kDelta[] = {
  ivec2{  0, -1 }, // north = 0 = 0b00
  ivec2{  0,  1 }, // south = 1 = 0b01
  ivec2{ -1,  0 }, // west  = 2 = 0b10
  ivec2{  1,  0 }, // east  = 3 = 0b11
};


static inline uint32_t opposite(uint32_t direction)
{
  return direction ^ 1;
}


struct Data {
  vh::IntcodeComputer prog;
  std::map<ivec2, int> depths;
  ivec2 target{ 0, 0 };


  void search() {
    ivec2 pos{ 0, 0 };
    int depth = 0;
    std::vector<int> directions;
    directions.reserve(1000);

    depths[pos] = 0;
    directions.push_back(-1);
    while (true) {
      int direction = ++directions.back();
      if (direction == 4) {
        directions.pop_back();
        if (directions.empty()) {
          return;
        }

        int opp = opposite(directions.back());
        prog.run(opp + 1);
        pos += kDelta[opp];
        --depth;
        continue;
      }

      ivec2 next = pos + kDelta[direction];
      auto it = depths.find(next);
      if (it != depths.end() && it->second <= depth) {
        continue;
      }

      prog.run(direction + 1);
      if (prog.out[0] == 0) {
        depths[next] = -1;
        continue;
      }

      directions.push_back(-1);
      ++depth;
      depths[next] = depth;
      pos = next;
      if (prog.out[0] == 2) {
        target = next;
      }
    }
  }
};




int main(int argc, char** argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <prog.txt>\n", argv[0]);
    return EXIT_FAILURE;
  }

  Data data;
  if (!data.prog.load(argv[1])) {
    fprintf(stderr, "Couldn't open %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  data.search();
  printf("shortest path to target has length %d\n", data.depths[data.target]);
  return EXIT_SUCCESS;
}
