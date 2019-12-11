#define TINY_INTCODE 1

#if TINY_INTCODE
#include <tiny-intcode.h>
#else
#include <intcode.h>
#include <queue>
#endif

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
//    printf("perm = { %d, %d, %d, %d, %d }\n", perm[0], perm[1], perm[2], perm[3], perm[4]);

#if TINY_INTCODE
    int64_t output = 0;
    for (int phase : perm) {
//      printf("  phase = %d, input = %lld --> ", phase, output);
      vh::IntcodeComputer prog = src;
      prog.run(phase);
      prog.run(output);
      output = prog.out[0];
//      printf("output = %lld\n", output);
    }
    if (output > maxOutput) {
      maxOutput = output;
    }
#else
    std::vector<int64_t> output{ 0 };
    for (int phase : perm) {
//      printf("  phase = %d, input = %lld --> ", phase, output.back());
      vh::IntcodeComputer prog = src;
      std::queue<int64_t> input;
      input.push(phase);
      input.push(output.back());
      output.clear();
      prog.run(input, output);
//      printf("output = %lld\n", output.back());
    }
    if (output.back() > maxOutput) {
      maxOutput = output.back();
    }
#endif
  } while (std::next_permutation(perm.begin(), perm.end()));

  printf("Max output = %lld\n", maxOutput);
  return EXIT_SUCCESS;
}
