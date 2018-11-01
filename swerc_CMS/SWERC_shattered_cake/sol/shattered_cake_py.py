#!/usr/bin/env python

import sys

R = [int(x) for x in sys.stdin.read().split()]
A = 0
for i in range(2, len(R), 2):
    A += R[i] * R[i+1]
print(int(A/R[0]))
