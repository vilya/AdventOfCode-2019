#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <string>


//
// Helper types, constants and functions.
//

struct ivec2 {
  int64_t x, y;
};
bool operator == (ivec2 a, ivec2 b) { return a.x == b.x && a.y == b.y; }
bool operator != (ivec2 a, ivec2 b) { return a.x != b.x || a.y != b.y; }
bool operator <  (ivec2 a, ivec2 b) { return (a.x != b.x) ? (a.x < b.x) : (a.y < b.y); }


constexpr ivec2 kDelta[] = {
  ivec2{  0, -1 }, // north = 0 = 0b00
  ivec2{  0,  1 }, // south = 1 = 0b01
  ivec2{ -1,  0 }, // west  = 2 = 0b10
  ivec2{  1,  0 }, // east  = 3 = 0b11
};

constexpr int kTooFar = 1000000;

static inline ivec2 step(ivec2 pos, int direction) { return ivec2{ pos.x + kDelta[direction].x, pos.y + kDelta[direction].y }; }
static inline int opposite(int direction) { return direction ^ 1; }


//
// Map type
//

bool is_label_part(char ch) { return ch >= 'A' && ch <= 'Z'; }

struct Map {
  std::vector<std::string> grid;
  std::map<ivec2, ivec2> portals;
  ivec2 size{ 0, 0 };
  ivec2 entrance{ 0, 0 };
  ivec2 exit{ 0, 0 };


  char at(ivec2 pos) const {
    return (pos.x < 0 || pos.x >= size.x || pos.y < 0 || pos.y >= size.y) ? ' ' : grid[pos.y][pos.x];
  }


  bool load(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (f == nullptr) {
      return false;
    }

    const int bufSize = 255;
    char buf[bufSize];
    while (fgets(buf, bufSize, f) != NULL) {
      grid.push_back(buf);
      if (grid.back().back() == '\n') {
        grid.back().pop_back();
      }
      size.x = std::max(int64_t(grid.back().size()), size.x);
    }
    size.y = int64_t(grid.size());
    fclose(f);

    // Make sure all rows have the same length.
    for (std::string& row : grid) {
      while (row.size() < size.x) {
        row.push_back(' ');
      }
    }

    bool foundEntrance = false;
    bool foundExit = false;

    // Find and mark all labelled points
    std::map<std::string, std::pair<ivec2, ivec2>> portalPosByLabel; // values are the entry position (i.e. the position on the label char) and exit position (i.e. where it puts you when you've arrived through it).
    ivec2 pos{ 0, 0 };
    char label[3];
    label[2] = '\0';
    for (pos.y = 0; pos.y < size.y - 1; pos.y++) {
      for (pos.x = 0; pos.x < size.x - 1; pos.x++) {
        if (!is_label_part(at(pos))) {
          continue;
        }
        int dir = is_label_part(at(step(pos, 3))) ? 3 : is_label_part(at(step(pos, 1))) ? 1 : -1;
        if (dir == -1) {
          continue;
        }

        ivec2 next = step(pos, dir);

        label[0] = at(pos);
        label[1] = at(next);
        if (label[0] == 'A' && label[1] == 'A') {
          entrance = (at(step(next, dir)) == '.') ? step(next, dir) : step(pos, opposite(dir));
          foundEntrance = true;
          continue;
        }
        else if (label[0] == 'Z' && label[1] == 'Z') {
          exit = (at(step(next, dir)) == '.') ? step(next, dir) : step(pos, opposite(dir));
          foundExit = true;
          continue;
        }

        ivec2 portalEntry = (at(step(next, dir)) == '.') ? next : pos;
        ivec2 portalExit = (at(step(next, dir)) == '.') ? step(next, dir) : step(pos, opposite(dir));
        printf("Found portal %s at {%3lld,%3lld}\n", label, portalEntry.x, portalEntry.y);
        auto it = portalPosByLabel.find(label);
        if (it == portalPosByLabel.end()) {
          portalPosByLabel[label] = std::make_pair(portalEntry, portalExit);
        }
        else {
          ivec2 otherEntry = it->second.first;
          ivec2 otherExit = it->second.second;
          portals[otherEntry] = portalExit;
          portals[portalEntry] = otherExit;
          portalPosByLabel.erase(label);
        }
      }
    }

    return foundEntrance && foundExit && portalPosByLabel.empty();
  }


  struct Work {
    ivec2 pos;
    int distance;
  };


  int solve() {
    std::set<ivec2> visited;

    std::queue<Work> q;
    q.push(Work{entrance, 0});

    while (!q.empty()) {
      Work work = q.front();
      q.pop();

      if (work.pos == exit) {
        return work.distance;
      }

      if (!visited.insert(work.pos).second) {
        continue;
      }

      for (int dir = 0; dir < 4; dir++) {
        ivec2 next = step(work.pos, dir);
        char nextch = at(next);
        if (nextch == '.') {
          q.push(Work{ next, work.distance + 1 });
        }
        else if (is_label_part(nextch) && portals.count(next) > 0) {
          q.push(Work{ portals.at(next), work.distance + 1 });
        }
      }
    }

    return kTooFar;
  }


  void print() {
    for (const std::string& row : grid) {
      printf("%s\n", row.c_str());
    }
  }
};


int main(int argc, char** argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <prog.txt>\n", argv[0]);
    return EXIT_FAILURE;
  }

  Map map;
  if (!map.load(argv[1])) {
    fprintf(stderr, "Couldn't open %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  map.print();
  fflush(stdout);

  int shortest = map.solve();
  printf("Shortest path is %d steps\n", shortest);

  return EXIT_SUCCESS;
}
