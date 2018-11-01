#!/usr/bin/env python3
# swerc 2017 - christoph dürr

"""Table

Linear time solution in X*Y. Problem is similar to the largest rectangle under
a histogram problem.

Overall picture: Input is a 01-matrix A. Output is an integer matrix C of same
dimension with C[w][h] being the number of rectangles in A of dimensions w * h
containing only 0's.

Input: A can be constructed easily in time X * Y since the given 1-rectangles
are disjoint.

Idea 1: solve the problem independently for each row.

For a fixed row y build a histogram H such that H[x] is the maximal number of
zeros over the the cell (x,y).  In one pass over H, determine the maximal
rectangles of the form [x1,x2,h] such that h is the minimum H-value between x1
(included) and x2 (excluded), and H[x1 - 1] < h as well as H[x2] < h. In other
words the rectangle lays on the base row y and cannot be extended to the left,
right or top.  Count in a matrix B these rectangles positively, meaning
increment B[w][h] for w = x2 - x1.

These rectangles are detected with the use of a stack S. The stack contains
pairs of the form (x1, h), such that x1 is the left end and h the height of a
maximal rectangle for which the right end has not yet been discovered.  When a
new entry H[x2] is considered, then all pairs (x1,h) are popped from S for
which h > H[x2].  Each triplet (x1, x2, h) describes a maximal rectangle.
After that (x1, H[x2]) is pushed on the stack with x1 being either the value
from the last popped pair, or x1=x2 if there was no pop.

Idea 2: When a rectangle [x1, x2, h] is counted positively we count negatively
its intersection with another maximal rectangle [x0, x3, h'] with x0 < x1, x3
>= x2 and h' < h and h' maximal.  The height h' is easy to find, it is
corresponds to the second top most pair on the stack or to H[x2].  As a result
every cell in the histogram grid is covered by exactly one more positive
maximal rectangle than by a negative maximal rectangle.

Idea 3: C[x][y] is simply the sum over x' >= x, y' >= y of (x' - x + 1) *
B[x'][y'], where the factor corresponds to the number of rectangles of
dimension x * h inside a rectangle of dimension x' * h' sharing the same
bottom side.  There are several methods to compute C from B.  We propose a one
pass solution.

"""

from math import sqrt
from pprint import pprint

def readInt():      return int(input())
def readStr():      return input().strip()
def readInts():     return map(int, input().split())


def array2d(a,b, default=None):
    return [[default for _ in range(b)] for _ in range(a)]


X, Y, N, D = readInts()
A = array2d(X, Y, 0)
B = array2d(X + 1, Y + 1, 0)   # indices from 1 to X included, and 1 to Y included
C = array2d(X + 1, Y + 2, 0)   # adding index Y + 1 avoids a test later
# read decoration rectangles
for _ in range(N):
    x1, x2, y1, y2 = readInts()
    for x in range(x1, x2):
        for y in range(y1, y2):
            A[x][y] = 1
# scan each row
H = [0] * (X + 1)              # add a right border
for y in range(Y - 1, -1, -1):
    S = [(-1, 0)]              # dummy pair avoids empty stack case
    for x in range(X + 1):
        # update histogram
        if x == X or A[x][y]:  # include right border
            H[x] = 0
        else:
            H[x] += 1
        # detect maximal rectangles
        x1 = x
        while S[-1][1] > H[x]:
            x1, h = S.pop()
            w = x - x1
            B[w][h] += 1       # positive rectangle
            B[w][max(H[x], S[-1][1])] -= 1      # negative rectangle
        S.append((x1, H[x]))   # new rectangle

# compute C
for h in range(Y, 0, -1):
    E = 0  # suffix sum of row y with weights 1,1,1,...,1
    F = 0  # suffix sum of row y with weights 1,2,3,...,k
    for w in range(X, 0, -1):
        E += B[w][h]
        F += E
        C[w][h] = C[w][h + 1] + F

# answer queries
for _ in range(D):
    w, h = readInts()
    print(C[w][h])
