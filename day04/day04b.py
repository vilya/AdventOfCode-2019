def preprocess(num):
  for i in xrange(1, len(num)):
    if num[i] < num[i-1]:
      num[i:] = [num[i-1]] * (len(num) - i)
      return


def increment(num):
  i = len(num) - 1
  while i >= 0 and num[i] == 9:
    i -= 1
  if i < 0:
    num.insert(0, 1)
  else:
    num[i] += 1
  num[(i+1):] = [num[i]] * (len(num) - i - 1)


def valid(num):
  i = 0
  while i < len(num):
    j = i + 1
    while j < len(num) and num[j] == num[i]:
      j += 1
    if j - i == 2:
      return True
    i = j
  return False


num = [1, 0, 8, 4, 5, 7] + [0] * 100
end = [5, 6, 2, 0, 4, 1] + [0] * 100
count = 0
preprocess(num)
while num < end:
  if valid(num):
    count += 1
  increment(num)
print "Answer is %d" % count

