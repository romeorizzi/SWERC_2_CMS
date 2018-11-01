#! /usr/bin/env python
#

import sys

def read_line():
    return sys.stdin.readline().strip()

if __name__ == '__main__':
    line = read_line()
    assert not line.startswith('0') 
    W = int(line)
    line = read_line()
    assert not line.startswith('0') 
    N = int(line) 
    assert 1 <= W and W <= 10000
    assert 1 <= N and N <= 5000000

    for _ in range(N):
        line = read_line().split()
        assert len(line)==2
        assert not line[0].startswith('0')
        assert not line[1].startswith('0')
        w = int(line[0])
        l = int(line[1])
        assert 1 <= w and w <= 10000
        assert 1 <= l and l <= 10000

    assert len(sys.stdin.readline()) == 0
    sys.exit(42)
