import numpy as np
from timeit import default_timer as timer
from numba import vectorize
import threading as th
threads = list()

total_sum = 0

class SplitNormal:
    def split(self, vector):
        global total_sum

        vector_np = np.array(vector, dtype=np.float32)

        for i in range(len(vector_np)):
            total_sum+=vector_np[i]

@vectorize(['float32(float32)'], target='cuda')
def maxSum(number):
    sum = 0

    if (number > 0):    
        sum += number

    return sum

class SplitCuda:
    def split(self, vector):
        global total_sum

        vector_np = np.array(vector, dtype=np.float32)
        sum = maxSum(vector_np)
        
        for i in range(len(vector)):
            total_sum+=sum[i]

def maxSumNormal(vector, len):
    sum = 0
    maxm = max(vector) 

    if (maxm <= 0):
        return maxm
 
    for i in range(len):
        if (vector[i] > 0):    
            sum += vector[i] 
    
    return sum

def main():
    global total_sum
    len_vector = 1000000000
    threads = []
    sum_normal = 0

    vector = np.random.sample(len_vector)
    print('Start vector\n')

    ########################################

    start = timer()
    sum_normal = maxSumNormal(vector, len_vector)
    duration = timer() - start
    
    print('\nSum normal')
    print('Total: {:.0f}'.format(sum_normal))
    print('Duration: {:.7f}'.format(duration))

    ########################################

    number_threads = 2

    print('\nWith', number_threads, 'threads\n')

    start = timer()    
    vector_split = np.array_split(vector, number_threads)

    for index in range(0, number_threads):
        obj = SplitNormal()
        x = th.Thread(target=obj.split(vector_split[index]), args=(vector_split[index]))   
        threads.append(x)                                           
        x.start()

    duration = timer() - start

    print('-\nSplit with threads')
    print('Total: {:.0f}'.format(total_sum))
    print('Duration: {:.7f}'.format(duration))

    total_sum = 0

    ###############################

    start = timer()    
    vector_split = np.array_split(vector, number_threads)

    for index in range(0, number_threads):
        obj = SplitCuda()
        x = th.Thread(target=obj.split(vector_split[index]), args=(vector_split[index]))   
        threads.append(x)                                           
        x.start()

    duration = timer() - start

    print('-\nUsing with threads and GPU')
    print('Total: {:.0f}'.format(total_sum))
    print('Duration: {:.7f}'.format(duration))

    ########################################

    print('\n\n')

if __name__ == '__main__':
    main()