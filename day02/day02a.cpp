#include <tiny-intcode.h>

#include <cstdio>

int main(int argc, char** argv)
{
  vh::IntcodeComputer prog;
  if (!prog.load(argv[1])) {
    fprintf(stderr, "Failed to load %s\n", argv[1]);
    return 1;
  }
  prog.mem[1] = 12;
  prog.mem[2] = 2;
  prog.run();
  printf("Value is %lld\n", prog.mem[0]);
  return 0;
}
