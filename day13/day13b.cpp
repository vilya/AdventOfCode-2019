#include <tiny-intcode.h>

#include <cstdio>

constexpr int64_t kPaddle = 3;
constexpr int64_t kBall = 4;


struct Game {
  vh::IntcodeComputer src;
  int64_t ballx = 0;
  int64_t paddlex = 0;
  int64_t joystick = 0;
  int64_t score = 0;

  void update() {
    for (size_t i = 0, endI = src.out.size(); i < endI; i += 3) {
      if (src.out[i] == -1 && src.out[i+1] == 0) {
        score = src.out[i+2];
      }
      else if (src.out[i+2] == kPaddle) {
        paddlex = src.out[i];
      }
      else if (src.out[i+2] == kBall) {
        ballx = src.out[i];
      }
    }
    joystick = (paddlex < ballx) ? 1 : (paddlex > ballx) ? -1 : 0;
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
  return EXIT_SUCCESS;
}
