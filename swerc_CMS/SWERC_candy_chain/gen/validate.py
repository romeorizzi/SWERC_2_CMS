#! /usr/bin/env python3
#

import sys

def myint(s):
    assert s == "0" or not s.startswith("0"),\
            "leading 0 detected"
    return int(s)        

def check_string(s):
    assert len(s) > 0
    assert len(s) <= 50
    for c in s:
        assert c >= 'a'
        assert c <= 'z'

if __name__ == '__main__':
    s = sys.stdin.readline().strip('\n')
    check_string(s)
    C = myint(sys.stdin.readline().strip('\n'))
    assert C > 0
    assert C <= 200
    for _ in range(C):
        line = sys.stdin.readline().strip('\n')
        splitted = line.split()
        assert len(splitted) == 2
        word = splitted[0]
        check_string(word)
        score = myint(splitted[1])
        assert score >= 0
        assert score <= 1000000
    assert len(sys.stdin.readline()) == 0
    sys.exit(42)
