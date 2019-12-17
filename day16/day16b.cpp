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
  size_t offset = 0;
  for (int i = 0; i < 7; i++) {
    tmp = fgetc(f);
    if (tmp == EOF) {
      return false;
    }
    tmp -= '0';
    offset = offset * 10 + tmp;
  }
  fseek(f, 0, SEEK_SET);

  while ((tmp = fgetc(f)) != EOF) {
    out.push_back(tmp - '0');
  }
  fclose(f);

  const size_t n = out.size();
  const size_t repeats = kRepeat - offset / n;

  out.resize(n * repeats);
  for (size_t i = 1; i < repeats; i++) {
    std::memcpy(out.data() + i * n, out.data(), n * sizeof(int64_t));
  }
  out.erase(out.begin(), out.begin() + offset % n);

  return true;
}


void apply_pattern(const std::vector<int64_t>& in, std::vector<int64_t>& out)
{
  const size_t n = in.size();
  assert(out.size() == n);
  int64_t sum = 0;
  for (int64_t i = n - 1; i >= 0; --i) {
    sum += in[i];
    out[i] = sum % 10;
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
