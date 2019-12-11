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

  std::vector<int> perm{ 5, 6, 7, 8, 9 };
  int maxOutput = 0;
  do {
//    printf("perm = { %d, %d, %d, %d, %d }\n", perm[0], perm[1], perm[2], perm[3], perm[4]);

    vh::IntcodeComputer prog[5] = { src, src, src, src, src };

#if TINY_INTCODE
    int64_t status, amount = 0;
    for (size_t i = 0; i < 5; i++) {
      prog[i].run(perm[i]);
    }
    do {
      for (size_t i = 0; i < 5; i++) {
        status = prog[i].run(amount);
        amount = prog[i].out[0];
      }
    } while (status != 0);
#else
    vh::Status status;
    std::vector<int64_t> output;
    int amount = 0;
    bool hasPhase = false;
    do {
      for (size_t i = 0; i < 5; i++) {
        int phase = perm[i];
//        printf("  phase = %d, input = %d --> ", phase, amount);
        std::queue<int64_t> input;
        if (!hasPhase) {
          input.push(phase);
        }
        input.push(amount);
        status = prog[i].run(input, output);
        amount = output.back();
//        printf("output = %d\n", amount);
      }
      hasPhase = true;
    } while (status != vh::Status::Halted);
#endif

    if (amount > maxOutput) {
      maxOutput = amount;
    }
  } while (std::next_permutation(perm.begin(), perm.end()));

  printf("Max output = %d\n", maxOutput);
  return EXIT_SUCCESS;
}
