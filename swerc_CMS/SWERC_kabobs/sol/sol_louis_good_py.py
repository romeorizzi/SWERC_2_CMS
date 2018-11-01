#!/usr/bin/python
import sys
from collections import *
from itertools import *

steps,alphabet = sys.stdin.readline().split()
steps = int(steps)
rules = sys.stdin.readline()

class Auto:
    def __init__(self,n):
        self.nb=n
        self.out=[dict() for _ in range(n)]
        self.defout=[-1 for _ in range(n)]
        self.fin = []     
        
    @classmethod
    def from_rule(cls,rule):
        res = cls(len(rule)-1)
        fail = 0
        dec = 0
        for i in range(res.nb):
            if rule[i]=='>':
                dec=1
                fail=i
            else:
                if fail != i:
                    res.out[i][rule[fail+dec]]=(fail+1)%res.nb
            res.out[i][rule[i+dec]]=(i+1)%res.nb
            res.defout[i]=fail
            if fail == 0:
                res.fin.append(i)
        # print(" ====== FROM "+rule+" ===== ")
        # res.show()
        return res

    @classmethod
    def product(cls,a,b):
        if b is None:
            return a
        res = cls(a.nb*b.nb)
        for sa in range(a.nb):
            for sb in range(b.nb):
                s=sa*b.nb+sb
                res.defout[s] = a.defout[sa]*b.nb+b.defout[sb]    
                for (c,ta) in a.out[sa].items():
                    if c in b.out[sb]:
                        res.out[s][c] = ta*b.nb+b.out[sb][c]
                    else:
                        res.out[s][c] = ta*b.nb+b.defout[sb]
                for (c,tb) in b.out[sb].items():
                    if not (c in a.out[sa]):
                        res.out[s][c] = a.defout[sa]*b.nb+tb
        res.fin = [ sa*b.nb+sb for (sa,sb) in product(a.fin,b.fin)]
        # print(" ====== PRODUCT ===== ")
        # res.show()
        return res

    @classmethod
    def from_ruleset(cls,rules):
        if len(rules) == 1:
            return cls.from_rule(rules[0])
        else:
            return cls.trim(cls.product(cls.from_ruleset(rules[:len(rules)//2]),
                                        cls.from_ruleset(rules[len(rules)//2:])))            

    def show(self):
        print("Auto with "+str(self.nb)+" states")
        for i in range(self.nb):
            for (c,s) in self.out[i].items():
                print( str(i) + "-- "+c+" --> "+str(s) )
            print("def ---> "+str(self.defout[i]))
        print("Finals: "+str(self.fin))

    @classmethod
    def trim(cls,a):
        res = cls(a.nb)
        accessible_states = [0]
        cur = 0
        remap = dict()
        remap[0]=0
        while cur < len(accessible_states):
            for (c,disc) in a.out[accessible_states[cur]].items():
                if disc not in remap:
                    remap[disc] = len(accessible_states)
                    accessible_states.append(disc)
                res.out[cur][c] = remap[disc]
            if a.defout[accessible_states[cur]] not in remap:
                remap[a.defout[accessible_states[cur]]] = len(accessible_states)
                accessible_states.append(a.defout[accessible_states[cur]])
            res.defout[cur] = remap[a.defout[accessible_states[cur]]]
            cur+=1
        res.nb = cur
            
        for i in a.fin:
            if i in remap:
                res.fin.append(remap[i])
        # print(" ====== TRIM ===== ")
        # res.show()
        return res

a = Auto.trim(Auto.from_ruleset(rules.strip().split('|')))

def_tr = [0]*a.nb
count_tr = [0]*a.nb
one_tr = [[] for _ in range(a.nb)]
for s1 in range(a.nb):
    for (c,s2) in a.out[s1].items():
        one_tr[s1].append(s2)
    count_tr[s1] = len(alphabet)-len(a.out[s1])
    def_tr[s1] = a.defout[s1]

cur=[0]*a.nb
for i in a.fin:
    cur[i]=1

for i in range(steps):
    nxt=[0]*a.nb
    for f in range(a.nb):
        nxt[f] = cur[def_tr[f]]*count_tr[f]
        for t in one_tr[f]:
            nxt[f]+=cur[t]
    for i in range(a.nb):
        cur[i] = nxt[i] % 10000000
print (cur[0])
