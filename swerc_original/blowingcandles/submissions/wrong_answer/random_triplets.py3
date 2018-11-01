#!/usr/bin/env python3
# swerc 2017 - christoph dürr

"""Blowing candles

naive probabilistic heuristic  - fails

For points a,b,c determines the smallest strip covering them.
Returns the maximum over some randomly chosen points
"""

from math import sqrt
from random import randint

def readInt():      return int(input())
def readStr():      return input().strip()
def readInts():     return map(int, input().split())


def dist2(p0, p1, p2):
    """distance from point p0 to line (p1,p2)
    https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_two_points
    """
    x0, y0 = p0
    x1, y1 = p1
    x2, y2 = p2
    num = ((y2 - y1)*x0 - (x2 - x1)*y0 + x2 * y1 - y2 * x1) ** 2
    denom = (y2 - y1)**2 + (x2 - x1)**2
    return num / denom


def strip(p0, p1, p2):
    """ smallest strip covering the given points
    """
    return min(dist2(p0, p1, p2), dist2(p1, p0, p2), dist2(p2, p0, p1))


def solve(S):
    n = len(S)
    best = 0
    for i in range(1000000):
        a = i % n
        b = (7 * i + 3 ) % n
        c = (13 * i + 5) % n
        if a == b or a == c or b == c:
            continue
        best = max(best, strip(S[a], S[b], S[c]))
    return sqrt(best)    # compute just once sqrt. Does not improve much (reduce by factor 0.9)


N, radius = readInts()
S = [tuple(readInts()) for _ in range(N)]
print(solve(S))
