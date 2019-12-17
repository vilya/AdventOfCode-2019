#include <tiny-intcode.h>

#include <cstdio>
#include <cstdlib>
#include <map>

struct ivec2 {
  int64_t x, y;
};
bool operator == (ivec2 a, ivec2 b) { return a.x == b.x && a.y == b.y; }
bool operator <  (ivec2 a, ivec2 b) { return (a.x != b.x) ? (a.x < b.x) : (a.y < b.y); }


void update(const vh::IntcodeComputer& src, std::map<ivec2, int64_t>& display)
{
  for (size_t i = 0, endI = src.out.size(); i < endI; i += 3) {
    ivec2 pos{ src.out[i], src.out[i+1] };
    if (src.out[i+2] == 2) {// 2 == a block
      display[pos] = src.out[i+2];
    }
    else if (src.out[i+2] == 4) { // 4 == the ball
      display.erase(pos);
    }
  }
}


int main(int argc, char** argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <prog.txt>\n", argv[0]);
    return EXIT_FAILURE;
  }

  vh::IntcodeComputer src;
  if (!src.load(argv[1])) {
    fprintf(stderr, "Couldn't open %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  std::map<ivec2, int64_t> display;
  while (src.run()) {
    update(src, display);
  }
  update(src, display);

  printf("Num blocks = %u\n", uint32_t(display.size()));
  return EXIT_SUCCESS;
}
