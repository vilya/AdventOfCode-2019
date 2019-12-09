#include <cstdio>
#include <vector>

int main(int argc, char** argv)
{
  FILE* f = fopen(argv[1], "r");
  const size_t layerW = 25, layerH = 6, layerLen = layerW * layerH;
  std::vector<int> image(layerLen, 2);
  for (size_t i = 0; true; i = (i + 1) % layerLen) {
    int tmp = fgetc(f);
    if (tmp == EOF) {
      break;
    }
    else if (image[i] == 2) {
      image[i] = tmp - '0';
    }
  }
  for (size_t i = 0, endI = image.size(); i < endI; i++) {
    fputc((image[i] == 1) ? '*' : ' ', stdout);
    if ((i + 1) % layerW == 0) {
      fputc('\n', stdout);
    }
  }
}
