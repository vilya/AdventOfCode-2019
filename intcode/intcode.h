#ifndef VH_INTCODE_H
#define VH_INTCODE_H

#include <cstdint>
#include <vector>
#include <queue>

namespace vh {

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
    size_t ip      = 0;
    size_t relBase = 0;

    bool load(const char* filename);

    void init();

    Status run(std::queue<int64_t>& input, std::vector<int64_t>& output);

  private:
    int64_t  fetch(size_t index, Mode mode) const;
    int64_t& put(size_t index, Mode mode);
  };

} // namespace vh

#endif // VH_INTCODE_H
