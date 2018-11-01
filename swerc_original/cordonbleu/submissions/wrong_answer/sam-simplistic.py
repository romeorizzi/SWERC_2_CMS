#

import sys

def read_ints(): return tuple(map(int, sys.stdin.readline().split()))

def distance(a, b): return abs(a[0]-b[0])+abs(a[1]-b[1])

def solve(bottles, couriers, restaurant):
    """For every bottle, starting with the farthest one from the
    restaurant, use the closest courier, then place this courier at
    the restaurant."""
    total = 0
    bottles.sort(key=lambda x: distance(x, restaurant))
    while bottles:
        b = bottles.pop()
        (i, c) = sorted(enumerate(couriers),
                        key=lambda x: distance(x[1], b))[0]
        total += distance(c, b) + distance(b, restaurant)
        couriers[i] = restaurant
    return total

nbottles, ncouriers = read_ints()
bottles = [read_ints() for _ in range(nbottles)]
couriers = [read_ints() for _ in range(ncouriers)]
restaurant = read_ints()
print(solve(bottles, couriers, restaurant))
