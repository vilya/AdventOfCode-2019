#include <tiny-intcode.h>

#include <cstdio>
#include <cstdlib>
#include <map>

struct ivec2 {
  int x, y;
};
ivec2 operator + (ivec2 a, ivec2 b) { return ivec2{ a.x + b.x, a.y + b.y }; }
ivec2 operator - (ivec2 a, ivec2 b) { return ivec2{ a.x - b.x, a.y - b.y }; }
ivec2 operator / (ivec2 a, int k) { return ivec2{ a.x / k, a.y / k }; }
bool operator == (ivec2 a, ivec2 b) { return a.x == b.x && a.y == b.y; }
bool operator <  (ivec2 a, ivec2 b) { return (a.x != b.x) ? (a.x < b.x) : (a.y < b.y); }


static constexpr int64_t kBlack = 0;
static constexpr int64_t kWhite = 1;

static constexpr int64_t kTurnLeft = 0;
static constexpr int64_t kTurnRight = 1;

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

  std::map<ivec2, int64_t> grid;
  ivec2 pos{ 0, 0 };
  ivec2 facing{ 0, -1 }; // facing up

  int64_t output = 0;
  int result = 1;
  while (true) {
    int64_t input = (grid.count(pos) > 0) ? grid[pos] : kBlack;
    result = src.run(input, output);
    if (result == 0) {
      break;
    }
    assert(result == 2);
    grid[pos] = output;
    result = src.run(input, output);
    if (result == 0) {
      break;
    }
    assert(result == 2);
    if (output == kTurnLeft) {
      facing = ivec2{ facing.y, -facing.x };
    }
    else {
      facing = ivec2{ -facing.y, facing.x };
    }
    pos = pos + facing;
  }
  printf("Painted %u panels at least once\n", uint32_t(grid.size()));
  return EXIT_SUCCESS;
}
