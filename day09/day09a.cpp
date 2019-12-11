// #include <intcode.h>
#include <tiny-intcode.h>

#include <cstdio>
#include <cstdlib>

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
  int64_t output = 0;
  int result = 1;
  while (result != 0) {
    result = src.run(1, output);
    if (result == 2) {
      printf("%lld,", output);
    }
  }
  printf("\n");

  return EXIT_SUCCESS;
}
