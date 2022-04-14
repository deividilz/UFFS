import numpy as np
from timeit import default_timer as timer

def pow(a, n):
    sum = 0

    for i in range(n):
        if (a[i] > 0):    
            sum += a[i]

    return sum

def main():
    vec_size = 10000000

    a = np.random.sample(vec_size)
    c = 0

    start = timer()
    c = pow(a, vec_size)
    duration = timer() - start

    print(duration)

    
    print("\nResultado: ", c)

if __name__ == '__main__':
    main()