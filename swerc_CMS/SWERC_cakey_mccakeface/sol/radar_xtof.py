#!/usr/bin/env python
# -*- coding: utf-8 -*-
# c.durr - swerc - 2017

""" Radar

For two list of integers A, B find offset p such that B has maximal intersection with A + p.

Complexity: O(K^2)
For every element b in B determine the offsets that would match b with some element in A.
Just return the offset that appeared most often.
"""

# Compatibility with python2 (for pypy)
from __future__ import absolute_import, division, print_function

import sys
from collections     import Counter

# Compatibility with python2 (for pypy) and 3.
input_fun = input if sys.version_info >= (3, 0) else raw_input

def readInt():      return int(input_fun())
def readInts():     return map(int, input_fun().split())

def solve(A, B):
    C = Counter()
    for b in B:
        for a in A:
            if a <= b:
                C[b - a] += 1
    answer = 0
    best = 0
    for offset in C:
        score = C[offset]
        if score > best or ( score == best and offset < answer ):
            answer = offset
            best = score
    return answer


def solve2(A, B):
    C = Counter([b - a for b in B for a in A if a <= b])
    try:
        # default argument of max() not available in python2.
        score, _offset = max(((C[d], -d) for d in C)) #, default=(0,0))
    except ValueError:
        score, _offset = 0, 0
        #    print(score)
    return - _offset


# T = readInt()
# for _ in range(T):
N = readInt()
M = readInt()
# A = read01list()
# B = read01list()
A = list(readInts())
B = list(readInts())
print(solve2(A, B))
