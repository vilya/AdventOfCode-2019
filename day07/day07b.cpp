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

  std::vector<int> perm{ 5, 6, 7, 8, 9 };
  int64_t maxOutput = 0;
  do {
    vh::IntcodeComputer prog[5] = { src, src, src, src, src };

    for (size_t i = 0; i < 5; i++) {
      prog[i].run(perm[i]);
    }

    int64_t status = 0, amount = 0;
    do {
      for (size_t i = 0; i < 5; i++) {
        status = prog[i].run(amount);
        amount = prog[i].out[0];
      }
    } while (status != 0);

    if (amount > maxOutput) {
      maxOutput = amount;
    }
  } while (std::next_permutation(perm.begin(), perm.end()));

  printf("Max output = %lld\n", maxOutput);
  return EXIT_SUCCESS;
}
