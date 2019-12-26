import itertools

items = ["mug", "easter egg", "shell", "space heater", "festive hat", "whirled peas", "sand", "weather machine"]
inv = 2 ** len(items) - 1
for n in xrange(1, 2 ** len(items)):
  print "# Combination %d" % n
  for i in xrange(len(items)):
    bit = 1 << i
    holding_item = (inv & bit) != 0
    want_item = (n & bit) != 0
    if holding_item != want_item:
      if holding_item:
        print "drop %s" % items[i]
      else:
        print "take %s" % items[i]
  print "south"
  print "# If we had the right weight we're at C now, otherwise at B"
  inv = n
