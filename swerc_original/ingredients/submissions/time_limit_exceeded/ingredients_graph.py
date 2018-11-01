import sys
import time

#inputs
B = 0
N = 0

#data structures
costs = []
prestiges = []
dishes = []
did = {}
recipes_from = {}
originals = set()

#optimal matrix
opt = []

#computing a shortest-path tree starting from a original dish
def compute_shortest_path_tree(sdish):
    to_visit = []
    to_visit.append(sdish)
    while len(to_visit)>0:
        cdish = to_visit.pop()
        if cdish in recipes_from:
            for recipe in recipes_from[cdish]:
                new_cost = costs[cdish]+recipe[2]
                if new_cost<costs[recipe[0]]:
                    costs[recipe[0]] = new_cost
                    prestiges[recipe[0]] = prestiges[cdish]+recipe[3]
                    if recipe[0] not in to_visit:
                        to_visit.append(recipe[0])
                elif new_cost==costs[recipe[0]]:
                    new_prest = prestiges[cdish]+recipe[3]
                    if new_prest>prestiges[recipe[0]]:
                        prestiges[recipe[0]] = new_prest
                    if recipe[0] not in to_visit:
                        to_visit.append(recipe[0])

#reading the input and populating the data structures
def read_and_prepare_input(f):
    B = int(f.readline())
    N = int(f.readline())
    idx = 0
    for i in range(N):
        ingr = f.readline().strip().split()
        dish_to = ingr[0]
        dish_from = ingr[1]
        cost = int(ingr[3])
        prestige = int(ingr[4])
        #creating the id (so as not to deal with strings all over)
        if dish_from not in did:
            #dish appears for the first time
            did[dish_from] = idx
            originals.add(idx)
            costs.append(0)
            prestiges.append(0)
            dishes.append(dish_from)
            idx += 1
        if dish_to not in did:
            #dish appears for the first time
            did[dish_to] = idx
            costs.append(sys.maxint)
            prestiges.append(0)
            dishes.append(dish_to)
            idx += 1
        else:
            di = did[dish_to]
            costs[di] = sys.maxint
            if di in originals: originals.remove(di)
        di = did[dish_from]
        if di not in recipes_from:
            recipes_from[di]=[]
        recipes_from[di].append((did[dish_to],di,cost,prestige))
    f.close()
    return B

#compute the 0-1 knapsack solution using dynamic programming
def compute_optimal(B):
    opt.append([])
    for j in range(B+1):
        opt[0].append(0)
    for i in range(1,len(dishes)+1):
        opt.append([])
        for j in range(B+1):
            if costs[i-1]>j:
                opt[i].append(opt[i-1][j])
            else:
                opt[i].append(max(opt[i-1][j],opt[i-1][j-costs[i-1]]+prestiges[i-1]))
    print opt[len(dishes)][B]

def reconstruct_solution(B):
    #we reconstruct for minimal cost
    nitems = len(dishes)
    optP = opt[nitems][B]
    curB = B
    optimal = True
    while optimal and curB>0:
        if opt[nitems][curB]!=optP:
            optimal=False
            curB = curB+1
        else:
            curB = curB-1
    print curB
    '''
    solution = []
    curB = B
    curN = nitems
    while curN>0:
        if opt[curN][curB]>opt[curN-1][curB]:
            solution.append(curN-1)
            curN = curN-1
            curB = curB-costs[curN]
        else:
            curN = curN-1
    print sum([costs[x] for x in solution])
    #print len(solution)
    #for sol in reversed(solution):
    #    print dishes[sol]
    '''

if __name__=='__main__':
    time_debug_arg = ''
    start = 0
    end = 0

    if len(sys.argv)==2: time_debug_arg=sys.argv[1]
    
    if time_debug_arg=='-t': start = time.time()
    B = read_and_prepare_input(sys.stdin)
    for odish in originals: compute_shortest_path_tree(odish) 
    compute_optimal(B)
    reconstruct_solution(B)
    if time_debug_arg=='-t':
        end = time.time()
        print >> sys.stderr, end-start
