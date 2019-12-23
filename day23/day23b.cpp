#include <tiny-intcode.h>

#include <cstdio>
#include <cstdlib>
#include <queue>
#include <unordered_set>
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
  int64_t natX = 0;
  int64_t natY = 0;
  std::unordered_set<int64_t> delivered;

  for (int64_t i = 0, endI = int64_t(computers.size()); i < endI; i++) {
    computers[i].run(i);
  }
  while (true) {
    // Process outputs
    bool anyOutputs = false;
    for (int64_t i = 0, endI = int64_t(computers.size()); i < endI; i++) {
      for (int64_t j = 0, endJ = int64_t(computers[i].out.size()); j < endJ; j += 3) {
        int64_t dest = computers[i].out[j];
        int64_t x = computers[i].out[j+1];
        int64_t y = computers[i].out[j+2];
        if (dest == 255) {
          natX = x;
          natY = y;
        }
        else {
          inputs[dest].push_back(x);
          inputs[dest].push_back(y);
        }
        anyOutputs = true;
      }
    }

    // Run with any new inputs
    bool anyInputs = false;
    for (int64_t i = 0, endI = int64_t(computers.size()); i < endI; i++) {
      if (inputs[i].empty()) {
        computers[i].run(-1);
      }
      else {
        computers[i].run(inputs[i]);
        anyInputs = true;
      }
    }

    // Is the network idle?
    if (anyOutputs || anyInputs) {
      continue;
    }
    inputs[0].push_back(natX);
    inputs[0].push_back(natY);
    if (!delivered.insert(natY).second) {
      printf("First y value to be delivered by the NAT twice in a row is %lld\n", natY);
      return EXIT_SUCCESS;
    }
  }

  return EXIT_FAILURE;
}
