import sys
import random
import string

pieces = [[(1,1)],[(1,2),(1,3),(2,3),(1,1),(1,1),(2,1),(2,5)]]
wl = [[(1,1)],[(5,5)]]
npiec = [1,7]

# pypy test_generator.py <method> <width> <length>

if __name__=='__main__':
    m = int(sys.argv[1])
    W = int(sys.argv[2])
    L = int(sys.argv[3])

    print W
    w_pieces=W/wl[m][0][0]
    l_pieces=L/wl[m][0][1]
    N = w_pieces*l_pieces
    print N*npiec[m]
    
    final_pieces = []
    for _ in range(N):
        for p in pieces[m]:
            print '%d %d'%(p[0],p[1])
    '''
            final_pieces.append(p)
    random.shuffle(final_pieces)

    for piece in final_pieces:
        print '%d %d'%(piece[0],piece[1])
    '''
