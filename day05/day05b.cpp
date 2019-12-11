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

  prog.run(5);
  bool passed = !prog.out.empty();
  int64_t diagnostic = prog.out.back();
  prog.out.pop_back();
  printf("test results:");
  for (int64_t x : prog.out) {
    if (x != 0) {
      passed = false;
    }
    printf(" %lld", x);
  }
  printf("\n");

  printf("tests %s\n", passed ? "passed" : "failed");
  printf("diagnostic code = %lld\n", diagnostic);

  return passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
