#include <tiny-intcode.h>

#include <cstdio>
#include <cstdlib>
#include <string>


std::string output_to_string(std::vector<int64_t>& output)
{
  std::string s;
  for (int64_t val : output) {
    s.push_back(static_cast<char>(val));
  }
  return s;
}

/*
 * Jumps last for 4 steps. Need to make sure there is ground 4 steps away when
 * jumping.
 *
 * These are the possible combinations for the next 4 tiles (0 = ground, 1 = hole):
 * - 0b0000
 * - 0b0001
 * - 0b0010
 * - 0b0011
 * - 0b0100
 * - 0b0101
 * - 0b0111
 * - 0b1000
 * - 0b1001
 * - 0b1010
 * - 0b1011
 * - 0b1100
 * - 0b1101
 * - 0b1110
 * - 0b1111
 *
 * Note that:
 * - Any leading or trailing 0s can be ignored. Combinations which only differ
 *   by leading or trailing 0s are equivalent.
 * - 0b1111 is impossible to jump over, so assume it can't occur in the input.
 * - 0b0000 is impossible to fail on, can ignore this.
 *
 * So, combinations we need to care about are:
 * - 0b1
 * - 0b11
 * - 0b101
 * - 0b111
 * - 0b1001
 * - 0b1011
 * - 0b1101
 *
 * Never jump if D is false (i.e. no ground 4 steps away).
 * Always jump if we're on the ground and A is false (i.e. no ground at next step)
 *
 * Jump if any of A, B or C are false and D is true, i.e. ((!A) | (!B) | (!C)) && D
 *
 * NOT A J
 * NOT B T
 * OR T J
 * NOT C T
 * OR T J
 * AND D J
 */

int main(int argc, char** argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <prog.txt>\n", argv[0]);
    return EXIT_FAILURE;
  }

  vh::IntcodeComputer prog;
  if (!prog.load(argv[1])) {
    fprintf(stderr, "Couldn't open %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  prog.run(0ll); // Prints the 'Input Instruction' prompt
  prog.run(
        "OR  A J\n"
        "AND B J\n"
        "AND C J\n"
        // J is now false if any of A, B or C are empty

        "NOT J J\n"
        // J is now true if we need to jump

        "AND D J\n"
        // J is false if jumping now would land us on an empty tile.

        "WALK\n");
  if (prog.out.back() < 0 || prog.out.back() >= 256) {
    printf("Success! Damage to hull: %lld\n", prog.out.back());
    return EXIT_SUCCESS;
  }
  else {
    std::string out = output_to_string(prog.out);
    printf("%s\n", out.c_str());
    return EXIT_FAILURE;
  }
}
