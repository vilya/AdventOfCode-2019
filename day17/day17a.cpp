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


void print(const std::map<ivec2, char>& map, ivec2 size)
{
  for (int y = 0; y < size.y; y++) {
    for (int x = 0; x < size.x; x++) {
      auto it = map.find(ivec2{x, y});
      if (it != map.end()) {
        fputc(it->second, stdout);
      }
      else {
        fputc('.', stdout);
      }
    }
    fputc('\n', stdout);
  }
}


int main(int argc, char** argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <prog.txt>\n", argv[0]);
    return EXIT_FAILURE;
  }

  vh::IntcodeComputer prog;
  if (!prog.load(argv[1])) {
    fprintf(stderr, "Couldn't open %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  prog.run();

  ivec2 pos{ 0, 0 };
  ivec2 size{ 0, 0 };
  std::map<ivec2, char> map;
  for (int64_t x : prog.out) {
    if (x == '\n') {
      size.x = std::max(pos.x, size.x);
      pos.x = 0;
      pos.y++;
    }
    else if (x == '#') {
      map[pos] = '#';
      pos.x++;
    }
    else {
      pos.x++;
    }
  }
  size.y = pos.y;

  int64_t result = 0;
  for (auto it : map) {
    pos = it.first;
    bool intersection = true;
    for (ivec2 delta : kDelta) {
      ivec2 neighbor = pos + delta;
      if (map.count(neighbor) == 0) {
        intersection = false;
        break;
      }
    }
    if (intersection) {
      map[pos] = 'O';
      result += pos.x * pos.y;
    }
  }

  print(map, size);
  printf("Result = %lld\n", result);
  return EXIT_SUCCESS;
}
