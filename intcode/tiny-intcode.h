#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>

#include <sys/mman.h>

namespace vh {
  
  static constexpr int64_t kMemSize = 256 * 1024 * 1024;

  struct IntcodeComputer {
    int64_t* mem;
    std::vector<int64_t> out;
    int64_t ip = 0, rel = 0;
    const uint8_t* modes = nullptr;

    IntcodeComputer() {
      mem = reinterpret_cast<int64_t*>(mmap(nullptr, kMemSize * sizeof(int64_t), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_NORESERVE, -1, 0));
      out.reserve(16);
    }

    ~IntcodeComputer() {
      munmap(mem, kMemSize * sizeof(int64_t));
    }

    bool load(const char* filename) {
      FILE* f = fopen(filename, "r");
      if (f == nullptr)
        return false;
      int64_t tmp, i = 0;
      while (fscanf(f, "%lld,", &tmp) == 1)
        mem[i++] = tmp;
      fclose(f);
      return true;
    }

    int64_t peek0(int64_t i) const {
      int64_t addr = mem[ip + i];
      return mem[addr];
    }

    int64_t peek1(int64_t i) const {
      int64_t addr = ip + i;
      return mem[addr];
    }

    int64_t peek2(int64_t i) const {
      int64_t addr = mem[ip + i] + rel;
      return mem[addr];
    }

    void poke0(int64_t i, int64_t val) {
      int64_t addr = mem[ip + i];
      mem[addr] = val;
    }

    void poke2(int64_t i, int64_t val) {
      int64_t addr = mem[ip + i] + rel;
      mem[addr] = val;
    }

    // return value: 0 = halted, 1 = need input
    int run(std::deque<int64_t>& input) {
      out.clear();
      while (1) {
        int op = static_cast<int>(mem[ip]);
        switch (op) {
          // variants of add
          case     1: poke0(3, peek0(1) + peek0(2)); ip += 4; break;
          case   101: poke0(3, peek1(1) + peek0(2)); ip += 4; break;
          case   201: poke0(3, peek2(1) + peek0(2)); ip += 4; break;
          case  1001: poke0(3, peek0(1) + peek1(2)); ip += 4; break;
          case  1101: poke0(3, peek1(1) + peek1(2)); ip += 4; break;
          case  1201: poke0(3, peek2(1) + peek1(2)); ip += 4; break;
          case  2001: poke0(3, peek0(1) + peek2(2)); ip += 4; break;
          case  2101: poke0(3, peek1(1) + peek2(2)); ip += 4; break;
          case  2201: poke0(3, peek2(1) + peek2(2)); ip += 4; break;
          case 20001: poke2(3, peek0(1) + peek0(2)); ip += 4; break;
          case 20101: poke2(3, peek1(1) + peek0(2)); ip += 4; break;
          case 20201: poke2(3, peek2(1) + peek0(2)); ip += 4; break;
          case 21001: poke2(3, peek0(1) + peek1(2)); ip += 4; break;
          case 21101: poke2(3, peek1(1) + peek1(2)); ip += 4; break;
          case 21201: poke2(3, peek2(1) + peek1(2)); ip += 4; break;
          case 22001: poke2(3, peek0(1) + peek2(2)); ip += 4; break;
          case 22101: poke2(3, peek1(1) + peek2(2)); ip += 4; break;
          case 22201: poke2(3, peek2(1) + peek2(2)); ip += 4; break;

          // variants of mul
          case     2: poke0(3, peek0(1) * peek0(2)); ip += 4; break;
          case   102: poke0(3, peek1(1) * peek0(2)); ip += 4; break;
          case   202: poke0(3, peek2(1) * peek0(2)); ip += 4; break;
          case  1002: poke0(3, peek0(1) * peek1(2)); ip += 4; break;
          case  1102: poke0(3, peek1(1) * peek1(2)); ip += 4; break;
          case  1202: poke0(3, peek2(1) * peek1(2)); ip += 4; break;
          case  2002: poke0(3, peek0(1) * peek2(2)); ip += 4; break;
          case  2102: poke0(3, peek1(1) * peek2(2)); ip += 4; break;
          case  2202: poke0(3, peek2(1) * peek2(2)); ip += 4; break;
          case 20002: poke2(3, peek0(1) * peek0(2)); ip += 4; break;
          case 20102: poke2(3, peek1(1) * peek0(2)); ip += 4; break;
          case 20202: poke2(3, peek2(1) * peek0(2)); ip += 4; break;
          case 21002: poke2(3, peek0(1) * peek1(2)); ip += 4; break;
          case 21102: poke2(3, peek1(1) * peek1(2)); ip += 4; break;
          case 21202: poke2(3, peek2(1) * peek1(2)); ip += 4; break;
          case 22002: poke2(3, peek0(1) * peek2(2)); ip += 4; break;
          case 22102: poke2(3, peek1(1) * peek2(2)); ip += 4; break;
          case 22202: poke2(3, peek2(1) * peek2(2)); ip += 4; break;

          // variants of read
          case   3: if (input.empty()) return 1; poke0(1, input.front()); input.pop_front(); ip += 2; break;
          case 203: if (input.empty()) return 1; poke2(1, input.front()); input.pop_front(); ip += 2; break;

          // variants of write
          case   4: out.push_back(peek0(1)); ip += 2; break;
          case 104: out.push_back(peek1(1)); ip += 2; break;
          case 204: out.push_back(peek2(1)); ip += 2; break;

          // variants of jump-if-true
          case    5: ip = (peek0(1) != 0) ? peek0(2) : (ip + 3); break;
          case  105: ip = (peek1(1) != 0) ? peek0(2) : (ip + 3); break;
          case  205: ip = (peek2(1) != 0) ? peek0(2) : (ip + 3); break;
          case 1005: ip = (peek0(1) != 0) ? peek1(2) : (ip + 3); break;
          case 1105: ip = (peek1(1) != 0) ? peek1(2) : (ip + 3); break;
          case 1205: ip = (peek2(1) != 0) ? peek1(2) : (ip + 3); break;
          case 2005: ip = (peek0(1) != 0) ? peek2(2) : (ip + 3); break;
          case 2105: ip = (peek1(1) != 0) ? peek2(2) : (ip + 3); break;
          case 2205: ip = (peek2(1) != 0) ? peek2(2) : (ip + 3); break;

          // variants of jump-if-false
          case    6: ip = (peek0(1) == 0) ? peek0(2) : (ip + 3); break;
          case  106: ip = (peek1(1) == 0) ? peek0(2) : (ip + 3); break;
          case  206: ip = (peek2(1) == 0) ? peek0(2) : (ip + 3); break;
          case 1006: ip = (peek0(1) == 0) ? peek1(2) : (ip + 3); break;
          case 1106: ip = (peek1(1) == 0) ? peek1(2) : (ip + 3); break;
          case 1206: ip = (peek2(1) == 0) ? peek1(2) : (ip + 3); break;
          case 2006: ip = (peek0(1) == 0) ? peek2(2) : (ip + 3); break;
          case 2106: ip = (peek1(1) == 0) ? peek2(2) : (ip + 3); break;
          case 2206: ip = (peek2(1) == 0) ? peek2(2) : (ip + 3); break;

          // variants of less-than
          case     7: poke0(3, (peek0(1) < peek0(2)) ? 1 : 0); ip += 4; break;
          case   107: poke0(3, (peek1(1) < peek0(2)) ? 1 : 0); ip += 4; break;
          case   207: poke0(3, (peek2(1) < peek0(2)) ? 1 : 0); ip += 4; break;
          case  1007: poke0(3, (peek0(1) < peek1(2)) ? 1 : 0); ip += 4; break;
          case  1107: poke0(3, (peek1(1) < peek1(2)) ? 1 : 0); ip += 4; break;
          case  1207: poke0(3, (peek2(1) < peek1(2)) ? 1 : 0); ip += 4; break;
          case  2007: poke0(3, (peek0(1) < peek2(2)) ? 1 : 0); ip += 4; break;
          case  2107: poke0(3, (peek1(1) < peek2(2)) ? 1 : 0); ip += 4; break;
          case  2207: poke0(3, (peek2(1) < peek2(2)) ? 1 : 0); ip += 4; break;
          case 20007: poke2(3, (peek0(1) < peek0(2)) ? 1 : 0); ip += 4; break;
          case 20107: poke2(3, (peek1(1) < peek0(2)) ? 1 : 0); ip += 4; break;
          case 20207: poke2(3, (peek2(1) < peek0(2)) ? 1 : 0); ip += 4; break;
          case 21007: poke2(3, (peek0(1) < peek1(2)) ? 1 : 0); ip += 4; break;
          case 21107: poke2(3, (peek1(1) < peek1(2)) ? 1 : 0); ip += 4; break;
          case 21207: poke2(3, (peek2(1) < peek1(2)) ? 1 : 0); ip += 4; break;
          case 22007: poke2(3, (peek0(1) < peek2(2)) ? 1 : 0); ip += 4; break;
          case 22107: poke2(3, (peek1(1) < peek2(2)) ? 1 : 0); ip += 4; break;
          case 22207: poke2(3, (peek2(1) < peek2(2)) ? 1 : 0); ip += 4; break;

          // variants of equals
          case     8: poke0(3, (peek0(1) == peek0(2)) ? 1 : 0); ip += 4; break;
          case   108: poke0(3, (peek1(1) == peek0(2)) ? 1 : 0); ip += 4; break;
          case   208: poke0(3, (peek2(1) == peek0(2)) ? 1 : 0); ip += 4; break;
          case  1008: poke0(3, (peek0(1) == peek1(2)) ? 1 : 0); ip += 4; break;
          case  1108: poke0(3, (peek1(1) == peek1(2)) ? 1 : 0); ip += 4; break;
          case  1208: poke0(3, (peek2(1) == peek1(2)) ? 1 : 0); ip += 4; break;
          case  2008: poke0(3, (peek0(1) == peek2(2)) ? 1 : 0); ip += 4; break;
          case  2108: poke0(3, (peek1(1) == peek2(2)) ? 1 : 0); ip += 4; break;
          case  2208: poke0(3, (peek2(1) == peek2(2)) ? 1 : 0); ip += 4; break;
          case 20008: poke2(3, (peek0(1) == peek0(2)) ? 1 : 0); ip += 4; break;
          case 20108: poke2(3, (peek1(1) == peek0(2)) ? 1 : 0); ip += 4; break;
          case 20208: poke2(3, (peek2(1) == peek0(2)) ? 1 : 0); ip += 4; break;
          case 21008: poke2(3, (peek0(1) == peek1(2)) ? 1 : 0); ip += 4; break;
          case 21108: poke2(3, (peek1(1) == peek1(2)) ? 1 : 0); ip += 4; break;
          case 21208: poke2(3, (peek2(1) == peek1(2)) ? 1 : 0); ip += 4; break;
          case 22008: poke2(3, (peek0(1) == peek2(2)) ? 1 : 0); ip += 4; break;
          case 22108: poke2(3, (peek1(1) == peek2(2)) ? 1 : 0); ip += 4; break;
          case 22208: poke2(3, (peek2(1) == peek2(2)) ? 1 : 0); ip += 4; break;

          // variants of set-relative-base
          case   9: rel += peek0(1); ip += 2; break;
          case 109: rel += peek1(1); ip += 2; break;
          case 209: rel += peek2(1); ip += 2; break;

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
