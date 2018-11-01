#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# c.durr - swerc - 2017

""" Burglary
Dynamic programming in time O(N*M + N*L^4).
(N = #shelfs, M = #slots on each shelf, L = #ladders on each shelf)

There are N*L^2 subproblems each needs O(L^2) time to be solved.

## Notations

Shelfs are numbered from top to bottom 0 to n-1. Ladders below shelf i are
numbered from left to right 0 to size[i]-1. By ladder k we mean the ladder in
column position k.

shelf 0 is the top most shelf and it is empty

shelf[i] is the content of the shelf indexed by the m positions. 0=empty.

left[k] = is the closest index j <= k such that shelf[i][j] > 0 and assuming
shelf[i][k]=0. In other words it is the position of a jar to the left of k
that could be collected by arriving on k, making a return trip to j and
leaving at k or some position to the right.

right[j] = is similar.

ladder[i] is the list of ladder positions below shelf i,
connecting to shelf i + 1 or to the floor in case i = n - 1

B[i][k][l] = maximum score for a walk from shelf 0 to shelf i and back to shelf 0,
reaching shelf i with the k-th ladder (between shelfs i - 1 and i)
and leaving on the l-th ladder.

A[i][k][l] = maximum score for a walk that leaves shelf i on the k-th ladder
to shelf i + 1 and returns through the l-th ladder. The score is restricted to
the jars on shelfs 0 to i collected during the walk.

## Recursion

A[i][k0][l0] is the maximum over the ladder choices k1, l1 between shelf i and i - 1
of A[i - 1][k1][l1] plus the score collected on shelf i, which computes as follows:

Let a--b and c--d be the segments formed by k0--k1 and l0--l1 normalized in
the order a<=b, c<=d and a<=c. Then we have the following cases.

Without loss of generality the walk down is always to the left to the walk up,
in the following sense.  If between two successive shelfs ladder k is used for
the down direction and ladder l for the up direction, then k <= l. As a result
the dynamic programs A, B need to be computed only for indices k <= l.


```
   (1) a------------b
           c--------------d (2)


   (1) a---------b (3)
                         (4) c-----------d (2)
```

First of all in the intersection of the segments no jar is allowed, since it
is forbidden to walk twice over the same jar.  If the intersection is not
empty, the score is -infinity.

Moreover in addition to the total jar values in the union of the segments, the
walk can make tiny excursions to collect the next jar to the left of a, the
next jar to the right of b if any etc.  These additional jar collections are
numbered from 1 to 5 in this figure.  This is the tricky detail of this
problem.  For example in the first depicted case with a < c. If there is a jar
at position a, the walk cannot collect the jar to its left (if any).

Cases 3 and 4 are interesting. The jar to the right of b has to be before c in order
to be collected. The same restriction applies to the jar to the left of c. But
here special care needs to be done to avoid counting the same jar twice in
case there is a single jar between b and c.

"""

def readInt():      return int(input())
def readStr():      return input().strip()
def readInts():     return map(int, input().split())

def array3d(a,b,c, default=None):
    return [[[default for _ in range(c)] for _ in range(b)] for _ in range(a)]


def enum_pair(L):
    """enumerates over pairs (a,b) such that a, b are in L, but a is b or a is before b.
    """
    n = len(L)
    for j in range(n):
        for i in range(j + 1):
            yield (i, j, L[i], L[j])


def window(total, start, end):
    """returns sum(L[start:end+1]) for some list L.
    total[i] is defined as sum(L[:i])
    """
    if start == 0:
        return total[end]
    else:
        return total[end] - total[start - 1]


def solve(shelf, ladder):
    n = len(shelf)
    m = len(shelf[0])
    M = range(m)
    A = [[] for _ in range(n)]
    B = [[] for _ in range(n)]
    for i in range(n):
        L = range(len(ladder[i]))
        A[i] = [[float('-inf') for k in L] for l in L]
        if i < n -1:
            B[i + 1] = [[float('-inf') for k in L] for l in L]

    #                                                --- basis case
    for i, j, k, l in enum_pair(ladder[0]):
        A[0][i][j] = 0    # The top-most shelf is empty
        if n > 1:
            B[1][i][j] = 0

    for i in range(1, n):
        #                                            --- left and right
        left = [-1] * m
        right = [-1] * m
        last = -1
        for j in M:
            if shelf[i][j] > 0:
                last = j
                left[j] = - 1
            else:
                left[j] = last
        last = -1
        for j in range(m - 1, -1, -1):
            if shelf[i][j] > 0:
                last = j
                right[j] = -1
            else:
                right[j] = last
        # prefix sum needed for function window
        tot = 0
        total = []
        for j in M:
            tot += shelf[i][j]
            total.append(tot)
        #                                            --- B
        for ik, il, k, l in enum_pair(ladder[i - 1]):
            score = A[i - 1][ik][il] + window(total, k, l)
            if left[k] != -1:
                score += shelf[i][left[k]]
            if right[l] != -1:
                score += shelf[i][right[l]]
            B[i][ik][il] = score
        #                                            --- A
        for i0, j0, k0, l0 in enum_pair(ladder[i]):
            for i1, j1, k1, l1 in enum_pair(ladder[i - 1]):
                a = min(k0, k1)  # now we have a problem with just 2 segments a--b and c--d
                b = max(k0, k1)               # normalize the segment orientation left to right
                c = min(l0, l1)
                d = max(l0, l1)
                if a > c:                     # normalize the segment order
                    a, b, c, d = c, d, a, b   # exchange segments
                # test for intersections between the segments
                if c > b or window(total, c, b) == 0:  # no jars in the intersection
                    score = A[i - 1][i1][j1] + window(total, a, b) + window(total, c, d)
                    if left[a] != -1:                # (1)
                        score += shelf[i][left[a]]
                    if right[d] != -1:     # (2)
                        score += shelf[i][right[d]]
                    if b < c:
                        if right[b] != -1 and right[b] < c:   # (3)
                            score += shelf[i][right[b]]
                        if left[c] != -1 and left[c] > b and left[c] != right[b]:  # (4)
                            score += shelf[i][left[c]]
                    A[i][i0][j0] = max(A[i][i0][j0], score)
    #      ---- extract best solution
    best = 0
    for i, j, k, l in enum_pair(ladder[n - 1]):         # --- walk until floor
        best = max(best, A[n - 1][i][j])
    for i in range(1, n):                         # --- return at shelf i
        for ik, jl, k, l in enum_pair(ladder[i - 1]):
                best = max(best, B[i][ik][jl])
    return best



N, M = readInts()
shelf = []
ladder = []
for i in range(N):
    #                      -- the shelf
    s = readStr()
    shelf.append([])
    for x in s:
        if x.isdigit():
            shelf[-1].append(int(x))
        else:
            shelf[-1].append(0)
    #                      -- the ladders below
    s = readStr()
    ladder.append([])
    for i, x in enumerate(s):
        if x == "|":
            ladder[-1].append(i)
print(solve(shelf, ladder))

