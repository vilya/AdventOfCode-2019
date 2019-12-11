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
  while (src.run(1) != 0) {
    printf("%lld,", src.out[0]);
  }
  printf("\n");

  return EXIT_SUCCESS;
}
