#include <cstdio>
#include <cstdint>
#include <cstring> // for memcmp and memcpy

const int kNumMoons = 4;


inline static int sign_of(int x)
{
  return (x < 0) ? -1 : (x > 0) ? 1 : 0;
}


static int64_t gcd(int64_t x, int64_t y)
{
  return (y == 0) ? x : gcd(y, x % y);
}


inline static int64_t lcm(int64_t a, int64_t b)
{
  return (a == 0 || b == 0) ? 0 : (a * b / gcd(a, b));
}


static void step(int state[kNumMoons][2])
{
  // Apply gravity
  for (int m0 = 0; m0 < kNumMoons; m0++) {
    for (int m1 = 0; m1 < kNumMoons; m1++) {
      state[m0][1] += sign_of(state[m1][0] - state[m0][0]);
    }
  }
  // Apply velocity
  for (int m = 0; m < kNumMoons; m++) {
    state[m][0] += state[m][1];
  }
}


static int64_t cycle_length(const int initial[kNumMoons][2])
{
  int state[kNumMoons][2];
  std::memcpy(state, initial, sizeof(int) * kNumMoons * 2);
  int64_t i = 0;
  do {
    step(state);
    i++;
  } while (memcmp(state, initial, sizeof(int) * kNumMoons * 2) != 0);
  return i;
}


static int64_t solve(const int positions[kNumMoons][3])
{
  int moons[3][kNumMoons][2];
  for (int axis = 0; axis < 3; axis++) {
    for (int moon = 0; moon < kNumMoons; moon++) {
      moons[axis][moon][0] = positions[moon][axis];
      moons[axis][moon][1] = 0;
    }
  }
  int64_t cycleX = cycle_length(moons[0]);
  int64_t cycleY = cycle_length(moons[1]);
  int64_t cycleZ = cycle_length(moons[2]);
  return lcm(lcm(cycleX, cycleY), cycleZ);
}


const int example1Input[kNumMoons][3] = {
  {-1,  0,  2},
  { 2,-10, -7},
  { 4, -8,  8},
  { 3,  5, -1},
};


const int realInput[kNumMoons][3] = {
  { 17,   5,  1 },
  { -2,  -8,  8 },
  {  7,  -6, 14 },
  {  1, -10,  4 },
};


int main(int argc, char** argv)
{
//  int64_t numSteps = solve(example1Input);
  int64_t numSteps = solve(realInput);
  printf("Repeats after %lld steps\n", numSteps);
  return 0;
}
