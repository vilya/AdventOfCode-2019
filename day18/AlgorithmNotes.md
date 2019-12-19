Day 18 Notes
======================

Part 1
------

Removing dead ends (including doors with nothing behind them) simpifies the map a lot.
- For my input, it reduces the number of visitable locations from 3201 to 1979.

In my input, the entrance sits in the middle of a 3x3 area of open floor
space. Other than within this area, the map contains no loops. We can remove
the loops within this area by deleting the open positions immediately to the
left and right of the entrance.
- Don't remove the loops from the central area when doing a BFS (see below),
  it isn't necessary and it throws the results off.

No loops means there is exactly one (possibly empty) set of keys which are
required to reach each of the remaining keys. This gives us some constraints
on the order we can visit the keys in.

No loops also means that some keys can only be reached by passing through
other keys, so they cannot be visited before those keys. That further
constrains the order we can visit them in.


Part 1 Algorithm
----------------

Build up a table of distances from each key to every other key that can come
after it in the sequence.

Iterate over all possible key orders which satisfy our constraints. For any
given order the path length will be the sum of the distances between each
consecutive pair (which we computed earlier).

THE ABOVE DOES NOT RUN IN A REASONABLE AMOUNT OF TIME

Better solution (gleaned from the solutions megathread on reddit) is to do a
BFS over a graph where the key for each node is (x, y, keys_collected).


Part 2
------

I worked on the assumption that if one robot got stuck at a door, one of the
other robots would always be able to pick up the necessary key while it
waited. This worked out for the input I was given, but it may not be be true if
there are dependencies in both directions between two quadrants.

Anyway, given the shaky assumption above: there's no added cost for a robot
sitting still while it waits for one of the other robots to pick up a key, so
we don't have to simulate it. We can just give each robot all the keys from
the other quadrants as a starting point and run them through to completion
one-by-one.
