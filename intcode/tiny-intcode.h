#include <cstdio>
#include <vector>

namespace vh {
  
  static constexpr int64_t kModeDiv[] = { 10, 100, 1000, 10000 };

  struct IntcodeComputer {
    std::vector<int64_t> mem;
    std::vector<int64_t> out;
    int64_t ip = 0, rel = 0;

    bool load(const char* filename) {
      FILE* f = fopen(filename, "r");
      if (f == nullptr)
        return false;
      int64_t tmp;
      while (fscanf(f, "%lld,", &tmp) == 1)
        mem.push_back(tmp);
      fclose(f);
      if (mem.size() < 128 * 1024)
        mem.resize(128 * 1024, 0ll);
      out.reserve(16);
      return true;
    }

    int64_t& loc(int64_t i) {
      int64_t mode = (mem[ip] / kModeDiv[i]) % 10;
      return mem[((mode == 1) ? (ip + i) : mem[ip + i]) + ((mode == 2) ? rel : 0)]; 
    }

    // return value: 0 = halted, 1 = need input
    int run(int64_t input) {
      bool has_in = true;
      out.clear();
      while (1) {
        int64_t op = mem[ip];
        switch (op % 100) {
          case 1: loc(3) = loc(1) + loc(2); ip += 4; break; // add
          case 2: loc(3) = loc(1) * loc(2); ip += 4; break; // mul
          case 3: if (!has_in) return 1; loc(1) = input; has_in = false; ip += 2; break; // read
          case 4: out.push_back(loc(1)); ip += 2; break; // write
          case 5: ip = (loc(1) != 0) ? loc(2) : (ip + 3); break; // jump if true
          case 6: ip = (loc(1) == 0) ? loc(2) : (ip + 3); break; // jump if false
          case 7: loc(3) = (loc(1) <  loc(2)) ? 1 : 0; ip += 4; break; // less than
          case 8: loc(3) = (loc(1) == loc(2)) ? 1 : 0; ip += 4; break; // equals
          case 9: rel += loc(1); ip += 2; break; // set relative base
          default: return 0;
        }
      }
    }
  };

} // namespace vh
