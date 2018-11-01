#!/usr/bin/env python2

N=int(input())
M=int(input())
S=(1<<(N))
modu=1000*1000*1000

def mult(A,B):
   C = [[0]*S for _ in range(S)]
   for i in range(S):
      for j in range(i+1):
         C[i][j] = sum( (A[i][k]*B[k][j]% modu) for k in range(S)) % modu
         C[j][i] = C[i][j]
   return C
         
bin_fibo = [1]*(S*2)
for c in range(2,2*S):
    bin_fibo[c] = bin_fibo[c//2]
    if c & 3 == 0:
        bin_fibo[c] += bin_fibo[c//4]

trans = [ [0]*S for _ in range(S)]
for y in range(S):
    for x in range(S):
        if x&y == 0:
            trans[y][x] = bin_fibo[S+(x|y)]

res = [ [0]*S for _ in range(S) ]
for i in range(S):
    res[i][i]=1
while M:
    if M & 1:
        res = mult(res,trans)
    trans = mult(trans,trans)
    M = M//2
print(res[0][0])
