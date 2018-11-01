#!/usr/bin/env python3


from random import randint
from sys import argv

N = int(argv[1])
R = int(argv[2])
points = set()
print("%i %i" % (N, R))
R //= 2
while len(points) < N:
    p = (randint(-R, R), randint(-R, R))
    if p not in points:
        points.add(p)
        print("%i %i" % p)

