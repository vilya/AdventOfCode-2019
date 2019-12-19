import sys

def is_key(ch):
  return ch >= 'a' and ch <= 'z'

def is_door(ch):
  return ch >= 'A' and ch <= 'Z'

def keyidx(ch):
  return ord(ch) - ord('a')

def keybit(ch):
  return 1 << keyidx(ch)

def dooridx(ch):
  if (ch < 'A') or (ch > 'Z'):
    print "char '%s' is not a door" % ch

  return ord(ch) - ord('A')

def doorbit(ch):
  return 1 << dooridx(ch)

def has_key(keys, door):
  return (keys & doorbit(door)) != 0

def step(pos, direction):
  delta = [
    ( 0, -1), # north
    ( 0,  1), # south
    (-1,  0), # west
    ( 1,  0), # east
  ]
  x, y = pos
  dx, dy = delta[direction]
  return x + dx, y + dy

tiles = {}
size = None
initial_entrance = None
entrances = []
keys = [None] * 26
nkeys = 0

with open(sys.argv[1]) as f:
  w = 0
  y = 0
  for line in f:
    line = line.strip()
    w = max(w, len(line))
    for x, ch in enumerate(line):
      if ch == '@':
        initial_entrance = (x, y)
        tiles[(x, y)] = ch
      elif is_key(ch):
        keys[keyidx(ch)] = (x, y)
        nkeys += 1
        tiles[(x, y)] = ch
      elif ch == '.' or is_door(ch):
        tiles[(x, y)] = ch
    y += 1
  size = (w, y)

del tiles[initial_entrance]
for direction in xrange(4):
  del tiles[step(initial_entrance, direction)]

entrances = [
  step(step(initial_entrance, 0), 2),
  step(step(initial_entrance, 0), 3),
  step(step(initial_entrance, 1), 2),
  step(step(initial_entrance, 1), 3),
]
for e in entrances:
  tiles[e] = '@'

# print the map
print "%dx%d map:" % size
for y in xrange(size[1]):
  print "".join([tiles.get((x,y), ' ') for x in xrange(size[0])])

# For each quadrant we assume that any keys not in this quadrant will be taken
# care of by one of the other robots. There's no cost for waiting without
# moving, so we don't need to simulate it. We can simply say that the robot in
# each quadrant starts with all of the keys from all other quadrants already
# and solve the quadrants independently.
keys_by_quadrant = [ 0 ] * 4
ex, ey = initial_entrance
for i in xrange(nkeys):
  kx, ky = keys[i]
  idx = (2 if ky > ey else 0) | (1 if kx > ex else 0)
  keys_by_quadrant[idx] |= (1 << i)

all_keys = (1 << nkeys) - 1
initial_visited = [ all_keys ^ k for k in keys_by_quadrant ]

# bfs
def bfs(entrance, initial_visited):
  all_keys = (1 << nkeys) - 1 
  state = {} # mapping from (x, y, visited) tuples to distances
  work = [ entrance + (initial_visited, 0) ] # each tuple is (x, y, visited, distance)
  progress = 0
  while work != []:
    x, y, visited, distance = work.pop(0)
    pos = (x, y)
    ch = tiles[pos]
    # print "pos = (%2d,%2d), visited = %28s, distance = %3d, tile = '%s'" % (pos[0], pos[1], bin(visited), distance, ch)
    if is_key(ch):
      bit = keybit(ch)
      visited |= keybit(ch)
      if visited == all_keys:
        return distance
    elif is_door(ch) and not has_key(visited, ch):
      continue
    state_key = (x, y, visited)
    state_val = state.get(state_key, 1000000)
    if distance >= state_val:
      continue
    state[state_key] = distance
    distance += 1
    work += [ step(pos, d) + (visited, distance) for d in xrange(4) if step(pos, d) in tiles ]
    progress += 1
    if progress % 1000 == 0:
      print "\r%d" % (progress/1000),
      sys.stdout.flush()
  return -1
print ""

shortest = sum(bfs(entrances[i], initial_visited[i]) for i in xrange(len(entrances)))
print "shortest path for all keys is %d" % shortest

