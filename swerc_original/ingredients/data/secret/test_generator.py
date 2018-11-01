import sys
import random
import string

ssize = 20
dishes = []
originals = []

# pypy test_generator.py <number_dishes> <number_recipes> <budget> <range_cost>
#                        <range_prestige>

if __name__=='__main__':
    D = int(sys.argv[1])
    N = int(sys.argv[2])
    B = int(sys.argv[3])
    rC = int(sys.argv[4])
    rP = int(sys.argv[5])

    print B
    print N

    #first step, generate some random strings for dishes
    for _ in range(D):
        dname=''.join(random.choice(string.ascii_uppercase+string.ascii_lowercase)\
                for _ in range(ssize))
        dishes.append(dname)

    #second step, generate some random "links"
    for _ in range(N):
        dish_from_pos = random.randint(0,D-2)
        dish_to_pos = random.randint(dish_from_pos+1,D-1)
        cost = int(random.uniform(1,rC))
        prestige = int(random.uniform(1,rP))
        print '%s %s ingredient %d %d'\
                %(dishes[dish_to_pos],dishes[dish_from_pos],cost,prestige)
