#! /usr/bin/env python3
#

import sys

def read_ints(line = None):
    strs = (line or sys.stdin.readline()).split()
    assert all(s == "0" or not s.startswith("0") for s in strs), \
            "leading 0 detected"
    return tuple(int(s) for s in strs)

if __name__ == '__main__':
    x, y, n, d = read_ints();
    assert 1 <= x and x <= 2000
    assert 1 <= y and y <= 2000
    assert 0 <= n and n <= 1000000
    assert 1 <= d and d <= 100000

    table = [[False for _ in range(y)] for _ in range(x)]

    for _ in range(n):
        x1, x2, y1, y2 = read_ints()
        assert 0 <= x1 and x1 < x2 and x2 <= x
        assert 0 <= y1 and y1 < y2 and y2 <= y
        for xx in range(x1, x2):
            for yy in range(y1, y2):
                assert not table[xx][yy]
                table[xx][yy] = True

    for _ in range(d):
        xx, yy = read_ints()
        assert 0 < xx and xx <= x
        assert 0 < yy and yy <= y

    assert len(sys.stdin.readline()) == 0
    sys.exit(42)
