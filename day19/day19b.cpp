#include <tiny-intcode.h>

#include <cstdio>
#include <cstdlib>


struct TractorBeam {
  vh::IntcodeComputer prog;
  vh::IntcodeComputer tmpProg;
  std::deque<int64_t> coords;

  bool beam(int64_t x, int64_t y) {
    coords.clear();
    coords.push_back(x);
    coords.push_back(y);

    tmpProg = prog;
    tmpProg.run(coords);
    return tmpProg.out[0] != 0;
  }
};


int main(int argc, char** argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <prog.txt>\n", argv[0]);
    return EXIT_FAILURE;
  }

  TractorBeam tractor;
  if (!tractor.prog.load(argv[1])) {
    fprintf(stderr, "Couldn't open %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  // position on the left/bottom edge
  int64_t x0 = 3;
  int64_t y0 = 4;

  // position on the right/top edge
  int64_t x1 = 3;
  int64_t y1 = 4;

  int64_t w = x1 - x0 + 1;
  int64_t h = y0 - y1 + 1;

  const int64_t kWidth = 100;
  const int64_t kHeight = 100;

  while (w < kWidth || h < kHeight) {
    while (w < kWidth) {
      x1 += kWidth - w;
      while (!tractor.beam(x1, y1)) {
        ++y1;
      }
      w = x1 - x0 + 1;
      h = y0 - y1 + 1;
    }
    while (h < kHeight) {
      y0 += kHeight - h;
      while (!tractor.beam(x0, y0)) {
        ++x0;
      }
      w = x1 - x0 + 1;
      h = y0 - y1 + 1;
    }
  }

  printf("p0 = {%lld,%lld}, p1 = {%lld,%lld}, answer = %lld\n", x0, y0, x1, y1, x0 * 10000 + y1);
  return EXIT_SUCCESS;
}
