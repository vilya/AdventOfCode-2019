#include <cassert>
#include <cstdio>
#include <cstdint>
#include <cstring>

#ifdef _WIN32
  #include <intrin.h>
  #define popcnt(x) static_cast<int>(__popcnt(x))
#else
  #define popcnt(x) __builtin_popcount(x)
#endif

constexpr int kWidth = 5;
constexpr int kHeight = 5;
constexpr int kMaxDepth = 201;
constexpr int kNumLevels = kMaxDepth * 2 + 1;


struct Grid {
  uint32_t vals[kNumLevels];
  int activeLo = kMaxDepth;
  int activeHi = kMaxDepth;


  Grid() {
    std::memset(vals, 0, sizeof(vals));
  }


  Grid(const Grid& other) {
    std::memcpy(vals, other.vals, sizeof(vals));
    activeLo = other.activeLo;
    activeHi = other.activeHi;
  }


  Grid& operator = (const Grid& other) {
    std::memcpy(vals, other.vals, sizeof(vals));
    activeLo = other.activeLo;
    activeHi = other.activeHi;
    return *this;
  }


  int total_bugs() const {
    int bugs = 0;
    for (int level = activeLo; level <= activeHi; level++) {
      bugs += popcnt(vals[level]);
    }
    return bugs;
  }


  void set(int x, int y, int level, bool cellval) {
    assert(level >= 0 && level < kNumLevels);
    assert(x >= 0 && x < kWidth && y >= 0 && y < kHeight);
    assert(!(x == 2 && y == 2)); // Can't set the center tile, it's a proxy for the next level up.

    uint32_t bit = 1u << (y * kWidth + x);
    if (cellval) {
      vals[level] |= bit;
    }
    else {
      vals[level] &= ~bit;
    }
  }


  bool update_level(const Grid& old, int level) {
    assert(level > 0 && level < kNumLevels - 1);

    const uint32_t outer   = old.vals[level - 1];
    const uint32_t current = old.vals[level];
    const uint32_t inner   = old.vals[level + 1];

    const int outerLeft  = popcnt(outer & 0b00000'00000'00010'00000'00000);
    const int outerRight = popcnt(outer & 0b00000'00000'01000'00000'00000);
    const int outerUp    = popcnt(outer & 0b00000'00000'00000'00100'00000);
    const int outerDown  = popcnt(outer & 0b00000'00100'00000'00000'00000);

    const int innerLeftEdge   = popcnt(inner & 0b00001'00001'00001'00001'00001);
    const int innerRightEdge  = popcnt(inner & 0b10000'10000'10000'10000'10000);
    const int innerTopEdge    = popcnt(inner & 0b00000'00000'00000'00000'11111);
    const int innerBottomEdge = popcnt(inner & 0b11111'00000'00000'00000'00000);

    const int adjacent[25] = {
      popcnt(current & 0b00000'00000'00000'00001'00010) + outerUp + outerLeft,
      popcnt(current & 0b00000'00000'00000'00010'00101) + outerUp,
      popcnt(current & 0b00000'00000'00000'00100'01010) + outerUp,
      popcnt(current & 0b00000'00000'00000'01000'10100) + outerUp,
      popcnt(current & 0b00000'00000'00000'10000'01000) + outerUp + outerRight,

      popcnt(current & 0b00000'00000'00001'00010'00001) + outerLeft,
      popcnt(current & 0b00000'00000'00010'00101'00010),
      popcnt(current & 0b00000'00000'00000'01010'00100) + innerTopEdge,
      popcnt(current & 0b00000'00000'01000'10100'01000),
      popcnt(current & 0b00000'00000'10000'01000'10000) + outerRight,

      popcnt(current & 0b00000'00001'00010'00001'00000) + outerLeft,
      popcnt(current & 0b00000'00010'00001'00010'00000) + innerLeftEdge,
      0,
      popcnt(current & 0b00000'01000'10000'01000'00000) + innerRightEdge,
      popcnt(current & 0b00000'10000'01000'10000'00000) + outerRight,

      popcnt(current & 0b00001'00010'00001'00000'00000) + outerLeft,
      popcnt(current & 0b00010'00101'00010'00000'00000),
      popcnt(current & 0b00100'01010'00000'00000'00000) + innerBottomEdge,
      popcnt(current & 0b01000'10100'01000'00000'00000),
      popcnt(current & 0b10000'01000'10000'00000'00000) + outerRight,

      popcnt(current & 0b00010'00001'00000'00000'00000) + outerDown + outerLeft,
      popcnt(current & 0b00101'00010'00000'00000'00000) + outerDown,
      popcnt(current & 0b01010'00100'00000'00000'00000) + outerDown,
      popcnt(current & 0b10100'01000'00000'00000'00000) + outerDown,
      popcnt(current & 0b01000'10000'00000'00000'00000) + outerDown + outerRight,
    };

    uint32_t newval = 0u;
    for (int i = 0; i < 25; ++i) {
      if (adjacent[i] == 1) {
        newval |= (1u << i);
      }
      else if (adjacent[i] == 2) {
        newval |= (~current & (1u << i));
      }
    }
    vals[level] = newval;
    return vals[level] != 0u;
  }


  void update(const Grid& old) {
    activeLo = old.activeLo;
    activeHi = old.activeHi;
    for (int level = activeLo; level <= activeHi; level++) {
      update_level(old, level);
    }

    // Do we need to expand the range of active levels...?
    if (activeLo > 0 && update_level(old, activeLo - 1)) {
      --activeLo;
    }
    if (activeHi < (kNumLevels - 1) && update_level(old, activeHi + 1)) {
      ++activeHi;
    }
  }


  void print() const {
    for (int level = activeLo; level <= activeHi; level++) {
      printf("Depth %d:\n", level - kMaxDepth);
      for (int y = 0; y < kHeight; y++) {
        for (int x = 0; x < kWidth; x++) {
          if (x == 2 && y == 2) {
            fputc('?', stdout);
          }
          else {
            uint32_t bit = 1u << (y * kWidth + x);
            fputc(((vals[level] & bit) != 0) ? '#' : '.', stdout);
          }
        }
        fputc('\n', stdout);
      }
      fputc('\n', stdout);
    }
    fflush(stdout);
  }
};


int main(int argc, char** argv)
{
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);
    return 1;
  }

  FILE* f = fopen(argv[1], "r");
  if (!f) {
    fprintf(stderr, "Couldn't open %s\n", argv[1]);
    return 1;
  }

  char line[16];
  Grid grid[2];
  int active = 0;
  int y = 0;
  while (fgets(line, sizeof(line), f) != nullptr) {
    for (int x = 0; x < 5; x++) {
      if (x == 2 && y == 2) {
        continue;
      }
      grid[0].set(x, y, grid[0].activeLo, line[x] == '#');
    }
    y++;
  }

  const int minutes = (strcmp(argv[1], "input.txt") == 0) ? 200 : 10;
  for (int i = 1; i <= minutes; i++) {
    active ^= 1;
    grid[active].update(grid[active ^ 1]);
  }

//  grid[active].print();
  printf("There are %d bugs present\n", grid[active].total_bugs());
  return 0;
}
