#! /usr/bin/python3
#
# Usage: ./generate.py N L
# where N is the number of test cases and L
# the maximum number of bottles and couriers
#
# The files will be named sequentially 000.in, 001.in, etc.

import random
import sys

# Range for coordinates will be in [-RADIUS, RADIUS]
RADIUS = 1000

def distance(a, b):
    """Distance between points."""
    return abs(a[0]-b[0])+abs(a[1]-b[1])

def random_pos():
    """Return a random positin."""
    return (random.randint(-RADIUS, RADIUS), random.randint(-RADIUS, RADIUS))

def random_pos_until(f):
    """Return a random position which satisfies predicate f."""
    while True:
        pos = random_pos()
        if f(pos):
            return pos

def regular_case(l):
    """Generate a test with max l bottles or couriers."""
    bottles = [random_pos() for _ in range(random.randint(1, l))]
    couriers = [random_pos() for _ in range(random.randint(1, l))]
    return (bottles, couriers, random_pos())

def special_cases(l):
    """Return a list of some edge or pathological cases that we want
    to include into our problems list."""
    special_cases = []
    # One bottle positionned at the lab with a courier at the lab
    # as well.
    lab = random_pos()
    special_cases.append(([lab], [lab], lab))
    # One bottle positionned at the lab with a courier somewhere
    # else.
    lab = random_pos()
    courier = random_pos_until(lambda p: p != lab)
    special_cases.append(([lab], [courier], lab))
    # One courier positionned at the lab with a bottle somewhere
    # else.
    lab = random_pos()
    bottle = random_pos_until(lambda p: p != lab)
    special_cases.append(([bottle], [lab], lab))
    # Three bottles at the same place, one courier.
    lab = random_pos()
    bottle = random_pos_until(lambda p: p != lab)
    courier = random_pos_until(lambda p: p != lab and p != bottle)
    special_cases.append(([bottle, bottle, bottle], [courier], lab))
    # Three couriers at the same place, one bottle.
    lab = random_pos()
    bottle = random_pos_until(lambda p: p != lab)
    courier = random_pos_until(lambda p: p != lab and p != bottle)
    special_cases.append(([bottle], [courier, courier, courier], lab))
    # Three couriers, three bottles, all bottles closer to lab
    # than to couriers.
    lab = random_pos()
    bottles = [random_pos_until(lambda p: distance(p, lab) < RADIUS/4) for _ in range(3)]
    couriers = [random_pos_until(lambda p: distance(p, lab) > 3*RADIUS/4) for _ in range(3)]
    special_cases.append((bottles, couriers, lab))
    # Three couriers, three bottles, all couriers closer to lab
    # than to bottles.
    lab = random_pos()
    bottles = [random_pos_until(lambda p: distance(p, lab) > 3*RADIUS/4) for _ in range(3)]
    couriers = [random_pos_until(lambda p: distance(p, lab) < RADIUS/4) for _ in range(3)]
    special_cases.append((bottles, couriers, lab))
    # Combinations of 1/l couriers and 1/l bottles.
    for nbottles in [1, l]:
        for ncouriers in [1, l]:
            lab = random_pos()
            bottles = [random_pos() for _ in range(nbottles)]
            couriers = [random_pos() for _ in range(ncouriers)]
            special_cases.append((bottles, couriers, lab))
    # Lab in one corder along with a courier, l bottles in the other corner.
    special_cases.append(([(RADIUS, RADIUS)]*l, [(-RADIUS, -RADIUS)], (-RADIUS, -RADIUS)))
    # Return the collection of special cases
    return special_cases

if __name__ == '__main__':
    n = int(sys.argv[1])
    l = int(sys.argv[2])
    problems = special_cases(l)
    for i in range(n-len(problems)):
        problems.append(regular_case(l))
    for (i, (bottles, couriers, lab)) in enumerate(problems):
        with open("{:03d}.in".format(i), "w") as fd:
            fd.write("{} {}\n".format(len(bottles), len(couriers)))
            for x in bottles + couriers + [lab]:
                fd.write("{} {}\n".format(x[0], x[1]))
