# By Sam, O(n)

import sys

def read_ints(): return [int(x) for x in sys.stdin.readline().split()]

def solve():
    sys.stdin.readline() # n
    A = read_ints()
    B = read_ints()
    colors = [0, 0, 0]
    for (a, aa) in enumerate(A):
        for (b, bb) in enumerate(B):
            colors[(a+b+2)%3] += aa*bb
    print("{} {} {}".format(*colors))

if __name__ == '__main__':
    solve()
