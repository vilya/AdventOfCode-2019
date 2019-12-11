import sys
sys.path.append("../intcode")
import intcode

grid = {}
pos = (0,0)
facing = (0,-1)

prog = intcode.load(sys.argv[1])
try:
  g = intcode.run(prog)
  output = g.next() # output should be empty here.
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

print "Painted %d panels at least once" % len(grid)
