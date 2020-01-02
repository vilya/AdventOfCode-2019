#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>

#ifndef INTCODE_MEM_SIZE
#define INTCODE_MEM_SIZE (128 * 1024)
#endif

namespace vh {
  
  static constexpr uint8_t kModes_000[4] = { 0, 0, 0, 0 };
  static constexpr uint8_t kModes_001[4] = { 0, 1, 0, 0 };
  static constexpr uint8_t kModes_002[4] = { 0, 2, 0, 0 };
  static constexpr uint8_t kModes_010[4] = { 0, 0, 1, 0 };
  static constexpr uint8_t kModes_011[4] = { 0, 1, 1, 0 };
  static constexpr uint8_t kModes_012[4] = { 0, 2, 1, 0 };
  static constexpr uint8_t kModes_020[4] = { 0, 0, 2, 0 };
  static constexpr uint8_t kModes_021[4] = { 0, 1, 2, 0 };
  static constexpr uint8_t kModes_022[4] = { 0, 2, 2, 0 };
  static constexpr uint8_t kModes_200[4] = { 0, 0, 0, 2 };
  static constexpr uint8_t kModes_201[4] = { 0, 1, 0, 2 };
  static constexpr uint8_t kModes_202[4] = { 0, 2, 0, 2 };
  static constexpr uint8_t kModes_210[4] = { 0, 0, 1, 2 };
  static constexpr uint8_t kModes_211[4] = { 0, 1, 1, 2 };
  static constexpr uint8_t kModes_212[4] = { 0, 2, 1, 2 };
  static constexpr uint8_t kModes_220[4] = { 0, 0, 2, 2 };
  static constexpr uint8_t kModes_221[4] = { 0, 1, 2, 2 };
  static constexpr uint8_t kModes_222[4] = { 0, 2, 2, 2 };


  struct IntcodeComputer {
    std::vector<int64_t> mem;
    std::vector<int64_t> out;
    int64_t ip = 0, rel = 0, size = 0;
    const uint8_t* modes = nullptr;


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
      int64_t addr;
      switch (modes[i]) {
        case 0: addr = mem[ip + i]; break;
        case 1: addr = ip + i; break;
        case 2: addr = mem[ip + i] + rel; break;
      }
      return (addr >= size) ? 0 : mem[addr];
    }

    int64_t& poke(int64_t i) {
      int64_t addr = mem[ip + i] + (modes[i] >> 1) * rel;
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
        int op = static_cast<int>(mem[ip]);
        switch (op / 100) {
          case   0: modes = kModes_000; break;
          case   1: modes = kModes_001; break;
          case   2: modes = kModes_002; break;
          case  10: modes = kModes_010; break;
          case  11: modes = kModes_011; break;
          case  12: modes = kModes_012; break;
          case  20: modes = kModes_020; break;
          case  21: modes = kModes_021; break;
          case  22: modes = kModes_022; break;
          case 200: modes = kModes_200; break;
          case 201: modes = kModes_201; break;
          case 202: modes = kModes_202; break;
          case 210: modes = kModes_210; break;
          case 211: modes = kModes_211; break;
          case 212: modes = kModes_212; break;
          case 220: modes = kModes_220; break;
          case 221: modes = kModes_221; break;
          case 222: modes = kModes_222; break;
          default: modes = nullptr; break;
        }

        switch (op % 100) {
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
