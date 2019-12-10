#include <cstdio>
#include <set>
#include <vector>

int gcd(int x, int y)
{
  return (y == 0) ? x : gcd(y, x % y);
}


struct ivec2 {
  int x, y;
};
ivec2 operator + (ivec2 a, ivec2 b) { return ivec2{ a.x + b.x, a.y + b.y }; }
ivec2 operator - (ivec2 a, ivec2 b) { return ivec2{ a.x - b.x, a.y - b.y }; }
ivec2 operator / (ivec2 a, int k) { return ivec2{ a.x / k, a.y / k }; }
bool operator == (ivec2 a, ivec2 b) { return a.x == b.x && a.y == b.y; }
bool operator <  (ivec2 a, ivec2 b) {  return (a.x != b.x) ? (a.x < b.x) : (a.y < b.y); }
int length(ivec2 a) { return gcd(std::abs(a.x), std::abs(a.y)); }
ivec2 normalize(ivec2 a) { return a / length(a); }


bool load(const char* filename, std::vector<ivec2>& asteroids, int& w)
{
  FILE* f = fopen(filename, "r");
  if (f == nullptr) {
    return false;
  }

  char line[256];
  int y = 0;
  while (fgets(line, sizeof(line), f) != nullptr) {
    int x = 0;
    while (line[x] != '\n' && line[x] != '\0') {
      if (line[x] == '#') {
        asteroids.push_back(ivec2{x, y});
      }
      x++;
    }
    w = x;
    ++y;
  }
  fclose(f);

  return true;
}


size_t best_asteroid(const std::vector<ivec2>& asteroids, size_t* numVisible)
{
  const size_t n = asteroids.size();
  size_t maxVisible = 0;
  size_t bestI = 0;
  std::set<ivec2> visible;
  for (size_t i = 0; i < n; i++) {
    visible.clear();
    for (size_t j = 0; j < n; j++) {
      if (i == j) {
        continue;
      }
      visible.insert(normalize(asteroids[j] - asteroids[i]));
    }
    if (visible.size() > maxVisible) {
      maxVisible = visible.size();
      bestI = i;
    }
  }

  if (numVisible != nullptr) {
    *numVisible = maxVisible;
  }
  return bestI;
}


int main(int argc, char** argv)
{
  std::vector<ivec2> asteroids;
  int w;
  if (!load(argv[1], asteroids, w)) {
    fprintf(stderr, "Couldn't load %s\n", argv[1]);
    return 1;
  }
  size_t numVisible = 0;
  size_t bestI = best_asteroid(asteroids, &numVisible);
  printf("best result is at {%d,%d} with %u asteroids visible\n",
         asteroids[bestI].x, asteroids[bestI].y, uint32_t(numVisible));
  return 0;
}
