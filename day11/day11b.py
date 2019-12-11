import sys
sys.path.append("../intcode")
import intcode

grid = {}
pos = (0,0)
facing = (0,-1)

grid[pos] = 1

prog = intcode.load(sys.argv[1])
try:
  g = intcode.run(prog)
  g.next()
  while True:
    output = g.send(grid.get(pos, 0))
    grid[pos] = output[0]
    if output[1] == 0:
      facing = facing[1], -facing[0]
    else:
      facing = -facing[1], facing[0]
    pos = pos[0] + facing[0], pos[1] + facing[1]
except StopIteration:
  pass

x0, y0, x1, y1 = 1000000000, 1000000000, -1000000000, -1000000000
for x, y in grid.keys():
  if x < x0:
    x0 = x
  if x > x1:
    x1 = x
  if y < y0:
    y0 = y
  if y > y1:
    y1 = y;
for y in xrange(y0, y1 + 1):
  print "".join(['#' if grid.get((x,y), 0) != 0 else ' ' for x in xrange(x0, x1 + 1)])
