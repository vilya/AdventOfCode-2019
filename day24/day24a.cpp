#include <cstdio>
#include <cstdint>
#include <unordered_set>


constexpr int kWidth = 5;
constexpr int kHeight = 5;

struct Grid {
  uint32_t val = 0u;

  bool get(int x, int y) const {
    if (x < 0 || x >= kWidth || y < 0 || y >= kHeight) {
      return false;
    }
    uint32_t bit = 1u << (y * kWidth + x);
    return (val & bit) != 0;
  }

  void set(int x, int y, bool cellval) {
    if (x < 0 || x >= kWidth || y < 0 || y >= kHeight) {
      return;
    }
    uint32_t bit = 1u << (y * kWidth + x);
    if (cellval) {
      val |= bit;
    }
    else {
      val &= ~bit;
    }
  }

  void update(const Grid& oldgrid) {
    val = 0;
    for (int y = 0; y < kHeight; y++) {
      for (int x = 0; x < kWidth; x++) {
        bool hasBug = oldgrid.get(x, y);
        int adjacentBugs = 0;
        adjacentBugs += oldgrid.get(x - 1, y) ? 1 : 0;
        adjacentBugs += oldgrid.get(x + 1, y) ? 1 : 0;
        adjacentBugs += oldgrid.get(x, y - 1) ? 1 : 0;
        adjacentBugs += oldgrid.get(x, y + 1) ? 1 : 0;
        set(x, y, (hasBug && adjacentBugs == 1) || (!hasBug && adjacentBugs > 0 && adjacentBugs <= 2));
      }
    }
  }

  void print() const {
    for (int y = 0; y < kHeight; y++) {
      for (int x = 0; x < kWidth; x++) {
        fputc(get(x, y) ? '#' : '.', stdout);
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
  Grid grid;
  int y = 0;
  while (fgets(line, sizeof(line), f) != nullptr) {
    for (int x = 0; x < 5; x++) {
      grid.set(x, y, line[x] == '#');
    }
    y++;
  }

  std::unordered_set<uint32_t> seen;
  while (seen.insert(grid.val).second) {
    Grid tmp = grid;
    grid.update(tmp);
  }
  printf("First biodiversity value seen twice is %u\n", grid.val);

  return 0;
}
