#include <tiny-intcode.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>


struct ivec2 {
  int64_t x, y;

  ivec2& operator += (ivec2 a) { x += a.x; y += a.y; return *this; }
};
bool operator == (ivec2 a, ivec2 b) { return a.x == b.x && a.y == b.y; }
bool operator <  (ivec2 a, ivec2 b) { return (a.x != b.x) ? (a.x < b.x) : (a.y < b.y); }

ivec2 operator + (ivec2 a, ivec2 b) { return ivec2{ a.x + b.x, a.y + b.y }; }


constexpr ivec2 kDelta[] = {
  ivec2{  0, -1 }, // north = 0 = 0b00
  ivec2{  0,  1 }, // south = 1 = 0b01
  ivec2{ -1,  0 }, // west  = 2 = 0b10
  ivec2{  1,  0 }, // east  = 3 = 0b11
};


static constexpr int kLeft  = -1;
static constexpr int kRight = -2;


inline static constexpr ivec2 turn_left(ivec2 dir)
{
  return ivec2{ dir.y, -dir.x };
}


inline static constexpr ivec2 turn_right(ivec2 dir)
{
  return ivec2{ -dir.y, dir.x };
}


static std::vector<int> find_list(const std::vector<int>& haystack, const std::vector<int>& needle)
{
//  std::vector<int> found;
//  int h = static_cast<int>(haystack.size());
//  int n = static_cast<int>(needle.size());
//  int i = 0;
//  while (i < h) {
//    int64_t j = 0;
//    while (j < n && (i + j) < h && haystack[i+j] == needle[j]) {
//      j++;
//    }
//    if (j == n) {
//      found.push_back(i);
//      i += j;
//    }
//    else {
//      ++i;
//    }
//  }
//  return found;

  std::vector<int> found;
  auto start = haystack.begin();
  auto pos = std::search(start, haystack.end(), needle.begin(), needle.end());
  while (pos != haystack.end()) {
    found.push_back(int(std::distance(haystack.begin(), pos)));
    start += int64_t(needle.size());
    pos = std::search(start, haystack.end(), needle.begin(), needle.end());
  }
  return found;
}


static std::vector<int> replace_list(const std::vector<int>& haystack, const std::vector<int>& needle, const std::vector<int>& newNeedle)
{
  std::vector<int> found = find_list(haystack, needle);
  if (found.empty()) {
    return haystack;
  }

  std::vector<int> replaced;
  replaced.reserve(haystack.size() - needle.size() * found.size() + newNeedle.size() * found.size());

  const int n = static_cast<int>(needle.size());
  int start = 0;
  for (int i : found) {
    if (i > start) {
      replaced.insert(replaced.end(), haystack.begin() + start, haystack.begin() + i);
    }
    replaced.insert(replaced.end(), newNeedle.begin(), newNeedle.end());
    start = i + n;
  }
  if (start < int(haystack.size())) {
    replaced.insert(replaced.end(), haystack.begin() + start, haystack.end());
  }

  return replaced;
}


static std::vector<std::vector<int>> split_list(const std::vector<int>& src, const std::vector<int>& splitAt)
{
  std::vector<std::vector<int>> out;

  std::vector<int> found = find_list(src, splitAt);
  if (found.empty()) {
    out.push_back(src);
    return out;
  }

  const int n = static_cast<int>(splitAt.size());
  
  int start = 0;
  for (int i : found) {
    if (start < i) {
      out.push_back(std::vector<int>(src.begin() + start, src.begin() + i));
    }
    start = i + n;
  }
  if (start < int(src.size())) {
    out.push_back(std::vector<int>(src.begin() + start, src.end()));
  }
  return out;
}


static std::vector<std::vector<int>> split_lists(const std::vector<std::vector<int>>& srcLists, const std::vector<int>& splitAt)
{
  std::vector<std::vector<int>> out;
  for (const std::vector<int>& srcList : srcLists) {
    std::vector<std::vector<int>> tmp = split_list(srcList, splitAt);
    if (!tmp.empty()) {
      out.insert(out.end(), tmp.begin(), tmp.end());
    }
  }
  return out;
}


static int cost(int val) 
{
  if (val <= 0) {
    // We use values < 0 to represent left or right turns. These get written
    // out as a single 'L' or 'R' character. There's also a trailing comma or
    // newline we have to account for.
    //
    // We should never encounter values == 0 in practice because it would
    // effectively mean "stand still and do nothing", so could always be
    // removed without changing which locations get visited.
    return 2;
  }

  int result = (val < 0) ? 2 : 1; // Allow for the minus sign if any and the trailing comma or newline.
  val = std::abs(val);
  while (val > 0) {
    ++result;
    val /= 10;
  }
  return result;
}


static bool find_subroutines(const std::vector<std::vector<int>>& paths, 
                             const std::vector<std::vector<int>>& subroutines,
                             std::vector<std::vector<int>>& out)
{
  if (paths.empty()) {
    out = subroutines;
    return true;
  }
  else if (subroutines.size() >= 3) {
    return false;
  }

  const std::vector<int>& path = paths.front();
  const int n = static_cast<int>(path.size());
  const int maxCost = 20;

  // Find the maximum length subroutine
  int sublen = 0;
  int subcost = 0;
  while (sublen < n && (subcost + cost(path[sublen])) < maxCost) {
    subcost += cost(path[sublen]);
    ++sublen;
  }

  // Test whether we can find a solution with this subroutine. If not, chop
  // one item off the end and try again. Repeat until either we find a
  // solution or we run out of things to chop off.
  std::vector<int> newSubroutine(path.begin(), path.begin() + sublen);
  while (!newSubroutine.empty()) {
    std::vector<std::vector<int>> newSubroutines = subroutines;
    newSubroutines.push_back(newSubroutine);
    if (find_subroutines(split_lists(paths, newSubroutine), newSubroutines, out)) {
      return true;
    }
    newSubroutine.pop_back();
  }
  return false;
}


static std::vector<int> master_path(const std::vector<int> path, const std::vector<std::vector<int>>& subroutines)
{
  int name = 'A';

  std::vector<int> master = path;
  for (const std::vector<int>& subroutine : subroutines) {
    master = replace_list(master, subroutine, std::vector<int>{ name });
    ++name;
  }
  return master;
}


static std::string master_to_string(const std::vector<int>& master)
{
  std::string out;
  bool comma = false;
  for (int val : master) {
    if (comma) {
      out.push_back(',');
    }
    else {
      comma = true;
    }
    out.push_back(static_cast<char>(val));
  }
  out.push_back('\n');
  return out;
}


static std::string path_to_string(const std::vector<int>& path)
{
  std::string out;

  char buf[32];
  bool comma = false;
  for (int val : path) {
    if (comma) {
      out.push_back(',');
    }
    else {
      comma = true;
    }

    if (val == kLeft) {
      out.push_back('L');
    }
    else if (val == kRight) {
      out.push_back('R');
    }
    else {
      snprintf(buf, sizeof(buf), "%d", val);
      for (int i = 0; i < 32 && buf[i] != '\0'; i++) {
        out.push_back(buf[i]);
      }
    }
  }
  out.push_back('\n');
  return out;
}


struct Data {
  vh::IntcodeComputer src;
  std::map<ivec2, char> map;
  ivec2 size{ 0, 0 };
  ivec2 robot{ 0, 0 };
  ivec2 facing{ 0, 0 };
  std::vector<int> path;


  bool load(const char* filename) {
    if (!src.load(filename)) {
      return false;
    }
    // Use a *copy* of the source program to get the map.
    vh::IntcodeComputer prog = src;
    prog.run();
    return load_map(prog.out);
  }


  bool load_map(const std::vector<int64_t>& data) {
    ivec2 pos{ 0, 0 };
    char dirCode = '\0';
    for (int64_t v : data) {
      if (v == '\n') {
        if (pos.x == 0) {
          break; // an empty line ends the map
        }
        size.x = std::max(pos.x, size.x);
        pos.x = 0;
        pos.y++;
      }
      else if (v == '#' || v == '<' || v == '>' || v == '^' || v == 'v') {
        if (v != '#') {
          robot = pos;
          dirCode = static_cast<char>(v);
        }
        map[pos] = '#';
        pos.x++;
      }
      else {
        pos.x++;
      }
    }
    size.y = pos.y;

    // Get the robot's initial direction
    switch (dirCode) {
    case '^': facing = kDelta[0]; break; // north
    case 'v': facing = kDelta[1]; break; // south
    case '<': facing = kDelta[2]; break; // east
    case '>': facing = kDelta[3]; break; // west
    default: return false;
    }
    return true;
  }


  void print_map() const {
    for (int y = 0; y < size.y; y++) {
      for (int x = 0; x < size.x; x++) {
        auto it = map.find(ivec2{x, y});
        if (it != map.end()) {
          fputc(it->second, stdout);
        }
        else {
          fputc('.', stdout);
        }
      }
      fputc('\n', stdout);
    }
  }


  void build_path() {
    ivec2 pos = robot;
    ivec2 dir = facing;

    while (true) {
      // Try to move forward
      int forward = 0;
      ivec2 next = pos + dir;
      while (map.count(next) > 0) {
        forward++;
        pos = next;
        next = pos + dir;
      }
      if (forward > 0) {
        path.push_back(forward);
      }

      ivec2 nextdir = turn_left(dir);
      if (map.count(pos + nextdir) > 0) {
        dir = nextdir;
        path.push_back(kLeft);
        continue;
      }

      nextdir = turn_right(dir);
      if (map.count(pos + nextdir) > 0) {
        dir = nextdir;
        path.push_back(kRight);
        continue;
      }

      // If we get here, we've reached the end of the path.
      return;
    }
  }
};


template <class T>
void print_output(const std::vector<T>& out)
{
  for (T ch : out) {
    if (ch >= 0 && ch < 256)  {
      fputc(static_cast<char>(ch), stdout);
    }
    else {
      printf("%lld", int64_t(ch));
    }
  }
}


void print_path(const std::vector<int>& path)
{
  if (path.empty()) {
    return;
  }
  bool needComma = false;
  for (int action : path) {
    if (needComma) {
      fputc(',', stdout);
    }
    else {
      needComma = true;
    }

    if (action == kLeft) {
      fputc('L', stdout);
    }
    else if (action == kRight) {
      fputc('R', stdout);
    }
    else {
      printf("%d", action);
    }
  }
  printf("\n");
}


int main(int argc, char** argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <prog.txt>\n", argv[0]);
    return EXIT_FAILURE;
  }

  Data data;
  if (!data.load(argv[1])) {
    fprintf(stderr, "Couldn't open %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  data.build_path();

  std::vector<std::vector<int>> paths;
  paths.push_back(data.path);

  std::vector<std::vector<int>> solution;
  if (!find_subroutines(paths, std::vector<std::vector<int>>{}, solution)) {
    fprintf(stderr, "Failed to find subroutines.\n");
    return EXIT_FAILURE;
  }

  std::string movement = master_to_string(master_path(data.path, solution));
  const char* videoFeed = "n\n";

  data.src.mem[0] = 2;

  if (data.src.run() == 0) {
    print_output(data.src.out);
    fprintf(stderr ,"Exit early (initial)\n");
    return EXIT_FAILURE;
  }
  print_output(data.src.out);
  printf("%s", movement.c_str());
  if (data.src.run(movement.c_str()) == 0) {
    print_output(data.src.out);
    fprintf(stderr ,"Exit early (movement)\n");
    return EXIT_FAILURE;
  }
  for (int i = 0; i < 3; i++) {
    print_output(data.src.out);
    std::string subroutineStr = path_to_string(solution[i]);
    printf("%s", subroutineStr.c_str());
    if (data.src.run(subroutineStr.c_str()) == 0) {
      print_output(data.src.out);
      fprintf(stderr ,"Exit early (A)\n");
      return EXIT_FAILURE;
    }
  }
  print_output(data.src.out);
  printf("%s", videoFeed);
  data.src.run(videoFeed);
  print_output(data.src.out);
  printf("\n");
  return EXIT_SUCCESS;
}
