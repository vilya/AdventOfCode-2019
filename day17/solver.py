def find_list(haystack, needle, start=0):
  h = len(haystack)
  n = len(needle)
  i = start
  while i < h:
    j = 0
    while j < n and (i+j) < h and needle[j] == haystack[i+j]:
      j += 1
    if j == n:
      yield i
      i += j
    else:
      i += 1


def replace_list(haystack, needle, new_needle):
  out = []
  start = 0
  n = len(needle)
  for i in find_list(haystack, needle):
    if i > start:
      out += haystack[start:i]
    out += new_needle
    start = i + n
  if start < len(haystack):
    out += haystack[start:]
  return out


def split_list(src, split_at):
  out = []
  start = 0
  for i in find_list(src, split_at):
    if start < i:
      out.append(src[start:i])
    start = i + len(split_at)
  if start < len(src):
    out.append(src[start:])
  return out


def split_lists(src_lists, split_at):
  out = []
  for src_list in src_lists:
    out += split_list(src_list, split_at)
  return out


def cost(s):
  return len(s) + 1


def find_subroutines(paths, subroutines):
  if paths == []:
    return subroutines
  elif len(subroutines) == 3:
    return None

  path = paths[0]
  sublen = 0
  subcost = 0
  while sublen < len(path) and (subcost + cost(path[sublen])) < 20:
    subcost += cost(path[sublen])
    sublen += 1

  new_subroutine = path[:sublen]
  while len(new_subroutine) > 0:
    new_subroutines = subroutines + [new_subroutine]
    soln = find_subroutines(split_lists(paths, new_subroutine), new_subroutines)
    if soln is not None:
      return soln
    new_subroutine.pop()
  return None


def master_path(path, subroutines):
  master = list(path)
  for name, subroutine in zip("ABC", subroutines):
    master = replace_list(master, subroutine, [name])
  return master


def print_soln(master, subroutines):
  for name, subroutine in zip("ABC", subroutines):
    print "%s = %s" % (name, ",".join(subroutine))
  print "master = %s" % (",".join(master))


def reconstruct_path(master, subroutines):
  out = []
  subs = dict(zip("ABC", subroutines))
  for name in master:
    out += subs[name]
  return ",".join(out)


PATHSTR = "L,10,R,8,R,8,L,10,R,8,R,8,L,10,L,12,R,8,R,10,R,10,L,12,R,10,L,10,L,12,R,8,R,10,R,10,L,12,R,10,L,10,L,12,R,8,R,10,R,10,L,12,R,10,R,10,L,12,R,10,L,10,R,8,R,8"
path = PATHSTR.split(",")
subroutines = find_subroutines([path], [])
master = master_path(path, subroutines)
print_soln(master, subroutines)
print ""
print "original path      = %s" % (PATHSTR)
print "reconstructed path = %s" % reconstruct_path(master, subroutines)
