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
      "OR A J\n"
      "AND B J\n"
      "AND C J\n"
      // J == A and B and C, i.e. true if the next 3 tiles are ground, false if any of them are empty

      "NOT J J\n"
      // T now indicates whether we should jump.

      "AND D J\n"
      // Must not jump now if D is empty.

      "OR E T\n"
      "OR H T\n"
      // T is false if we would have to jump again immediately after landing at D and that second jump would land on empty space

      "AND T J\n"
      // Bring them both together.

      "RUN \n");
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
