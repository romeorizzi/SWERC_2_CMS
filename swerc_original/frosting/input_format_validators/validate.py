#! /usr/bin/env python3
#

import sys

def read_ints(line = None):
    strs = (line or sys.stdin.readline()).split()
    assert all(s == "0" or not s.startswith("0") for s in strs), \
            "leading 0 detected"
    return tuple(int(s) for s in strs)

def check_bounds(low, high):
    ints = read_ints()
    assert all(i >= low and i <= high for i in ints)
    return ints

if __name__ == '__main__':
    ns = check_bounds(3, 100000)
    assert len(ns) == 1
    n = ns[0]
    assert len(check_bounds(1, 10000)) == n
    assert len(check_bounds(1, 10000)) == n
    assert len(sys.stdin.readline()) == 0
    sys.exit(42)
