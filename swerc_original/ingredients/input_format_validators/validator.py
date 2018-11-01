#! /usr/bin/env python
#

import sys

def read_line():
    return sys.stdin.readline().strip()

if __name__ == '__main__':
    line = read_line()
    if int(line)!=0: assert not line.startswith('0') 
    B = int(line)
    line = read_line()
    if int(line)!=0: assert not line.startswith('0') 
    N = int(line) 
    assert 0 <= B and B <= 10000
    assert 0 <= N and N<= 1000000

    for _ in range(N):
        line = read_line().split()
        assert len(line)==5
        d = line[0]
        o = line[1]
        i = line[2]
        assert 1 <= len(d) and len(d) <= 20
        assert 1 <= len(o) and len(o) <= 20
        assert 1 <= len(i) and len(i) <= 20
        assert not line[3].startswith('0')
        assert not line[4].startswith('0')
        c = int(line[3])
        p = int(line[4])
        assert 1 <= c and c <= 10000
        assert 1 <= p and p <= 10000

    assert len(sys.stdin.readline()) == 0
    sys.exit(42)
