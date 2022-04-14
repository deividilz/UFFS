import numpy as np
from timeit import default_timer as timer
from numba import vectorize

@vectorize(['float32(float32, int32)'], target='cuda')
def pow(a, n):
    sum = 0
    if (a > 0):    
        sum += a

    return sum

def main():
    vec_size = 10000000

    a = np.array(np.random.sample(vec_size), dtype=np.float32)
    c = 0

    start = timer()
    c += pow(a, vec_size)

    sum = 0
    for i in range(vec_size):
        sum += c[i]

    duration = timer() - start

    print(duration)

    print("\nResultado: ", sum)

if __name__ == '__main__':
    main()