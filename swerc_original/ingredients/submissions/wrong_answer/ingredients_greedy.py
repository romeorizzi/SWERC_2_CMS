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

#compute the 0-1 knapsack solution using the greedy algorithm
def compute_greedy(B):
    opt.append([])
    curB = B
    prest_per_costs = []
    reserved = []
    for i in range(len(dishes)):
        reserved.append(False)
        if costs[i]!=0 or prestiges[i]!=0:
            ppc = 0.
            if costs[i]>0: ppc = float(prestiges[i])/float(costs[i])
            prest_per_costs.append((i,ppc))
    #sorting the list
    prest_per_costs.sort(reverse=True, cmp=lambda x,y:cmp(x[1],y[1]))
    sol = []
    all_taken = False
    found = True
    while all_taken==False and found and curB>0:
        all_taken = True
        found = False
        for cdish in prest_per_costs:
            if costs[cdish[0]]<=curB and not reserved[cdish[0]]:
                sol.append(cdish[0])
                curB = curB-costs[cdish[0]]
                found=True
                reserved[cdish[0]]=True
            elif not reserved[cdish[0]]:
                all_taken=False
    sol.sort()
    print sum([prestiges[x] for x in sol])
    print sum([costs[x] for x in sol])
    #no more solution printing :(
    #print len(sol)
    #for x in sol: print dishes[x]
            
if __name__=='__main__':
    time_debug_arg = ''
    start = 0
    end = 0
    if len(sys.argv)==2: time_debug_arg=sys.argv[1]
    
    if time_debug_arg=='-t': start = time.time()
    B = read_and_prepare_input(sys.stdin)
    for odish in originals: compute_shortest_path_tree(odish) 
    compute_greedy(B)
    if time_debug_arg=='-t':
        end = time.time()
        print >> sys.stderr, end-start
