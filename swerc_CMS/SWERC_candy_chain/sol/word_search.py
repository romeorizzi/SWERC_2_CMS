#!/usr/bin/env pypy
# -*- coding: utf-8 -*-
# c.durr - swerc - 2017

""" Candy Stick ? (word_search)

dynamic programming in time O(N^3 L) where N is the length of the candy stick
and L is the total length over all words in the dictionary.

= Definitions =

A[i] = maximal score one can obtain from stick[i:] without necessarily
consuming all characters

B[i,j] = maximal score one can obtain by consuming all characters from stick[i:j]

C[i,j,k,p] = maximal score one can obtain by consuming all characters from
             stick[i:j] with a last extraction of the suffix word[k][p:]


= Recursions =

Maximization over the empty set returns -infinity.

== A ==

A[N] = 0 # since stick[N:] is the empty word

otherwise for i < N

A[i] = max { B[i,N],                # consume all characters
             max B[i,j] + A[j+1] }  # don't consume character j
             over i <= j < N
    # where j is the first letter that is not consumed

       B[i,j]             A[j + 1]
|-----------------|--|-------------------------|
 i                 j  j+1                        N

== B ==

B[i,i] = 0  # since stick[i:i] is the empty word

otherwise for i<j

B[i,j] = max{0, max B[i,r] + C[r+1,j,k,1] }
over 0 <= k < K, over i <= r <= j - len(word[k]) with stick[r] == word[k][0]
# k is the last word to be extracted, r the matching position of its first letter

                    0     1 2      ...
        taste[k]: |--|   |---|   |----|
       B[i,r]      ==     C[r+1,j,k,1]
|-----------------|--|-------------------------|
 i                 r  r+1                        j


== C ==

C[i,j,k,p] = score[k] + B[i,j] if p == len(word[k])

otherwise for p < len(word[k])

C[i,j,k,p] = max B[i,r] + C[r+1,j,k,p+1]
over i <= r <= j-len(word[k]) + p with stick[r] == word[k][p]


                    p     p+1      ...
        taste[k]: |--|   |---|   |----|
       B[i,r]      ==     C[r+1,j,k,p+1]
|-----------------|--|-------------------------|
 i                 r  r+1                        j


= Complexity =

For B there are O(N^2) variables which update in time O(N K)

For C there are O(N^2L) variables which update in time O(N)


"""
# Compatibility with python2 (for pypy)
from __future__ import absolute_import, division, print_function

import collections
import sys

# Compatibility with python2 (for pypy) and 3.
input_fun = input if sys.version_info >= (3, 0) else raw_input
if sys.version_info >= (3, 0): xrange = range

# dynamic program with memoization

memB = {}
memC = {}

def B(i,j):
    if (i,j) not in memB:
        if i == j:
            memB[i,j] = 0
        else:
            memB[i,j] = float('-inf')
            for k in xrange(len(taste)):
                for r in xrange(i, j - len(taste[k]) + 1):
                    if stick[r] == taste[k][0]:
                        memB[i,j] = max(memB[i,j], B(i,r) + C(r + 1, j, k, 1))
    return memB[i,j]

def C(i, j, k, p):
    if (i,j,k,p) not in memC:
        if p == len(taste[k]):
            memC[i,j,k,p] = price[k] + B(i,j)
        else:
            memC[i,j,k,p] = float('-inf')
            for r in xrange(i, j - len(taste[k]) + p + 1):
                if stick[r] == taste[k][p]:
                    memC[i,j,k,p] = max(memC[i,j,k,p], B(i,r) + C(r + 1, j, k, p + 1))
    return memC[i,j,k,p]




# -- read instance

stick = input_fun().strip()
N = len(stick)
K = int(input_fun())
taste_to_price = collections.defaultdict(int)
taste = []
price = []
for _ in range(K):
    tab = input_fun().split()
    taste_to_price[tab[0]] = max(taste_to_price[tab[0]], int(tab[1]))
    r = tab[0][::-1]  # Reverse
    taste_to_price[r] = max(taste_to_price[r], int(tab[1]))

for t, p in taste_to_price.items():
    taste.append(t)
    price.append(p)

# -- solve dynamic program

A = [0] * (N+1)
A[N] = 0
for i in range(N-1, -1, -1):
    A[i] = B(i, N)
    for j in range(i, N):
        A[i] = max(A[i], B(i, j) + A[j + 1])

# -- print solution
# print("A=%s" % A)
# for key in memB:
#     val = memB[key]
#     if val >= 0:
#         print("B[%s]\t=%i" % (key, val))
# for key in memC:
#     val = memC[key]
#     if val >= 0:
#         print("C[%s]\t=%i" % (key, val))
print(A[0])
