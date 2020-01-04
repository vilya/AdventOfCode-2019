#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>

#ifdef _WIN32
  #define NOMINMAX
  #include <Windows.h>
  #include <memoryapi.h>
#else
  #include <sys/mman.h>
#endif

namespace vh {
  
  static constexpr int64_t kMemSize = 256 * 1024 * 1024;
  static constexpr int64_t kDispatchTableSize = 22300;


#ifdef _WIN32
  void* alloc_virtual_mem(int64_t nbytes) {
    return VirtualAlloc(NULL, sizeof(int64_t) * nbytes, MEM_COMMIT, PAGE_READWRITE);
  }

  void free_virtual_mem(void* mem, int64_t /*nbytes*/) {
    VirtualFree(mem, 0, MEM_RELEASE);
  }
#else
  void* alloc_virtual_mem(int64_t nbytes) {
    return mmap(nullptr, sizeof(int64_t) * nbytes, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_NORESERVE, -1, 0);
  }

  void free_virtual_mem(void* mem, int64_t nbytes) {
    munmap(mem, sizeof(int64_t) * nbytes);
  }
#endif


  struct IntcodeComputer {
    int64_t* mem;
    std::vector<int64_t> out;
    int64_t ip = 0, rel = 0, size = 0;

    IntcodeComputer() {
      mem = reinterpret_cast<int64_t*>(alloc_virtual_mem(sizeof(int64_t) * kMemSize));
      out.reserve(16);
      // Run a trivial program so that the dispatch table gets initialised.
      mem[0] = 99;
      run();
    }

    IntcodeComputer(const IntcodeComputer& other) : mem(nullptr), out(other.out), ip(other.ip), rel(other.rel), size(other.size) {
      mem = reinterpret_cast<int64_t*>(alloc_virtual_mem(sizeof(int64_t) * kMemSize));
      std::memcpy(mem, other.mem, sizeof(int64_t) * size);
    }

    IntcodeComputer(IntcodeComputer&& other) : mem(other.mem), out(std::move(other.out)), ip(other.ip), rel(other.rel), size(other.size) {
      other.mem = nullptr;
    }

    ~IntcodeComputer() {
      if (mem)
        free_virtual_mem(mem, sizeof(int64_t) * kMemSize);
    }

    IntcodeComputer& operator = (const IntcodeComputer& other) {
      mem = reinterpret_cast<int64_t*>(alloc_virtual_mem(sizeof(int64_t) * kMemSize));
      out = other.out;
      ip = other.ip;
      rel = other.rel;
      size = other.size;
      return *this;
    }

    IntcodeComputer& operator = (IntcodeComputer&& other) {
      mem = other.mem;
      out = std::move(other.out);
      ip = other.ip;
      rel = other.rel;
      size = other.size;
      other.mem = nullptr;
      return *this;
    }

    bool load(const char* filename) {
      FILE* f = fopen(filename, "r");
      if (f == nullptr)
        return false;
      int64_t tmp;
      while (fscanf(f, "%lld,", &tmp) == 1)
        mem[size++] = tmp;
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
      static void** dispatch_table = nullptr;
      if (dispatch_table == nullptr) {
        dispatch_table = new void*[kDispatchTableSize];
        std::fill(dispatch_table, dispatch_table + kDispatchTableSize, &&case_default);
        // add labels
        dispatch_table[    1] = &&case_00001;
        dispatch_table[  101] = &&case_00101;
        dispatch_table[  201] = &&case_00201;
        dispatch_table[ 1001] = &&case_01001;
        dispatch_table[ 1101] = &&case_01101;
        dispatch_table[ 1201] = &&case_01201;
        dispatch_table[ 2001] = &&case_02001;
        dispatch_table[ 2101] = &&case_02101;
        dispatch_table[ 2201] = &&case_02201;
        dispatch_table[20001] = &&case_20001;
        dispatch_table[20101] = &&case_20101;
        dispatch_table[20201] = &&case_20201;
        dispatch_table[21001] = &&case_21001;
        dispatch_table[21101] = &&case_21101;
        dispatch_table[21201] = &&case_21201;
        dispatch_table[22001] = &&case_22001;
        dispatch_table[22101] = &&case_22101;
        dispatch_table[22201] = &&case_22201;
        // mul labels
        dispatch_table[    2] = &&case_00002;
        dispatch_table[  102] = &&case_00102;
        dispatch_table[  202] = &&case_00202;
        dispatch_table[ 1002] = &&case_01002;
        dispatch_table[ 1102] = &&case_01102;
        dispatch_table[ 1202] = &&case_01202;
        dispatch_table[ 2002] = &&case_02002;
        dispatch_table[ 2102] = &&case_02102;
        dispatch_table[ 2202] = &&case_02202;
        dispatch_table[20002] = &&case_20002;
        dispatch_table[20102] = &&case_20102;
        dispatch_table[20202] = &&case_20202;
        dispatch_table[21002] = &&case_21002;
        dispatch_table[21102] = &&case_21102;
        dispatch_table[21202] = &&case_21202;
        dispatch_table[22002] = &&case_22002;
        dispatch_table[22102] = &&case_22102;
        dispatch_table[22202] = &&case_22202;
        // read labels
        dispatch_table[  3] = &&case_003;
        dispatch_table[203] = &&case_203;
        // write labels
        dispatch_table[  4] = &&case_004;
        dispatch_table[104] = &&case_104;
        dispatch_table[204] = &&case_204;
        // jump-if-true labels
        dispatch_table[   5] = &&case_0005;
        dispatch_table[ 105] = &&case_0105;
        dispatch_table[ 205] = &&case_0205;
        dispatch_table[1005] = &&case_1005;
        dispatch_table[1105] = &&case_1105;
        dispatch_table[1205] = &&case_1205;
        dispatch_table[2005] = &&case_2005;
        dispatch_table[2105] = &&case_2105;
        dispatch_table[2205] = &&case_2205;
        // jump-if-false labels
        dispatch_table[   6] = &&case_0006;
        dispatch_table[ 106] = &&case_0106;
        dispatch_table[ 206] = &&case_0206;
        dispatch_table[1006] = &&case_1006;
        dispatch_table[1106] = &&case_1106;
        dispatch_table[1206] = &&case_1206;
        dispatch_table[2006] = &&case_2006;
        dispatch_table[2106] = &&case_2106;
        dispatch_table[2206] = &&case_2206;
        // less-than labels
        dispatch_table[    7] = &&case_00007;
        dispatch_table[  107] = &&case_00107;
        dispatch_table[  207] = &&case_00207;
        dispatch_table[ 1007] = &&case_01007;
        dispatch_table[ 1107] = &&case_01107;
        dispatch_table[ 1207] = &&case_01207;
        dispatch_table[ 2007] = &&case_02007;
        dispatch_table[ 2107] = &&case_02107;
        dispatch_table[ 2207] = &&case_02207;
        dispatch_table[20007] = &&case_20007;
        dispatch_table[20107] = &&case_20107;
        dispatch_table[20207] = &&case_20207;
        dispatch_table[21007] = &&case_21007;
        dispatch_table[21107] = &&case_21107;
        dispatch_table[21207] = &&case_21207;
        dispatch_table[22007] = &&case_22007;
        dispatch_table[22107] = &&case_22107;
        dispatch_table[22207] = &&case_22207;
        // equals labels
        dispatch_table[    8] = &&case_00008;
        dispatch_table[  108] = &&case_00108;
        dispatch_table[  208] = &&case_00208;
        dispatch_table[ 1008] = &&case_01008;
        dispatch_table[ 1108] = &&case_01108;
        dispatch_table[ 1208] = &&case_01208;
        dispatch_table[ 2008] = &&case_02008;
        dispatch_table[ 2108] = &&case_02108;
        dispatch_table[ 2208] = &&case_02208;
        dispatch_table[20008] = &&case_20008;
        dispatch_table[20108] = &&case_20108;
        dispatch_table[20208] = &&case_20208;
        dispatch_table[21008] = &&case_21008;
        dispatch_table[21108] = &&case_21108;
        dispatch_table[21208] = &&case_21208;
        dispatch_table[22008] = &&case_22008;
        dispatch_table[22108] = &&case_22108;
        dispatch_table[22208] = &&case_22208;

        // variants of set-relative-base
        dispatch_table[  9] = &&case_009;
        dispatch_table[109] = &&case_109;
        dispatch_table[209] = &&case_209;
      }

      out.clear();
      goto *dispatch_table[mem[ip]];
      while (1) {
        // variants of add
        case_00001: poke0(3, peek0(1) + peek0(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_00101: poke0(3, peek1(1) + peek0(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_00201: poke0(3, peek2(1) + peek0(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_01001: poke0(3, peek0(1) + peek1(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_01101: poke0(3, peek1(1) + peek1(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_01201: poke0(3, peek2(1) + peek1(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_02001: poke0(3, peek0(1) + peek2(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_02101: poke0(3, peek1(1) + peek2(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_02201: poke0(3, peek2(1) + peek2(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_20001: poke2(3, peek0(1) + peek0(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_20101: poke2(3, peek1(1) + peek0(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_20201: poke2(3, peek2(1) + peek0(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_21001: poke2(3, peek0(1) + peek1(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_21101: poke2(3, peek1(1) + peek1(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_21201: poke2(3, peek2(1) + peek1(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_22001: poke2(3, peek0(1) + peek2(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_22101: poke2(3, peek1(1) + peek2(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_22201: poke2(3, peek2(1) + peek2(2)); ip += 4; goto *dispatch_table[mem[ip]];

        // variants of mul
        case_00002: poke0(3, peek0(1) * peek0(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_00102: poke0(3, peek1(1) * peek0(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_00202: poke0(3, peek2(1) * peek0(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_01002: poke0(3, peek0(1) * peek1(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_01102: poke0(3, peek1(1) * peek1(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_01202: poke0(3, peek2(1) * peek1(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_02002: poke0(3, peek0(1) * peek2(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_02102: poke0(3, peek1(1) * peek2(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_02202: poke0(3, peek2(1) * peek2(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_20002: poke2(3, peek0(1) * peek0(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_20102: poke2(3, peek1(1) * peek0(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_20202: poke2(3, peek2(1) * peek0(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_21002: poke2(3, peek0(1) * peek1(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_21102: poke2(3, peek1(1) * peek1(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_21202: poke2(3, peek2(1) * peek1(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_22002: poke2(3, peek0(1) * peek2(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_22102: poke2(3, peek1(1) * peek2(2)); ip += 4; goto *dispatch_table[mem[ip]];
        case_22202: poke2(3, peek2(1) * peek2(2)); ip += 4; goto *dispatch_table[mem[ip]];

        // variants of read
        case_003: if (input.empty()) return 1; poke0(1, input.front()); input.pop_front(); ip += 2; goto *dispatch_table[mem[ip]];
        case_203: if (input.empty()) return 1; poke2(1, input.front()); input.pop_front(); ip += 2; goto *dispatch_table[mem[ip]];

        // variants of write
        case_004: out.push_back(peek0(1)); ip += 2; goto *dispatch_table[mem[ip]];
        case_104: out.push_back(peek1(1)); ip += 2; goto *dispatch_table[mem[ip]];
        case_204: out.push_back(peek2(1)); ip += 2; goto *dispatch_table[mem[ip]];

        // variants of jump-if-true
        case_0005: ip = (peek0(1) != 0) ? peek0(2) : (ip + 3); goto *dispatch_table[mem[ip]];
        case_0105: ip = (peek1(1) != 0) ? peek0(2) : (ip + 3); goto *dispatch_table[mem[ip]];
        case_0205: ip = (peek2(1) != 0) ? peek0(2) : (ip + 3); goto *dispatch_table[mem[ip]];
        case_1005: ip = (peek0(1) != 0) ? peek1(2) : (ip + 3); goto *dispatch_table[mem[ip]];
        case_1105: ip = (peek1(1) != 0) ? peek1(2) : (ip + 3); goto *dispatch_table[mem[ip]];
        case_1205: ip = (peek2(1) != 0) ? peek1(2) : (ip + 3); goto *dispatch_table[mem[ip]];
        case_2005: ip = (peek0(1) != 0) ? peek2(2) : (ip + 3); goto *dispatch_table[mem[ip]];
        case_2105: ip = (peek1(1) != 0) ? peek2(2) : (ip + 3); goto *dispatch_table[mem[ip]];
        case_2205: ip = (peek2(1) != 0) ? peek2(2) : (ip + 3); goto *dispatch_table[mem[ip]];

        // variants of jump-if-false
        case_0006: ip = (peek0(1) == 0) ? peek0(2) : (ip + 3); goto *dispatch_table[mem[ip]];
        case_0106: ip = (peek1(1) == 0) ? peek0(2) : (ip + 3); goto *dispatch_table[mem[ip]];
        case_0206: ip = (peek2(1) == 0) ? peek0(2) : (ip + 3); goto *dispatch_table[mem[ip]];
        case_1006: ip = (peek0(1) == 0) ? peek1(2) : (ip + 3); goto *dispatch_table[mem[ip]];
        case_1106: ip = (peek1(1) == 0) ? peek1(2) : (ip + 3); goto *dispatch_table[mem[ip]];
        case_1206: ip = (peek2(1) == 0) ? peek1(2) : (ip + 3); goto *dispatch_table[mem[ip]];
        case_2006: ip = (peek0(1) == 0) ? peek2(2) : (ip + 3); goto *dispatch_table[mem[ip]];
        case_2106: ip = (peek1(1) == 0) ? peek2(2) : (ip + 3); goto *dispatch_table[mem[ip]];
        case_2206: ip = (peek2(1) == 0) ? peek2(2) : (ip + 3); goto *dispatch_table[mem[ip]];

        // variants of less-than
        case_00007: poke0(3, (peek0(1) < peek0(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_00107: poke0(3, (peek1(1) < peek0(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_00207: poke0(3, (peek2(1) < peek0(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_01007: poke0(3, (peek0(1) < peek1(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_01107: poke0(3, (peek1(1) < peek1(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_01207: poke0(3, (peek2(1) < peek1(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_02007: poke0(3, (peek0(1) < peek2(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_02107: poke0(3, (peek1(1) < peek2(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_02207: poke0(3, (peek2(1) < peek2(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_20007: poke2(3, (peek0(1) < peek0(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_20107: poke2(3, (peek1(1) < peek0(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_20207: poke2(3, (peek2(1) < peek0(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_21007: poke2(3, (peek0(1) < peek1(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_21107: poke2(3, (peek1(1) < peek1(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_21207: poke2(3, (peek2(1) < peek1(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_22007: poke2(3, (peek0(1) < peek2(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_22107: poke2(3, (peek1(1) < peek2(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_22207: poke2(3, (peek2(1) < peek2(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];

        // variants of equals
        case_00008: poke0(3, (peek0(1) == peek0(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_00108: poke0(3, (peek1(1) == peek0(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_00208: poke0(3, (peek2(1) == peek0(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_01008: poke0(3, (peek0(1) == peek1(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_01108: poke0(3, (peek1(1) == peek1(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_01208: poke0(3, (peek2(1) == peek1(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_02008: poke0(3, (peek0(1) == peek2(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_02108: poke0(3, (peek1(1) == peek2(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_02208: poke0(3, (peek2(1) == peek2(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_20008: poke2(3, (peek0(1) == peek0(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_20108: poke2(3, (peek1(1) == peek0(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_20208: poke2(3, (peek2(1) == peek0(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_21008: poke2(3, (peek0(1) == peek1(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_21108: poke2(3, (peek1(1) == peek1(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_21208: poke2(3, (peek2(1) == peek1(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_22008: poke2(3, (peek0(1) == peek2(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_22108: poke2(3, (peek1(1) == peek2(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];
        case_22208: poke2(3, (peek2(1) == peek2(2)) ? 1 : 0); ip += 4; goto *dispatch_table[mem[ip]];

        // variants of set-relative-base
        case_009: rel += peek0(1); ip += 2; goto *dispatch_table[mem[ip]];
        case_109: rel += peek1(1); ip += 2; goto *dispatch_table[mem[ip]];
        case_209: rel += peek2(1); ip += 2; goto *dispatch_table[mem[ip]];

        case_default: return 0;
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
