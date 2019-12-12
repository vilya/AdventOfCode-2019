#include <algorithm> // for std::abs
#include <cstdio>
#include <cstdint>

const int kNumMoons = 4;


inline static int sign_of(int x)
{
  return (x < 0) ? -1 : (x > 0) ? 1 : 0;
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


static int64_t simulate(const int positions[kNumMoons][3], int numSteps)
{
  int moons[3][kNumMoons][2];
  for (int axis = 0; axis < 3; axis++) {
    for (int moon = 0; moon < kNumMoons; moon++) {
      moons[axis][moon][0] = positions[moon][axis];
      moons[axis][moon][1] = 0;
    }
  }

  for (int i = 0; i < numSteps; i++) {
    step(moons[0]);
    step(moons[1]);
    step(moons[2]);
  }

  int64_t energy = 0;
  for (int m = 0; m < kNumMoons; m++) {
    int64_t potential = std::abs(moons[0][m][0]) + std::abs(moons[1][m][0]) + std::abs(moons[2][m][0]);
    int64_t kinetic   = std::abs(moons[0][m][1]) + std::abs(moons[1][m][1]) + std::abs(moons[2][m][1]);
    energy += potential * kinetic;
  }
  return energy;
}


const int example1Input[kNumMoons][3] = {
  {-1,  0,  2},
  { 2,-10, -7},
  { 4, -8,  8},
  { 3,  5, -1},
};

const int example2Input[kNumMoons][3] = {
  { -8, -10,  0 },
  {  5,   5, 10 },
  {  2,  -7,  3 },
  {  9,  -8, -3 },
};

const int realInput[kNumMoons][3] = {
  { 17,   5,  1 },
  { -2,  -8,  8 },
  {  7,  -6, 14 },
  {  1, -10,  4 },
};


int main(int argc, char** argv)
{
  int64_t energy;
//  energy = simulate(example1Input, 10);
//  printf("Total energy = %lld\n", energy);
//  energy = simulate(example2Input, 100);
//  printf("Total energy = %lld\n", energy);
  energy = simulate(realInput, 1000);
  printf("Total energy = %lld\n", energy);
  return 0;
}
