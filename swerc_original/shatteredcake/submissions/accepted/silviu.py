import sys

W = int(sys.stdin.readline())
N = int(sys.stdin.readline())
A = sum([reduce(lambda x,y:x*y,[int(x) for x in sys.stdin.readline().split()])\
        for _ in range(N)])
print A/W
