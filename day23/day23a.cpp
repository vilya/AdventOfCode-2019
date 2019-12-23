#include <tiny-intcode.h>

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

  std::vector<vh::IntcodeComputer> computers(50, src);
  std::vector<std::deque<int64_t>> inputs(50, std::deque<int64_t>());

  for (int64_t i = 0, endI = int64_t(computers.size()); i < endI; i++) {
    computers[i].run(i);
  }
  while (true) {
    // Process outputs
    for (int64_t i = 0, endI = int64_t(computers.size()); i < endI; i++) {
      for (int64_t j = 0, endJ = int64_t(computers[i].out.size()); j < endJ; j += 3) {
        int64_t dest = computers[i].out[j];
        int64_t x = computers[i].out[j+1];
        int64_t y = computers[i].out[j+2];
        if (dest == 255) {
          printf("First y value sent to address 255 is %lld\n", y);
          return EXIT_SUCCESS;
        }
        inputs[dest].push_back(x);
        inputs[dest].push_back(y);
      }
    }

    // Run with any new inputs
    for (int64_t i = 0, endI = int64_t(computers.size()); i < endI; i++) {
      if (inputs[i].empty()) {
        computers[i].run(-1);
      }
      else {
        computers[i].run(inputs[i]);
      }
    }
  }

  return EXIT_FAILURE;
}
