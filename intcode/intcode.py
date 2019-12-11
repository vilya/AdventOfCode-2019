def load(path):
  with open(path) as f:
    line = next(f)
    prog = [int(s) for s in line.split(",")]
    if len(prog) < 128 * 1024:
      prog = prog + ([0] * (128 * 1024 - len(prog)))
    return prog
  return None


def run(mem):
  ip = 0
  rel = 0
  out = []

  def loc(i):
    mode = (mem[ip] / (10 ** (i+1))) % 10
    idx = (ip + i) if mode == 1 else mem[ip + i]
    if mode == 2:
      idx += rel
    return idx

  while True:
    op = mem[ip] % 100
    if op == 1: # add
      mem[loc(3)] = mem[loc(1)] + mem[loc(2)]
      ip += 4
    elif op == 2: # mul
      mem[loc(3)] = mem[loc(1)] * mem[loc(2)]
      ip += 4
    elif op == 3: # read
      mem[loc(1)] = (yield out)
      out = []
      ip += 2
    elif op == 4: # write
      out.append(mem[loc(1)])
      ip += 2
    elif op == 5: # jump if true
      ip = mem[loc(2)] if mem[loc(1)] != 0 else (ip + 3)
    elif op == 6: # jump if false
      ip = mem[loc(2)] if mem[loc(1)] == 0 else (ip + 3)
    elif op == 7: # less than
      mem[loc(3)] = 1 if mem[loc(1)] < mem[loc(2)] else 0
      ip += 4
    elif op == 8: # equals
      mem[loc(3)] = 1 if mem[loc(1)] == mem[loc(2)] else 0
      ip += 4
    elif op == 9: # set relative base
      rel += mem[loc(1)]
      ip += 2
    else:
      if out != []:
        yield out
      return
