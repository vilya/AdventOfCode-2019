import re


def is_solvable(s):
  if "____" in s:
    return False
  if re.search(r'___#_.._', s):
    return False
  if re.match(r'#_.._', s):
    return False
  if re.match(r'#_..#_.._', s):
    return False
  return True


def should_jump(s):
  # print "should_jump('%s')" % s
  A, B, C, D, E, F, G, H, I = [True if ch == '#' else False for ch in s[:9]]

  T = False
  J = False

  T = not T
  T = A and T
  T = B and T
  T = C and T
  # T == A and B and C, i.e. True if the next 3 tiles are ground, false if any of them are empty
  
  T = not T
  # T indicates whether we should jump.

  T = D and T
  # Must not jump now if D is empty.

  J = E or J
  J = H or J
  # J is False if we would have to jump again immediately after landing at D and that second jump would land on empty space

  J = T and J

  return J

'''
  "NOT B T\n"
  "NOT C J\n"
  "OR T J\n"
  "NOT E T\n"
  "AND T J\n"
  "NOT H T\n"
  "AND T J\n"
  "AND D J\n"
  "NOT A T\n"
  "OR T J\n"
'''

def test(s):
  i = 0
  while i < 9:
    print "(%s)%s" % (s[i], s[i+1:]),
    if s[i] != '#':
      print " --> fall :-("
      return False
    if should_jump(s[i+1:]):
      i += 4
      print " --> jump! (i=%d)" % i
    else:
      i += 1
      print " --> step (i=%d)" % i
  return True


def testcases():
  for i in xrange(512):
    s = bin(i)[2:].replace('1', '#').replace('0', '_')
    if len(s) < 9:
      s = ('_' * (9 - len(s))) + s
    s += ('#' * 9)
    s = '#' + s
    yield s


passed = 0
failed = 0
for s in testcases():
  if is_solvable(s):
    if test(s):
      passed += 1
    else:
      failed += 1
      print "%s  FAILED" % s

print "%d passed, %d failed" % (passed, failed)

# if test("##__#___#_#########"):
#   print "passed"
# else:
#   print "failed"
