#include <tiny-intcode.h>

#include <cstdio>
#include <cstdlib>

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

  int64_t total = 0;
  std::deque<int64_t> coords;
  for (int64_t y = 0; y < 50; y++) {
    for (int64_t x = 0; x < 50; x++) {
      coords.clear();
      coords.push_back(x);
      coords.push_back(y);

      vh::IntcodeComputer tmp = prog;
      tmp.run(coords);
      total += tmp.out[0];
//      fputc(tmp.out[0] ? '#' : '.', stdout);
    }
//    fputc('\n', stdout);
  }

  printf("%lld\n", total);
  return EXIT_SUCCESS;
}
