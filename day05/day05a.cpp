#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>


bool load(const char* filename, std::vector<int>& mem)
{
  FILE* f = fopen(filename, "r");
  if (f == nullptr) {
    return false;
  }

  int tmp;
  while (fscanf(f, "%d,", &tmp) == 1) {
    mem.push_back(tmp);
    if (mem.size() > 1000) {
      break;
    }
  }
  return true;
}


void run(std::vector<int>& mem, std::queue<int>& input, std::vector<int>& output)
{
  const int kMode_Address   = 0;
  const int kMode_Immediate = 1;

  size_t ip = 0;

  while (true) {
    int opcode = std::abs(mem[ip]);

    int instr = opcode % 100;
    int mode0 = (opcode / 100) % 10;
    int mode1 = (opcode / 1000) % 10;
//    int mode2 = (opcode / 10000) % 10;

    int arg0, arg1, arg2;

    switch (instr) {
    case 1: // add
      arg0 = (mode0 == kMode_Immediate) ? mem[ip + 1] : mem[mem[ip + 1]];
      arg1 = (mode1 == kMode_Immediate) ? mem[ip + 2] : mem[mem[ip + 2]];
      arg2 = mem[ip + 3];
      mem[arg2] = arg0 + arg1;
      ip += 4;
      break;

    case 2: // mul
      arg0 = (mode0 == kMode_Immediate) ? mem[ip + 1] : mem[mem[ip + 1]];
      arg1 = (mode1 == kMode_Immediate) ? mem[ip + 2] : mem[mem[ip + 2]];
      arg2 = mem[ip + 3];
      mem[arg2] = arg0 * arg1;
      ip += 4;
      break;

    case 3: // read from input
      arg0 = mem[ip + 1];
      mem[arg0] = input.front();
      input.pop();
      ip += 2;
      break;

    case 4: // write to output
      arg0 = (mode0 == kMode_Immediate) ? mem[ip + 1] : mem[mem[ip + 1]];
      output.push_back(arg0);
      ip += 2;
      break;

    case 99:
    default:
      return;
    }
  }
}


int main(int argc, char** argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <prog.txt>\n", argv[0]);
    return EXIT_FAILURE;
  }

  std::vector<int> mem;
  if (!load(argv[1], mem)) {
    fprintf(stderr, "Couldn't open %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  std::queue<int> input;
  input.push(1);

  std::vector<int> output;
  run(mem, input, output);

  bool passed = !output.empty();
  int diagnostic = output.back();
  output.pop_back();
  printf("test results:");
  for (int x : output) {
    if (x != 0) {
      passed = false;
    }
    printf(" %d", x);
  }
  printf("\n");

  printf("tests %s\n", passed ? "passed" : "failed");
  printf("diagnostic code = %d\n", diagnostic);

  return passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
