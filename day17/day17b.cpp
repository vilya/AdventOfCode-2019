#include <tiny-intcode.h>

#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>


// This is a copy 'n' paste of day15b for now.

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


inline static constexpr ivec2 turn_left(ivec2 dir)
{
  return ivec2{ dir.y, -dir.x };
}


inline static constexpr ivec2 turn_right(ivec2 dir)
{
  return ivec2{ -dir.y, dir.x };
}


struct Data {
  vh::IntcodeComputer src;
  std::map<ivec2, char> map;
  ivec2 size{ 0, 0 };
  ivec2 robot{ 0, 0 };
  ivec2 facing{ 0, 0 };
  std::vector<int> path;

  static constexpr int kLeft  = -1;
  static constexpr int kRight = -2;


  bool load(const char* filename) {
    if (!src.load(filename)) {
      return false;
    }

    // Use a *copy* of the source program to get the map.
    vh::IntcodeComputer prog = src;
    prog.run();
    ivec2 pos{ 0, 0 };
    char dirCode = '\0';
    for (int64_t v : prog.out) {
      if (v == '\n') {
        size.x = std::max(pos.x, size.x);
        pos.x = 0;
        pos.y++;
      }
      else if (v == '#' || v == '<' || v == '>' || v == '^' || v == 'v') {
        if (v != '#') {
          robot = pos;
          dirCode = static_cast<char>(v);
        }
        map[pos] = '#';
        pos.x++;
      }
      else {
        pos.x++;
      }
    }
    size.y = pos.y;

    // Get the robot's initial direction
    switch (dirCode) {
    case '^': facing = kDelta[0]; break; // north
    case 'v': facing = kDelta[1]; break; // south
    case '<': facing = kDelta[2]; break; // east
    case '>': facing = kDelta[3]; break; // west
    default: break;
    }

    return true;
  }


  void print_map(bool labelled) const {
    if (labelled) {
      printf("     ");
      for (int x = 0; x < size.x; x += 5) {
        printf("%-6d", x);
      }
      printf("\n");
      printf("     ");
      for (int x = 0; x < size.x; x += 5) {
        printf("%-6s", ":");
      }
      printf("\n");
    }

    for (int y = 0; y < size.y; y++) {
      if (labelled) {
        printf("[%2d] ", y);
      }

      for (int x = 0; x < size.x; x++) {
        auto it = map.find(ivec2{x, y});
        if (it != map.end()) {
          fputc(it->second, stdout);
        }
        else {
          fputc('.', stdout);
        }

        if (labelled && x % 5 == 4) {
          fputc(' ', stdout);
        }
      }
      fputc('\n', stdout);

      if (labelled && y % 5 == 4) {
        fputc('\n', stdout);
      }
    }
  }


  void build_path() {
    ivec2 pos = robot;
    ivec2 dir = facing;

    while (true) {
      // Try to move forward
      int forward = 0;
      ivec2 next = pos + dir;
      while (map.count(next) > 0) {
        forward++;
        pos = next;
        next = pos + dir;
      }
      if (forward > 0) {
        path.push_back(forward);
      }

      ivec2 nextdir = turn_left(dir);
      if (map.count(pos + nextdir) > 0) {
        dir = nextdir;
        path.push_back(kLeft);
        continue;
      }

      nextdir = turn_right(dir);
      if (map.count(pos + nextdir) > 0) {
        dir = nextdir;
        path.push_back(kRight);
        continue;
      }

      // If we get here, we've reached the end of the path.
      return;
    }
  }


  void print_path() const {
    if (path.empty()) {
      return;
    }
    bool needComma = false;
    for (int action : path) {
      if (needComma) {
        fputc(',', stdout);
      }
      else {
        needComma = true;
      }

      if (action == kLeft) {
        fputc('L', stdout);
      }
      else if (action == kRight) {
        fputc('R', stdout);
      }
      else {
        printf("%d", action);
      }
    }
    printf("\n");
  }
};


void print_output(std::vector<int64_t>& out)
{
  for (int64_t ch : out) {
    if (ch >= 0 && ch < 256)  {
      fputc(static_cast<char>(ch), stdout);
    }
    else {
      printf("%lld", ch);
    }
  }
}


int main(int argc, char** argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <prog.txt>\n", argv[0]);
    return EXIT_FAILURE;
  }

  Data data;
  if (!data.load(argv[1])) {
    fprintf(stderr, "Couldn't open %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  data.print_map(false);

  data.build_path();
  data.print_path();

  const char* movement = "C,C,A,B,A,B,A,B,B,C\n";
  const char* A = "L,10,L,12,R,8,R,10\n";
  const char* B = "R,10,L,12,R,10\n";
//  std::deque<int64_t> B{ 'L', ',', 10, 'L', 12, 'R', 8, 'R', 10, 'R', 10, 'L', 12, 'R', 10, '\n' };
  const char* C = "L,10,R,8,R,8\n";
  const char* videoFeed = "n\n";

  data.src.mem[0] = 2;
  if (data.src.run() == 0) {
    print_output(data.src.out);
    fprintf(stderr ,"Exit early (initial)\n");
    return EXIT_FAILURE;
  }
  print_output(data.src.out);
  if (data.src.run(movement) == 0) {
    print_output(data.src.out);
    fprintf(stderr ,"Exit early (movement)\n");
    return EXIT_FAILURE;
  }
  print_output(data.src.out);
  if (data.src.run(A) == 0) {
    print_output(data.src.out);
    fprintf(stderr ,"Exit early (A)\n");
    return EXIT_FAILURE;
  }
  print_output(data.src.out);
  if (data.src.run(B) == 0) {
    print_output(data.src.out);
    fprintf(stderr ,"Exit early (B)\n");
    return EXIT_FAILURE;
  }
  print_output(data.src.out);
  if (data.src.run(C) == 0) {
    print_output(data.src.out);
    fprintf(stderr ,"Exit early (C)\n");
    return EXIT_FAILURE;
  }
  data.src.run(videoFeed);
  print_output(data.src.out);
  printf("\n");

  return EXIT_SUCCESS;
}
