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

  vh::IntcodeComputer prog;
  if (!prog.load(argv[1])) {
    fprintf(stderr, "Couldn't open %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  std::queue<int64_t> input;
  input.push(5);

  std::vector<int64_t> output;
  prog.run(input, output);

  bool passed = !output.empty();
  int64_t diagnostic = output.back();
  output.pop_back();
  printf("test results:");
  for (int64_t x : output) {
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
