#! /usr/bin/env python3
#

import sys

def read_ints():
    strs = sys.stdin.readline().split()
    assert all(s == "0" or not s.startswith("0") for s in strs), \
            "leading 0 detected"
    return tuple(int(s) for s in strs)

def check_bounds(low, high):
    ints = read_ints()
    assert all(i >= low and i <= high for i in ints), \
            "value out of bounds {}..{} in {}".format(low, high, ints)
    return ints

if __name__ == '__main__':
    b, c = check_bounds(1, 1000)
    for _ in range(b+c+1):
        check_bounds(-1000, 1000)
    assert len(sys.stdin.readline()) == 0, "extra lines in input file"
    sys.exit(42)
