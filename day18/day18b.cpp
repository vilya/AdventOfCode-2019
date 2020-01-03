#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <vector>
#include <queue>
#include <set>

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

static inline bool is_key(char ch)  { return ch >= 'a' && ch <= 'z'; }
static inline bool is_door(char ch) { return ch >= 'A' && ch <= 'Z'; }
static inline int keyidx(char ch)  { assert(is_key(ch));  return ch - 'a'; }
static inline int dooridx(char ch) { assert(is_door(ch)); return ch - 'A'; }
static inline uint32_t keybit(char ch)  { return 1u << keyidx(ch); }
static inline uint32_t doorbit(char ch) { return 1u << dooridx(ch); }
static inline bool has_key_for_door(uint32_t keys, char door) { return (keys & doorbit(door)) != 0; }
static inline ivec2 step(ivec2 pos, int direction) { return ivec2{ pos.x + kDelta[direction].x, pos.y + kDelta[direction].y }; }


//
// Map type
//

struct Map {
  std::map<ivec2,char> tiles;
  ivec2 size{ 0, 0 };
  ivec2 initialEntrance{ 0, 0 };
  ivec2 entrances[4];
  ivec2 keys[26];
  int nkeys = 0;


  inline bool contains(ivec2 pos) const {
    return tiles.find(pos) != tiles.end();
  }


  inline char at(ivec2 pos) const {
    auto it = tiles.find(pos);
    return (it != tiles.end()) ? it->second : '#';
  }


  bool load(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (f == nullptr) {
      return false;
    }
    ivec2 pos{ 0, 0 };
    int tmp;
    bool foundEntrance = false;
    while ((tmp = fgetc(f)) != EOF) {
      char ch = static_cast<char>(tmp);
      if (ch == '\n') {
        size.x = std::max(size.x, pos.x);
        pos.x = 0;
        pos.y++;
        continue;
      }

      if (ch == '@') {
        initialEntrance = pos;
        foundEntrance = true;
      }
      else if (is_key(static_cast<char>(tmp))) {
        keys[keyidx(ch)] = pos;
        ++nkeys;
      }
      else if (ch != '.' && !is_door(ch)) {
        // Deliberately not storing wall tiles. Everything in `tiles` is a location we can visit.
        pos.x++;
        continue;
      }

      tiles[pos] = ch;
      pos.x++;
    }
    size.y = pos.y;

    if (foundEntrance) {
      tiles.erase(initialEntrance);
      for (int direction = 0; direction < 4; direction++) {
        tiles.erase(step(initialEntrance, direction));
      }
      entrances[0] = step(step(initialEntrance, 0), 2);
      entrances[1] = step(step(initialEntrance, 0), 3);
      entrances[2] = step(step(initialEntrance, 1), 2);
      entrances[3] = step(step(initialEntrance, 1), 3);
      for (int i = 0; i < 4; i++) {
        tiles[entrances[i]] = '@';
      }
    }

    return foundEntrance;
  }


  // A dead end is any location where we can't reach a key without backtracking.
  // Return value is true if there's a key at the end of the path, false if it's a dead end.
  void remove_dead_ends(ivec2 prev, ivec2 pos, std::set<ivec2>& visited) {
    if (!contains(pos)) {
      return;
    }
    if (!visited.insert(pos).second) {
      return;
    }
    for (int direction = 0; direction < 4; direction++) {
      ivec2 next = step(pos, direction);
      if (next != prev) {
        remove_dead_ends(pos, next, visited);
      }
    }
    if (is_key(at(pos))) {
      return;
    }
    for (int direction = 0; direction < 4; direction++) {
      ivec2 next = step(pos, direction);
      if (next != prev && contains(next)) {
        return;
      }
    }
    tiles.erase(pos);
  }


  void remove_all_dead_ends() {
    std::set<ivec2> visited;
    for (int i = 0; i < 4; i++) {
      visited.clear();
      remove_dead_ends(entrances[i], entrances[i], visited);
    }
  }


  struct SolveKey {
    ivec2 pos;
    uint32_t visited;

    bool operator == (const SolveKey& other) const { return pos == other.pos && visited == other.visited; }
    bool operator != (const SolveKey& other) const { return pos != other.pos || visited != other.visited; }
    bool operator <  (const SolveKey& other) const { return (pos != other.pos) ? (pos < other.pos) : (visited < other.visited); }
  };


  struct Work {
    ivec2 pos;
    uint32_t visited;
    int distance;
  };


  int solve_quadrant(ivec2 entrance, uint32_t startingKeys) {
    const uint32_t allKeys = (1u << nkeys) - 1;

    std::map<SolveKey, int> state; // values are the distance travelled.

    std::queue<Work> q;
    q.push(Work{entrance, startingKeys, 0});

//    int64_t progress = 0;
    while (!q.empty()) {
      Work work = q.front();
      q.pop();

      char ch = at(work.pos);
      if (is_key(ch)) {
        uint32_t bit = keybit(ch);
        work.visited |= bit;
        if (work.visited == allKeys) {
          return work.distance;
        }
      }
      else if (is_door(ch) && !has_key_for_door(work.visited, ch)) {
        continue;
      }

      SolveKey stateKey{ work.pos, work.visited };
      auto stateIt = state.find(stateKey);
      int stateDist = (stateIt != state.end()) ? stateIt->second : kTooFar;
      if (work.distance >= stateDist) {
        continue;
      }

      state[stateKey] = work.distance;

      ++work.distance;
      for (int dir = 0; dir < 4; dir++) {
        ivec2 next = step(work.pos, dir);
        if (contains(next)) {
          q.push(Work{next, work.visited, work.distance});
        }
      }

//      ++progress;
//      if (progress % 1000 == 0) {
//        printf("\r%lld", progress / 1000);
//        fflush(stdout);
//      }
    }
//    printf("\n");

    return kTooFar;
  }


  int solve() {
    uint32_t keysByQuadrant[4] = { 0u, 0u, 0u, 0u };
    for (int i = 0; i < nkeys; i++) {
      uint32_t idx = 0;
      idx |= (keys[i].y > initialEntrance.y) ? 2u : 0u;
      idx |= (keys[i].x > initialEntrance.x) ? 1u : 0u;
      keysByQuadrant[idx] |= (1u << i);
    }

    int totalDist = 0;
    const uint32_t allKeys = (1u << nkeys) - 1;
    for (int i = 0; i < 4; i++) {
      uint32_t startingKeys = allKeys ^ keysByQuadrant[i];
      totalDist += solve_quadrant(entrances[i], startingKeys);
    }
    return totalDist;
  }


  void print() {
    ivec2 pos{ 0, 0 };
    for (pos.y = 0; pos.y < size.y; pos.y++) {
      for (pos.x = 0; pos.x < size.x; pos.x++) {
        auto it = tiles.find(pos);
        fputc((it != tiles.end()) ? it->second : ' ', stdout);
      }
      fputc('\n', stdout);
    }
    fputc('\n', stdout);

    for (int i = 0; i < nkeys; i++) {
      printf("key %c: {%3lld,%3lld}\n", 'a' + i, keys[i].x, keys[i].y);
    }
    printf("%d keys total\n", nkeys);
    printf("Grid size is %lldx%lld\n", size.x, size.y);
    printf("There are %lld visitable locations\n", int64_t(tiles.size()));
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

  map.remove_all_dead_ends();

  map.print();
  fflush(stdout);

  int shortest = map.solve();
  printf("Shortest path which finds all keys is %d steps\n", shortest);

  return EXIT_SUCCESS;
}
