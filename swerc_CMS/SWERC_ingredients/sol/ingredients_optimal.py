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
topo = []
deg = {}

#optimal matrix
opt = []

#topological sort using Kahn's degrees algorithm
def topological_sort(original_dishes):
    q = []
    topo = []
    for d in original_dishes: q.append(d)
    while len(q)>0:
        d = q.pop()
        topo.append(d)
        if d in recipes_from:
            for r in recipes_from[d]:
                old_deg = deg[r[0]]
                deg[r[0]] = old_deg-1
                assert deg[r[0]]>=0
                if deg[r[0]]==0: q.append(r[0])
    assert len(topo)==len(dishes)
    return topo

#computing costs by going through the topological sort
def compute_costs(topo_sort):
    for cdish in topo_sort:
        if cdish in recipes_from:
            for recipe in recipes_from[cdish]:
                new_cost = costs[cdish]+recipe[2]
                if new_cost<costs[recipe[0]]:
                    costs[recipe[0]] = new_cost
                    prestiges[recipe[0]] = prestiges[cdish]+recipe[3]
                elif new_cost==costs[recipe[0]]:
                    new_prest = prestiges[cdish]+recipe[3]
                    if new_prest>prestiges[recipe[0]]:
                        prestiges[recipe[0]] = new_prest

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
            deg[idx] = 0
            originals.add(idx)
            costs.append(0)
            prestiges.append(0)
            dishes.append(dish_from)
            idx += 1
        if dish_to not in did:
            #dish appears for the first time
            did[dish_to] = idx
            deg[idx] = 1
            costs.append(sys.maxint)
            prestiges.append(0)
            dishes.append(dish_to)
            idx += 1
        else:
            #dish does not appear for the first time
            di = did[dish_to]
            deg[di] = deg[di]+1
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

if __name__=='__main__':
    time_debug_arg = ''
    start = 0
    end = 0
    if len(sys.argv)==2: time_debug_arg=sys.argv[1]    
    if time_debug_arg=='-t': start = time.time()
    B = read_and_prepare_input(sys.stdin)
    compute_costs(topological_sort(originals))
    compute_optimal(B)
    reconstruct_solution(B)
    if time_debug_arg=='-t':
        end = time.time()
        print >> sys.stderr, end-start
