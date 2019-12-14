#include <tiny-intcode.h>

#include <cstdio>
#include <cstdlib>
#include <map>

struct ivec2 {
  int64_t x, y;
};
bool operator == (ivec2 a, ivec2 b) { return a.x == b.x && a.y == b.y; }
bool operator <  (ivec2 a, ivec2 b) { return (a.x != b.x) ? (a.x < b.x) : (a.y < b.y); }
ivec2 min(ivec2 a, ivec2 b) { return ivec2{ std::min(a.x, b.x), std::min(a.y, b.y) }; }
ivec2 max(ivec2 a, ivec2 b) { return ivec2{ std::max(a.x, b.x), std::max(a.y, b.y) }; }


constexpr int64_t kEmpty = 0;
constexpr int64_t kWall = 1;
constexpr int64_t kBlock = 2;
constexpr int64_t kPaddle = 3;
constexpr int64_t kBall = 4;

constexpr char kTile[] = {
  ' ', // empty
  '#', // wall
  '*', // block
  '-', // paddle
  'o', // ball
};


inline static int sign_of(int x)
{
  return (x < 0) ? -1 : (x > 0) ? 1 : 0;
}


struct Game {
  vh::IntcodeComputer src;
  ivec2 ball = ivec2{ 0, 0 };
  ivec2 paddle = ivec2{ 0, 0 };
  int64_t joystick = 0;
  int64_t numBricks = 0;
  int64_t score = 0;
  std::map<ivec2, int64_t> display;


  void update()
  {
    for (size_t i = 0, endI = src.out.size(); i < endI; i += 3) {
      ivec2 pos{ src.out[i], src.out[i+1] };
      int64_t type = src.out[i+2];
      if (pos.x == -1 && pos.y == 0) {
        score = src.out[i+2];
        continue;
      }
      switch (type) {
      case kBall:
        ball = pos;
        if (display.find(ball) != display.end()) {
          // Should always be a brick in this case, I think?
          --numBricks;
        }
        break;
      case kPaddle:
        paddle = pos;
        break;
      case kBlock:
        numBricks++;
        break;
      default:
        break;
      }

      display[pos] = type;
    }

    joystick = sign_of(ball.x - paddle.x);
  }


  void print() {
    ivec2 lo{ 1 << 30, 1 << 30 };
    ivec2 hi{ -(1 << 30), -(1 << 30) };
    for (auto it : display) {
      ivec2 pos = it.first;
      lo = min(lo, pos);
      hi = max(hi, pos);
    }
    for (int y = lo.y; y <= hi.y; y++) {
      for (int x = lo.x; x <= hi.x; x++) {
        ivec2 pos{ x, y };
        int64_t type = display.count(pos) > 0 ? display.at(pos) : kEmpty;
        char tile = kTile[type];
        printf("%c", tile);
      }
      printf("\n");
    }
  }


  void play() {
    src.mem[0] = 2; // play for free, woohoo!
    while (src.run(joystick)) {
      update();
    }
    update();
  }
};


int main(int argc, char** argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <prog.txt>\n", argv[0]);
    return EXIT_FAILURE;
  }

  Game game;
  if (!game.src.load(argv[1])) {
    fprintf(stderr, "Couldn't open %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  game.play();
  printf("Final score = %lld\n", game.score);
//  printf("Num blocks = %u\n", uint32_t(display.size()));
  return EXIT_SUCCESS;
}
