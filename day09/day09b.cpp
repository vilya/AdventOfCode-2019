#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>

enum class Mode : int64_t {
  Address = 0,
  Immediate = 1,
  Relative = 2,
};


enum class Status {
  Halted,
  AwaitingInput,
};


struct IntcodeComputer {
  std::vector<int64_t> mem;
  size_t ip = 0;
  size_t relBase = 0;

  bool load(const char* filename) {
    mem.clear();
    FILE* f = fopen(filename, "r");
    if (f != nullptr) {
      int64_t tmp;
      while (fscanf(f, "%lld,", &tmp) == 1) {
        mem.push_back(tmp);
      }
      fclose(f);
      return true;
    }
    return false;
  }

  void init() {
    ip = 0;
    relBase = 0;
  }

  inline int64_t fetch(size_t index, Mode mode) const {
    switch (mode) {
    case Mode::Address:
      index = (index < mem.size()) ? mem[index] : 0;
      return (index < mem.size()) ? mem[index] : 0;
    case Mode::Immediate:
      return (index < mem.size()) ? mem[index] : 0;
    case Mode::Relative:
      index = (index < mem.size()) ? mem[index] : 0;
      index += relBase;
      return (index < mem.size()) ? mem[index] : 0;
    }
  }

  // Lookup logic is the same as fetch(index, Mode::Address), but this returns
  // a reference rather than a value; and if the index is larger than the
  // current mem size it will resize mem to fit.
  inline int64_t& put(size_t index, Mode mode) {
    switch (mode) {
    case Mode::Address:
      index = (index < mem.size()) ? mem[index] : 0;
      if (index >= mem.size()) {
        mem.resize(index + 1, 0ll);
      }
      return mem[index];
    case Mode::Immediate:
      throw std::runtime_error("Invalid use of immediate mode for put");
    case Mode::Relative:
      index = (index < mem.size()) ? mem[index] : 0;
      index += relBase;
      if (index >= mem.size()) {
        mem.resize(index + 1, 0ll);
      }
      return mem[index];
    }
  }

  Status run(std::queue<int64_t>& input, std::vector<int64_t>& output)
  {
    while (true) {
      int64_t opcode = std::abs(mem[ip]);
      int64_t instr = opcode % 100;
      Mode mode1 = static_cast<Mode>((opcode / 100) % 10);
      Mode mode2 = static_cast<Mode>((opcode / 1000) % 10);
      Mode mode3 = static_cast<Mode>((opcode / 10000) % 10);
      switch (instr) {
      case 1: // add
        put(ip + 3, mode3) = fetch(ip + 1, mode1) + fetch(ip + 2, mode2);
        ip += 4;
        break;
      case 2: // mul
        put(ip + 3, mode3) = fetch(ip + 1, mode1) * fetch(ip + 2, mode2);
        ip += 4;
        break;
      case 3: // read from input
        if (input.empty()) {
          return Status::AwaitingInput;
        }
        put(ip + 1, mode1) = input.front();
        input.pop();
        ip += 2;
        break;
      case 4: // write to output
        output.push_back(fetch(ip + 1, mode1));
        ip += 2;
        break;
      case 5: // jump if true
        ip = (fetch(ip + 1, mode1) != 0) ? size_t(fetch(ip + 2, mode2)) : (ip + 3);
        break;
      case 6: // jump if false
        ip = (fetch(ip + 1, mode1) == 0) ? size_t(fetch(ip + 2, mode2)) : (ip + 3);
        break;
      case 7: // less than
        put(ip + 3, mode3) = (fetch(ip + 1, mode1) < fetch(ip + 2, mode2)) ? 1 : 0;
        ip += 4;
        break;
      case 8: // equals
        put(ip + 3, mode3) = (fetch(ip + 1, mode1) == fetch(ip + 2, mode2)) ? 1 : 0;
        ip += 4;
        break;
      case 9: // set relative base
        relBase += fetch(ip + 1, mode1);
        ip += 2;
        break;
      case 99:
      default:
        return Status::Halted;
      }
    }
  }
};


int main(int argc, char** argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <prog.txt>\n", argv[0]);
    return EXIT_FAILURE;
  }

  IntcodeComputer src;
  if (!src.load(argv[1])) {
    fprintf(stderr, "Couldn't open %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  std::queue<int64_t> input;
  input.push(2);
  std::vector<int64_t> output;
  src.init();
  src.run(input, output);

  for (int64_t x : output) {
    printf("%lld,", x);
  }
  printf("\n");
  return EXIT_SUCCESS;
}
