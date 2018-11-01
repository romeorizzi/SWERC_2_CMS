#

import sys

def read_ints(): return tuple(map(int, sys.stdin.readline().split()))

def distance(a, b): return abs(a[0]-b[0])+abs(a[1]-b[1])

def solve(bottles, couriers, restaurant, current_best = 1000000000):
    """For every bottle permutation, try every courier and
    record the best solution. Of course, we use a threshold
    to not even consider solutions worse than the current
    best."""
    best = current_best if bottles else 0
    for ib in range(len(bottles)):
        b = bottles.pop(ib)
        for ic in range(len(couriers)):
            c = couriers[ic]
            cost = distance(c, b) + distance(b, restaurant)
            if cost > best:
                continue
            couriers[ic] = restaurant
            cost += solve(bottles, couriers, restaurant, best - cost)
            couriers[ic] = c
            if cost < best:
                best = cost
        bottles.insert(ib, b)
    return best

nbottles, ncouriers = read_ints()
bottles = [read_ints() for _ in range(nbottles)]
couriers = [read_ints() for _ in range(ncouriers)]
restaurant = read_ints()
print(solve(bottles, couriers, restaurant))
