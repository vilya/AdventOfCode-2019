#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <vector>

int main(int argc, char** argv)
{
  FILE* f = fopen(argv[1], "r");
  if (f == nullptr) {
    return EXIT_FAILURE;
  }

  std::vector<int> pixels;
  int tmp;
  while (true) {
    tmp = fgetc(f);
    if (tmp == EOF) {
      break;
    }
    pixels.push_back(tmp - '0');
  }

  const size_t layerLen = 25 * 6;
  uint32_t minZeroes = std::numeric_limits<uint32_t>::max();
  uint32_t onesTimesTwos = 0;
  size_t bestLayer = 0;

  for (size_t layer = 0, endLayer = pixels.size() / layerLen; layer < endLayer; layer++) {
    uint32_t counts[3] = { 0, 0, 0 };
    for (size_t i = layer * layerLen, endI = (layer + 1) * layerLen; i < endI; i++) {
      tmp = pixels[i];
      if (tmp >= 0 && tmp < 3) {
        ++counts[tmp];
      }
    }

    printf("Layer %u, counts = { %u, %u, %u }\n", uint32_t(layer), counts[0], counts[1], counts[2]);

    if (counts[0] < minZeroes) {
      minZeroes = counts[0];
      onesTimesTwos = counts[1] * counts[2];
      bestLayer = layer;
    }
  }

  printf("Answer is %u, for layer %u\n", onesTimesTwos, uint32_t(bestLayer));

  return EXIT_SUCCESS;
}
