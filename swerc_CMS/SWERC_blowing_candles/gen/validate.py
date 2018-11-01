#! /usr/bin/env python3
#

import sys

def read_ints(line = None):
    strs = (line or sys.stdin.readline()).split()
    assert all(s == "0" or not s.startswith("0") for s in strs), \
            "leading 0 detected"
    return tuple(int(s) for s in strs)

if __name__ == '__main__':
    n, r = read_ints();
    assert 3 <= n and n <= 200000
    assert 10 <= r and r <= 200000000

    seen = set()

    for _ in range(n):
        x, y = read_ints()
        assert x*x + y*y <= r*r
        assert not (x, y) in seen
        seen.add((x, y))

    assert len(sys.stdin.readline()) == 0
    sys.exit(42)
