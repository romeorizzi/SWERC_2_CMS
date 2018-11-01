#! /usr/bin/env python3
#

import random
import sys

# from http://www.problemarchive.org/wiki/index.php/Problem_Format#Problem_Metadata

# The generators must be deterministic, i.e. always produce the same input file
# when give the same arguments.

# The generators must be idempotent, i.e. running them multiple times should
# result in the same contents of the test data directory as running them once.

random.seed(296324)

for t in range(10):
    n = 100000
    l = 10000
    with open("test{:02d}.in".format(t), "w") as fd:
            fd.write("%i\n" % n)
            for _ in range(2):
                for i in range(n):
                    if i < n-1:
                        fd.write("%i " % random.randint(1, l))
                    else:
                        fd.write("%i\n" % random.randint(1, l))
