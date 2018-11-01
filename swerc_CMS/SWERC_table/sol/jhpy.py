#!/usr/bin/python

import sys

def readintline():
    return [int(i) for i in sys.stdin.readline().split()]

(X, Y, N, D) = readintline()
table = [[False for _ in range(Y+1)] for _ in range(X)]
counts = [[0 for _ in range(Y+1)] for _ in range(X+1)]

for i in range(N):
    (x1, x2, y1, y2) = readintline()
    for x in range(x1, x2):
        for y in range(y1, y2):
            table[x][y] = True

for x in range(X):
    table[x][Y] = True

hist = [0 for _ in range(Y+1)]
for x in range(X):
    stk = [(0, -1)]
    for y in range(Y+1):
        hist[y] = 0 if table[x][y] else hist[y]+1
        y0 = y
        while stk[-1][0] > hist[y]:
            y0 = stk[-1][1]
            counts[stk[-1][0]][y - y0] += 1
            stk.pop()
            counts[max(stk[-1][0], hist[y])][y - y0] -= 1
        stk.append((hist[y], y0))

for x in range(X+1):
    for y in range(Y-1, 0, -1):
        counts[x][y] += counts[x][y+1]
    for y in range(Y-1, 0, -1):
        counts[x][y] += counts[x][y+1]

for x in range(X-1, 0, -1):
    for y in range(Y+1):
        counts[x][y] += counts[x+1][y]

for i in range(D):
    (x, y) = readintline()
    print(counts[x][y])
