import sys

'''

This takes an intcode program written in my invented intcode assembly language
and assembles it into an executable intcode program. See
benchmarks/vil-ackermann-src.txt for an example of the input syntax. The
assembled intcode is written to stdout; redirect stdout to a file of your
choice to save it.

For example:

  python intcode-assemble.py benchmarks/vil-ackermann-src.txt > vil-ackermann.txt

You can then run vil-ackermann.txt using the intcode-benchmark executable
built as part of this project, e.g.

  ../../build-release/intcode-benchmark vil-ackermann.txt 3 6

'''

labels = {}
prog = []
with open(sys.argv[1]) as f:
  for line in f:
    # strip off trailing comments
    if '#' in line:
      line = line[:line.find('#')]

    # remove leading and trailing whitespace
    line = line.strip()

    # if the line ends with a comma, remove it
    if line.endswith(','):
      line = line[:-1]

    # if the line starts with a label, save the label
    if ':' in line:
      label = line[:line.find(':')]
      line = line[line.find(':')+1:].lstrip()
      labels[label] = str(len(prog))
      # print "found label %s at idx %d" % (label, idx)

    if line == "":
      continue

    vals = line.split(',')
    prog += vals

# replace labels with their positions and output the resulting string
prog_str = ",".join(labels.get(op, op) for op in prog)
print prog_str,
print >>sys.stderr, "\n".join(str(x) for x in sorted(labels.iteritems()))
