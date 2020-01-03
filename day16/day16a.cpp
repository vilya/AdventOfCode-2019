#include <cassert>
#include <cstdint>
#include <cstdio>
#include <vector>


bool load(const char* filename, std::vector<int64_t>& out)
{
  FILE* f = fopen(filename, "r");
  if (!f) {
    return false;
  }
  int64_t tmp;
  while ((tmp = fgetc(f)) != EOF) {
    out.push_back(tmp - '0');
  }
  fclose(f);
  return true;
}


void apply_pattern(const std::vector<int64_t>& in, std::vector<int64_t>& out)
{
  const size_t n = in.size();
  assert(out.size() == n);

  const size_t k = 4;
  const int64_t pattern[k] = { 0, 1, 0, -1 };

  for (size_t outI = 1; outI <= n; outI++) {
    int64_t total = 0;
    for (size_t inI = outI; inI <= n; inI++) {
      size_t p = (inI / outI) % k;
      total += in[inI-1] * pattern[p];
    }
    out[outI - 1] = std::abs(total) % 10;
  }
}


int main(int argc, char** argv)
{
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <input.txt>\n", argv[0]);
    return 1;
  }

  std::vector<int64_t> data[2];
  if (!load(argv[1], data[0])) {
    fprintf(stderr, "Failed to load %s\n", argv[1]);
    return 1;
  }
  data[1].resize(data[0].size(), 0);

  const size_t numPhases = 100;
  for (size_t i = 0; i < numPhases; i++) {
    size_t src = i & 1;
    size_t dst = src ^ 1;
    apply_pattern(data[src], data[dst]);
  }

  size_t finalDst = ((numPhases - 1) ^ 1) & 1;
  for (size_t i = 0; i < 8; i++) {
    fputc(static_cast<int>(data[finalDst][i] + '0'), stdout);
  }
  printf("\n");

  return 0;
}
