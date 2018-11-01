# Inspired from radar_louis_n3_fast.cc, but in python.
# Since python has a much bigger time limit, we double-check that this
# O(N^3) solution does not pass in python.

# Compatibility with python2 (for pypy)
from __future__ import absolute_import, division, print_function

import sys
from collections     import Counter

# Compatibility with python2 (for pypy) and 3.
input_fun = input if sys.version_info >= (3, 0) else raw_input

def readInt():      return int(input_fun())
def readInts():     return map(int, input_fun().split())

def solve(A, B):
    N = len(A)
    M = len(B)
    B.append(A[-1] + B[-1])
    assoc = [0]*len(A)
    best_score = 0
    best_diff = 0
    diff = 0
    while assoc[0] != M:
        score = 0
        next_diff = B[-1]
        for i in xrange(N):
            if assoc[i] == M: break
            while A[i] + diff > B[assoc[i]]: assoc[i] += 1
            if A[i] + diff == B[assoc[i]]:
                score += 1
                assoc[i] += 1
            new_diff = B[assoc[i]] - A[i]
            if new_diff < next_diff: next_diff = new_diff
        if score > best_score: best_score, best_diff = score, diff
        diff = next_diff
    return best_diff


N = readInt()
M = readInt()
A = list(readInts())
B = list(readInts())
print(solve(A, B))
