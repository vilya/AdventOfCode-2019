#include <cassert>
#include <cstdint>
#include <cstdio>
#include <vector>

constexpr size_t kRepeat = 10000;


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

  const size_t originalSize = out.size();
  out.resize(out.size() * kRepeat);
  for (size_t i = 1; i < kRepeat; i++) {
    std::memcpy(out.data() + i * originalSize, out.data(), originalSize * sizeof(int64_t));
  }

  return true;
}


void apply_pattern(const std::vector<int64_t>& in, std::vector<int64_t>& out, size_t offset)
{
  const size_t n = in.size();
  assert(out.size() == n);
  assert(offset * 2 > n);

  int64_t sum = 0;
  for (size_t i = offset - 1; i < n; i++) {
    sum += in[i];
  }
  for (size_t i = offset - 1; i < n; i++) {
    out[i] = sum % 10;
    sum -= in[i];
  }
}


int main(int argc, char** argv)
{
  std::vector<int64_t> data[2];
  if (!load(argv[1], data[0])) {
    fprintf(stderr, "Failed to load %s\n", argv[1]);
    return 1;
  }
  data[1].resize(data[0].size(), 0);

  size_t offset = 0;
  for (size_t i = 0; i < 7; i++) {
    offset = offset * 10 + data[0][i];
  }
  if (offset * 2 < data[0].size()) {
    fprintf(stderr, "Uh oh, can't solve this one: offset is in the first half of the data.\n");
    return 1;
  }

  const size_t numPhases = 100;
  for (size_t i = 0; i < numPhases; i++) {
    size_t src = i & 1;
    size_t dst = src ^ 1;
    apply_pattern(data[src], data[dst], offset);
  }

  size_t finalDst = ((numPhases - 1) ^ 1) & 1;
  for (size_t i = 0; i < 8; i++) {
    fputc(data[finalDst][i + offset] + '0', stdout);
  }
  printf("\n");

  return 0;
}
