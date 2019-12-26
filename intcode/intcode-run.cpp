#include <tiny-intcode.h>

#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <cstdarg>
#include <string>


void print_output(std::vector<int64_t>& output)
{
  for (int64_t val : output) {
    if (val >= 0 && val < 255) {
      fputc(static_cast<char>(val), stdout);
    }
    else {
      fprintf(stdout, "%lld", val);
    }
  }
}


std::string format(const char* fmt, ...)
{
  char buf[1024];

  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);

  return std::string(buf);
}


struct Input {
  int numScripts = 0;
  char** scripts = nullptr;
  int scriptIndex = 0;
  FILE* script = nullptr;
  char line[1024];


  Input(int n, char** s) {
    numScripts = n;
    scripts = s;
    scriptIndex = 0;
    script = nullptr;
    if (scriptIndex < numScripts) {
      script = fopen(scripts[scriptIndex], "r");
      if (!script) {
//        fprintf(stderr, "Couldn't open input script %s\n", scripts[scriptIndex]);
        throw std::runtime_error(format("Couldn't open input script %s\n", scripts[scriptIndex]));
      }
    }
  }


  ~Input() {
    if (script != nullptr) {
      fclose(script);
    }
  }


  void next_script() {
    if (script == nullptr) {
      return;
    }
    fclose(script);
    script = nullptr;
    ++scriptIndex;
    if (scriptIndex >= numScripts) {
      return;
    }

    script = fopen(scripts[scriptIndex], "r");
    if (!script) {
      throw std::runtime_error(format("Couldn't open input script %s\n", scripts[scriptIndex]));
    }
  }


  void read_line() {
    while (script != nullptr) {
      char* result = nullptr;
      do {
        result = fgets(line, sizeof(line), script);
      } while (result != nullptr  && *result == '#');

      if (result != nullptr) {
        printf("%s", line);
        return;
      }

      // If we've exhausted all input in the current script, move to the next
      // one (if any).
      next_script();
    }

    fgets(line, sizeof(line), stdin);
  }
};


int main(int argc, char** argv)
{
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <prog.txt> [ <script.txt> ... ]\n", argv[0]);
    return EXIT_FAILURE;
  }

  try {
    vh::IntcodeComputer prog;
    if (!prog.load(argv[1])) {
      fprintf(stderr, "Couldn't open %s\n", argv[1]);
      return EXIT_FAILURE;
    }

    Input input(argc - 2, argv + 2);
    int running = prog.run();
    while (running != 0) {
      print_output(prog.out);
      input.read_line();
      running = prog.run(input.line);
    }
    print_output(prog.out);
  }
  catch (const std::runtime_error& ex) {
    fprintf(stderr, "%s\n", ex.what());
  }

  return EXIT_SUCCESS;
}
