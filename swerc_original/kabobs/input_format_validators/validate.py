#! /usr/bin/env python3
#

import sys
import re

def myint(s):
    assert s == "0" or not s.startswith("0"),\
            "leading 0 detected"
    return int(s)        

def check_alph(s):
    for c in s:
        assert( ('a' <= c <= 'z') or ('A' <= c <= 'Z') or ('0' <= c <= '9') )

def check_diff(s):
    seen = []
    for c in s:
        assert(c not in seen)
        seen.append(c)
            
def check_rule(r):
    check_diff(r)
    b,e = r.split('>')
    assert(len(b)>=1)
    assert(len(e)>=1)
    check_alph(b)
    check_alph(e)

if __name__ == '__main__':
    s = sys.stdin.readline().strip('\n')
    steps, alphabet = s.split()
    steps = myint(steps)
    assert( 0 < steps <= 500 )
    check_diff(alphabet)
    check_alph(alphabet)
    C = sys.stdin.readline().strip('\n')
    assert( 3 <= len(C)<= 60 )
    for r in C.split('|'):
        check_rule(r)
    for _ in sys.stdin:
        assert False
    sys.exit(42)
