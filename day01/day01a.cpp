#include <cstdio>

int main(int argc, char** argv)
{
  FILE* f = fopen(argv[1], "r");
  if (f == nullptr) {
    return 1;
  }

  int total = 0;
  int weight;
  while (fscanf(f, "%d", &weight) == 1) {
    total += (weight / 3) - 2;
  }
  fclose(f);

  printf("Total fuel = %d\n", total);
  return 0;
}
