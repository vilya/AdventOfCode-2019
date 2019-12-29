#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>

#ifndef INTCODE_MEM_SIZE
#define INTCODE_MEM_SIZE (128 * 1024)
#endif

namespace vh {
  
  static constexpr int64_t kModeDiv[] = { 10, 100, 1000, 10000 };

  struct IntcodeComputer {
    std::vector<int64_t> mem;
    std::vector<int64_t> out;
    int64_t ip = 0, rel = 0, size = 0;

    bool load(const char* filename) {
      FILE* f = fopen(filename, "r");
      if (f == nullptr)
        return false;
      mem.reserve(INTCODE_MEM_SIZE);
      int64_t tmp;
      while (fscanf(f, "%lld,", &tmp) == 1)
        mem.push_back(tmp);
      fclose(f);
      if (mem.size() < INTCODE_MEM_SIZE)
        mem.resize(INTCODE_MEM_SIZE, 0ll);
      size = static_cast<int64_t>(mem.size());
      out.reserve(16);
      return true;
    }

    int64_t peek(int64_t i) const {
      int64_t mode = (mem[ip] / kModeDiv[i]) % 10;
      int64_t addr = ((mode == 1) ? (ip + i) : mem[ip + i]) + ((mode == 2) ? rel : 0);
      return (addr >= size) ? 0 : mem[addr];
    }

    int64_t& poke(int64_t i) {
      int64_t mode = (mem[ip] / kModeDiv[i]) % 10;
//      int64_t addr = ((mode == 1) ? (ip + i) : mem[ip + i]) + ((mode == 2) ? rel : 0);
      int64_t addr = mem[ip + i] + ((mode == 2) ? rel : 0);
      if (addr >= size) {
        size = addr + INTCODE_MEM_SIZE;
        mem.resize(static_cast<size_t>(size), 0ll);
      }
      return mem[addr];
    }

    // return value: 0 = halted, 1 = need input
    int run(std::deque<int64_t>& input) {
      out.clear();
      while (1) {
        int64_t op = mem[ip] % 100;
        switch (op) {
          case 1: poke(3) = peek(1) + peek(2); ip += 4; break; // add
          case 2: poke(3) = peek(1) * peek(2); ip += 4; break; // mul
          case 3: if (input.empty()) return 1; poke(1) = input.front(); input.pop_front(); ip += 2; break; // read
          case 4: out.push_back(peek(1)); ip += 2; break; // write
          case 5: ip = (peek(1) != 0) ? peek(2) : (ip + 3); break; // jump if true
          case 6: ip = (peek(1) == 0) ? peek(2) : (ip + 3); break; // jump if false
          case 7: poke(3) = (peek(1) <  peek(2)) ? 1 : 0; ip += 4; break; // less than
          case 8: poke(3) = (peek(1) == peek(2)) ? 1 : 0; ip += 4; break; // equals
          case 9: rel += peek(1); ip += 2; break; // set relative base
          default: return 0;
        }
      }
    }

    int run(int64_t val) {
      std::deque<int64_t> input{ val };
      return run(input);
    }

    int run() {
      std::deque<int64_t> input;
      return run(input);
    }

    int run(const char* str) {
      std::deque<int64_t> input;
      for (int i = 0; str[i] != '\0'; i++) {
        input.push_back(str[i]);
      }
      return run(input);
    }
  };

} // namespace vh
