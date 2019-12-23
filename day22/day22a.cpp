#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <vector>

constexpr int64_t kNumCards = 10007;
constexpr int64_t kStartPos = 2019;


static inline int64_t mod(int64_t a, int64_t b)
{
  return (a >= 0) ? (a % b) : (b - (-a) % b);
}


int main(int argc, char** argv)
{
  FILE* f = fopen(argv[1], "r");
  if (!f) {
    fprintf(stderr, "Failed to open %s\n", argv[1]);
    return 1;
  }

  int64_t pos = kStartPos;

//  char line[256];
//  while (fgets(line, sizeof(line), f) != nullptr) {
//    int arg;
//    if (sscanf(line, "cut %d", &arg) == 1) {
//      pos = (pos + kNumCards - arg) % kNumCards;
//    }
//    else if (sscanf(line, "deal with increment %d", &arg) == 1) {
//      pos = (pos * arg) % kNumCards;
//    }
//    else {
//      pos = kNumCards - 1 - pos;
//    }
//  }
//  fclose(f);

  int64_t A = 1;
  int64_t B = 0;
  char line[256];
  while (fgets(line, sizeof(line), f) != nullptr) {
    int64_t arg, a, b;
    if (sscanf(line, "cut %lld", &arg) == 1) {
      if (arg < 0) {
        arg += kNumCards;
      }
      a = 1;
      b = kNumCards - arg;
    }
    else if (sscanf(line, "deal with increment %lld", &arg) == 1) {
      a = arg;
      b = 0;
    }
    else {
      a = -1;
      b = kNumCards - 1;
    }
    A = mod(a * A, kNumCards);
    B = mod(a * B + b, kNumCards);
  }
  fclose(f);

  for (int64_t shuffles = 1; shuffles <= 10; shuffles++) {
    pos = mod(A * pos + B, kNumCards);
    // correct answer is 3749 after 1 shuffle
    printf("Card %lld is at index %4lld after %lld shuffles\n", kStartPos, pos, shuffles);
  }

  return 0;
}
