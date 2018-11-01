#! /usr/bin/env python3
#

import sys

if __name__ == '__main__':
    limits = sys.stdin.readline().strip('\n').split()
    assert len(limits) == 2
    N = int(limits[0])
    M = int(limits[1])
    assert 1 <= N <= 1000
    assert 1 <= M <= 5000
    for _ in range(N):
        s = sys.stdin.readline().strip('\n')
        assert len(s) == M
        for c in s:
            assert c in ('-', '1', '2', '3', '4', '5', '6', '7', '8', '9')
        s = sys.stdin.readline().strip('\n')
        assert len(s) == M
        for c in s:
            assert c in ('.', '|')
    assert len(sys.stdin.readline()) == 0
    sys.exit(42)
