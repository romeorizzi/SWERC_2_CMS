#! /usr/bin/env python3
#

import sys

def myint(s):
    assert s == "0" or not s.startswith("0"),\
            "leading 0 detected"
    return int(s)        

if __name__ == '__main__':
    N = myint(sys.stdin.readline().strip('\n'))
    assert 1 <= N <= 8
    M = myint(sys.stdin.readline().strip('\n'))
    assert 1 <= M <= 10**18
    assert len(sys.stdin.readline()) == 0
    sys.exit(42)
