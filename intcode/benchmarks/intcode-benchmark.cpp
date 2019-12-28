#define INTCODE_MEM_SIZE (1024 * 1024)
#include <tiny-intcode.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <cstdarg>
#include <string>


class Timer {
public:
  Timer() {
    _start = _stop = std::chrono::high_resolution_clock::now();
  }

  void start() {
    _start = _stop = std::chrono::high_resolution_clock::now();
    _running = true;
  }

  void stop() {
    if (_running) {
      _stop = std::chrono::high_resolution_clock::now();
      _running = false;
    }
  }

  void resume() {
    if (!_running) {
      _start = std::chrono::high_resolution_clock::now() - _stop + _start;
      _running = true;
    }
  }

  double elapsedMS() const {
    std::chrono::duration<double, std::chrono::milliseconds::period> msecs =
       (_running ? std::chrono::high_resolution_clock::now() : _stop) - _start;
    return msecs.count();
  }

private:
  std::chrono::high_resolution_clock::time_point _start;  //!< Timestamp for when the timer was started.
  std::chrono::high_resolution_clock::time_point _stop;   //!< Timestamp for when the time was stopped.
  bool _running = false;  //!< Track whether we're currently timing or not.
};


void print_numeric_output(std::vector<int64_t>& output)
{
  if (output.empty()) {
    return;
  }
  printf("%lld", output.front());
  for (size_t i = 1, endI = output.size(); i < endI; i++) {
    printf(",%lld", output[i]);
  }
  printf("\n");
}


int main(int argc, char** argv)
{
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <prog.txt> [ <arg> ... ]\n", argv[0]);
    return EXIT_FAILURE;
  }

  try {
    vh::IntcodeComputer prog;
    if (!prog.load(argv[1])) {
      fprintf(stderr, "Couldn't open %s\n", argv[1]);
      return EXIT_FAILURE;
    }

    std::deque<int64_t> input;
    for (int i = 2; i < argc; i++) {
      input.push_back(std::stoll(argv[i]));
    }

    Timer timer;
    timer.start();
    int running = prog.run(input);
    timer.stop();
    if (running != 0) {
      fprintf(stderr, "Not enough input provided.\n");
      return EXIT_FAILURE;
    }

    print_numeric_output(prog.out);

    printf("Intcode completed in %.3lf ms\n", timer.elapsedMS());
  }
  catch (const std::runtime_error& ex) {
    fprintf(stderr, "%s\n", ex.what());
  }

  return EXIT_SUCCESS;
}
