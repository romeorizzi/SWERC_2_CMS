#! /usr/bin/env python3
#

import sys

def check_line(n):
    line = sys.stdin.readline().strip('\n')
    splitted = line.split()
    assert all(s == "0" or not s.startswith("0") for s in splitted), \
            "leading 0 detected"
    integers = [int(e) for e in splitted]
    assert len(integers) == n
    for e in integers:
        assert e >= 0
        assert e <= 1000000000
    assert sorted(integers) == integers
    # Check no dups:
    dups = []
    for i in range(len(integers)):
        if i == 0: continue
        if integers[i - 1] == integers[i]: dups.append(integers[i])
    assert len(dups) == 0, 'Dup: %s' % dups


if __name__ == '__main__':
    N = int(sys.stdin.readline().strip('\n'))
    M = int(sys.stdin.readline().strip('\n'))
    assert N >= 1
    assert N <= 2000
    assert M >= 1
    assert M <= 2000
    check_line(N)
    check_line(M)
    assert len(sys.stdin.readline()) == 0
    sys.exit(42)
