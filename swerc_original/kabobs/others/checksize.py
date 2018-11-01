def maxi(r):
    t = 61/r - 2
    tr = t*r
    state2 = t**r
    if t>=3 and t<4:
        nb3= r-(61-5*r)
        tr = (r-nb3)*4+nb3
    if t<3:
        nb3 = 61-4*r
        tr = nb3
        state2 = 2**(r-nb3)*3**(nb3)
        
    state1 = 2**r*(tr)
    
    #    print(t,r,tr, min(state1,t**r)*((2-2/t)*r) / 10**6)
    print(t,r,tr, min(state1*(1+2*r),state2*(1+(2-2/t)*r)) / 10**6)

for i in range(1,16):
    maxi(i)
