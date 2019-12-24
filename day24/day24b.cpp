#include <cassert>
#include <cstdio>
#include <cstdint>
#include <cstring>


constexpr int kWidth = 5;
constexpr int kHeight = 5;
constexpr int kMaxDepth = 1000;
constexpr int kNumLevels = kMaxDepth * 2 + 1;

constexpr int kDelta[4][2] = {
  { -1,  0 },
  {  1,  0 },
  {  0, -1 },
  {  0,  1 },
};

constexpr int kLeft  = 0;
constexpr int kRight = 1;
constexpr int kUp    = 2;
constexpr int kDown  = 3;


struct Grid {
  uint32_t vals[kNumLevels];
  int activeLo = kMaxDepth;
  int activeHi = kMaxDepth;


  Grid() {
    std::memset(vals, 0, sizeof(vals));
  }


  int num_bugs(int x, int y, int level) const {
    assert(level >= 0 && level < kNumLevels);
    assert(x >= 0 && x < kWidth && y >= 0 && y < kHeight);
    assert(!(x == 2 && y == 2));

    uint32_t bit = 1u << (y * kWidth + x);
    return (vals[level] & bit) ? 1 : 0;
  }


  int adjacent_bugs(int x, int y, int level) const {
    assert(level >= 0 && level < kNumLevels);
    assert(x >= 0 && x < kWidth && y >= 0 && y < kHeight);
    assert(!(x == 2 && y == 2));

    int bugs = 0;
    for (int direction = kLeft; direction <= kDown; direction++) {
      int ax = x + kDelta[direction][0];
      int ay = y + kDelta[direction][1];

      if (ax == 2 && ay == 2) {
        switch (direction) {
        case kLeft:
          for (int i = 0; i < kHeight; i++) {
            bugs += num_bugs(kWidth - 1, i, level + 1);
          }
          break;
        case kRight:
          for (int i = 0; i < kHeight; i++) {
            bugs += num_bugs(0, i, level + 1);
          }
          break;
        case kUp:
          for (int i = 0; i < kHeight; i++) {
            bugs += num_bugs(i, kHeight - 1, level + 1);
          }
          break;
        case kDown:
          for (int i = 0; i < kHeight; i++) {
            bugs += num_bugs(i, 0, level + 1);
          }
          break;
        default:
          break;
        }
      }
      else if (ax == -1) {
        bugs += num_bugs(1, 2, level - 1);
      }
      else if (ax == kWidth) {
        bugs += num_bugs(3, 2, level - 1);
      }
      else if (ay == -1) {
        bugs += num_bugs(2, 1, level - 1);
      }
      else if (ay == kHeight) {
        bugs += num_bugs(2, 3, level - 1);
      }
      else {
        bugs += num_bugs(ax, ay, level);
      }
    }
    return bugs;
  }


  int total_bugs() const {
    int bugs = 0;
    for (int level = activeLo; level <= activeHi; level++) {
      for (int y = 0; y < kHeight; y++) {
        for (int x = 0; x < kWidth; x++) {
          if (x == 2 && y == 2) {
            continue;
          }
          bugs += num_bugs(x, y, level);
        }
      }
    }
    return bugs;
  }


  void set(int x, int y, int level, bool cellval) {
    assert(level >= 0 && level < kNumLevels);
    assert(x >= 0 && x < kWidth && y >= 0 && y < kHeight);
    assert(!(x == 2 && y == 2)); // Can't set the center tile, it's a proxy for the next level up.

    if (x < 0 || x >= kWidth || y < 0 || y >= kHeight || level < 0 || level >= kNumLevels) {
      return;
    }
    uint32_t bit = 1u << (y * kWidth + x);
    if (cellval) {
      vals[level] |= bit;
    }
    else {
      vals[level] &= ~bit;
    }
  }


  bool update_level(const Grid& old, int level) {
    assert(level >= 0 && level < kNumLevels);

    vals[level] = 0u;
    for (int y = 0; y < kHeight; y++) {
      for (int x = 0; x < kWidth; x++) {
        if (x == 2 && y == 2) {
          continue;
        }
        bool hasBug = old.num_bugs(x, y, level) > 0;
        int adjacentBugs = old.adjacent_bugs(x, y, level);
        bool newHasBug = (hasBug && adjacentBugs == 1) ||
                         (!hasBug && adjacentBugs >= 1 && adjacentBugs <= 2);
        set(x, y, level, newHasBug);
      }
    }
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
            fputc((num_bugs(x, y, level) > 0) ? '#' : '.', stdout);
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

//  grid[active].print();
//  printf("There are %d bugs present\n", grid[active].total_bugs());

  for (int i = 0; i < 200; i++) {
    active ^= 1;
    grid[active].update(grid[active ^ 1]);
  }

//  grid[active].print();
  printf("There are %d bugs present\n", grid[active].total_bugs());

  return 0;
}
