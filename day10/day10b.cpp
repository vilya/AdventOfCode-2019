#define _USE_MATH_DEFINES

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <set>
#include <vector>

int gcd(int x, int y)
{
  if (y == 0) {
    return x;
  }
  return gcd(y, x % y);
}


struct ivec2 {
  int x, y;

  ivec2(int k) : x(k), y(k) {}
  ivec2(int xx, int yy) : x(xx), y(yy) {}
};
ivec2 operator + (ivec2 a, ivec2 b) { return ivec2{ a.x + b.x, a.y + b.y }; }
ivec2 operator - (ivec2 a, ivec2 b) { return ivec2{ a.x - b.x, a.y - b.y }; }
ivec2 operator / (ivec2 a, int k) { return ivec2{ a.x / k, a.y / k }; }
bool operator == (ivec2 a, ivec2 b) { return a.x == b.x && a.y == b.y; }
bool operator <  (ivec2 a, ivec2 b) { return (a.x != b.x) ? (a.x < b.x) : (a.y < b.y); }
int length(ivec2 a) { return gcd(std::abs(a.x), std::abs(a.y)); }
ivec2 normalize(ivec2 a) { return a / length(a); }


double to_angle(ivec2 a)
{
  double angle = std::atan2(double(a.x), double(-a.y));
  return (angle >= 0.0) ? angle : (2.0 * M_PI + angle);
}


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
        asteroids.push_back(ivec2(x, y));
      }
      x++;
    }
    w = x;
    ++y;
  }
  fclose(f);

  return true;
}


bool polar_order(ivec2 a, ivec2 b)
{
  if (a == b) {
    return false;
  }
  int a_dist = length(a);
  int b_dist = length(b);
  ivec2 a_dir = a / a_dist;
  ivec2 b_dir = b / b_dist;
  if (a_dir == b_dir) {
    return a_dist < b_dist;
  }
  else {
    return to_angle(a_dir) < to_angle(b_dir);
  }
}


size_t best_asteroid(const std::vector<ivec2>& asteroids)
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

  const size_t n = asteroids.size() - 1;
  size_t bestI = best_asteroid(asteroids);
  printf("base is at {%d,%d}\n", asteroids[bestI].x, asteroids[bestI].y);

  std::vector<ivec2> directions;
  directions.reserve(n);
  for (size_t i = 0; i <= n; i++) {
    if (i == bestI) {
      continue;
    }
    directions.push_back(asteroids[i] - asteroids[bestI]);
  }
  std::sort(directions.begin(), directions.end(), polar_order);

  std::vector<bool> removed(directions.size(), false);
  size_t i = 0, num_removed = 0;
  while (num_removed < 200) {
    while (i < n && removed[i]) {
      ++i;
    }
    removed[i] = true;
    ++num_removed;
    ivec2 pos = directions[i] + asteroids[bestI];
    if (num_removed == 200) {
      printf("The 200th asteroid to be vaporized is at %d,%d\n", pos.x, pos.y);
      break;
    }

    size_t j = i + 1;
    while (j < n && (removed[j] || normalize(directions[j]) == normalize(directions[i]))) {
      ++j;
    }
    i = j % n;
  }

  return 0;
}
