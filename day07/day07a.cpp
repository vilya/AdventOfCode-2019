#define TINY_INTCODE 1

#include <tiny-intcode.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <vector>


int main(int argc, char** argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <prog.txt>\n", argv[0]);
    return EXIT_FAILURE;
  }

  vh::IntcodeComputer src;
  if (!src.load(argv[1])) {
    fprintf(stderr, "Couldn't open %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  std::vector<int> perm{ 0, 1, 2, 3, 4 };
  int64_t maxOutput = 0;
  do {
    int64_t output = 0;
    for (int phase : perm) {
      vh::IntcodeComputer prog = src;
      prog.run(phase);
      prog.run(output);
      output = prog.out[0];
    }
    if (output > maxOutput) {
      maxOutput = output;
    }
  } while (std::next_permutation(perm.begin(), perm.end()));

  printf("Max output = %lld\n", maxOutput);
  return EXIT_SUCCESS;
}
