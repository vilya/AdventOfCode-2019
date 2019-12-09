#include <cstdio>
#include <vector>

struct Segment {
  int x0, y0, x1, y1;
  int offset;

  int left()   const { return std::min(x0, x1); }
  int right()  const { return std::max(x0, x1); }
  int top()    const { return std::min(y0, y1); }
  int bottom() const { return std::max(y0, y1); }
};


void min_distance(const Segment& lhs, const Segment& rhs, int& nearest)
{
  int dist = lhs.offset + rhs.offset;
  if (dist >= nearest) {
    return;
  }

  Segment out;
  out.x0 = std::max(lhs.left(), rhs.left());
  out.x1 = std::min(lhs.right(), rhs.right());
  out.y0 = std::max(lhs.top(), rhs.top());
  out.y1 = std::min(lhs.bottom(), rhs.bottom());
  if (out.x0 > out.x1 || out.y0 > out.y1) {
    return;
  }

  dist += std::min(
        std::abs(lhs.x0 - out.x0) + std::abs(lhs.y0 - out.y0) + std::abs(rhs.x0 - out.x0) + std::abs(rhs.y0 - out.y0),
        std::abs(lhs.x0 - out.x1) + std::abs(lhs.y0 - out.y1) + std::abs(rhs.x0 - out.x1) + std::abs(rhs.y0 - out.y1)
  );
  if (dist > 0 && dist < nearest) {
    nearest = dist;
  }
}


bool load(const char* filename, std::vector<Segment> wires[2])
{
  FILE* f = fopen(filename, "r");
  if (f == nullptr) {
    return false;
  }

  for (int i = 0; i < 2; i++) {
    int x = 0, y = 0, offset = 0;
    while (true) {
      int dir = fgetc(f);
      if (dir == '\n' || dir == EOF) {
        break;
      }

      int dx = 0, dy = 0;
      switch (dir) {
      case 'L': dx = -1; break;
      case 'R': dx = +1; break;
      case 'U': dy = -1; break;
      case 'D': dy = +1; break;
      }

      int len;
      fscanf(f, "%d,", &len);

      Segment seg = { x, y, x + dx * len, y + dy * len, offset };
      wires[i].push_back(seg);
      x = seg.x1;
      y = seg.y1;
      offset += len;
    }
  }
  fclose(f);
  return true;
}


int main(int argc, char** argv)
{
  std::vector<Segment> wires[2];
  if (!load(argv[1], wires)) {
    fprintf(stderr, "Failed to load %s\n", argv[1]);
    return 1;
  }

  int nearest = 1 << 30;
  for (const Segment& seg0 : wires[0]) {
    for (const Segment& seg1 : wires[1]) {
      min_distance(seg0, seg1, nearest);
    }
  }
  printf("Nearest intersection is %d\n", nearest);
  return 0;
}
