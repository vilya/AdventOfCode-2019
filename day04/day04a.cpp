#include <cstdio>
#include <cstdint>

void preprocess(uint8_t num[], int n)
{
  for (int i = 0; i < (n - 1); i++) {
    if (num[i] > num[i + 1]) {
      for (int j = i + 1; j < n; j++)
        num[j] = num[i];
      return;
    }
  }
}


void increment(uint8_t num[], int n)
{
  int i = n - 1;
  while (i >= 0 && num[i] == 9)
    --i;
  if (i < 0)
    return;
  ++num[i];
  for (int j = i + 1; j < n; j++) {
    num[j] = num[i];
  }
}


bool valid(const uint8_t num[], int n)
{
  for (int i = 1; i < n; i++) {
    if (num[i - 1] == num[i]) {
      return true;
    }
  }
  return false;
}


bool less(const uint8_t num[], const uint8_t end[], int n)
{
  for (int i = 0; i < n; i++) {
    if (num[i] != end[i]) {
      return num[i] < end[i];
    }
  }
  return false;
}


int main(int argc, char** argv)
{
  const int n = 6;

  uint8_t       num[n] = {1, 0, 8, 4, 5, 7};
  const uint8_t end[n] = {5, 6, 2, 0, 4, 1};

  uint64_t count = 0;
  preprocess(num, n);
  while (less(num, end, n)) {
    if (valid(num, n)) {
      ++count;
    }
    increment(num, n);
  }
  printf("Answer is %llu\n", count);

  return 0;
}
