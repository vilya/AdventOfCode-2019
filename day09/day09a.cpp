#include <intcode.h>

#include <cstdio>
#include <cstdlib>
#include <queue>
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

  std::queue<int64_t> input;
  input.push(1);
  std::vector<int64_t> output;
  src.init();
  src.run(input, output);

  for (int64_t x : output) {
    printf("%lld,", x);
  }
  printf("\n");
  return EXIT_SUCCESS;
}
