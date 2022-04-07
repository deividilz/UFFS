import time
from multiprocessing.pool import ThreadPool
import multiprocessing as mp
import random 

vector_sum = []

def maxSumNormal(a, n):
    sum = 0
    maxm = max(a) 

    if (maxm <= 0):
        return maxm
 
    for i in range(n):
        if (a[i] > 0):    
            sum += a[i] 
            #time.sleep(1)      

    print("Total: ", sum, "\n")
    return sum

def maxSum(array, init, end):
    sum = 0
    maxm = max(array)
 
    if (maxm <= 0):
        return maxm
 
    while(init<end):
        if (array[init] > 0):     
            sum += array[init]
            #time.sleep(1)
        init+=1
            
    vector_sum.append(sum) 
           
    return sum

if __name__ == '__main__':
    array = []
    threads = []

    for i in range(0, 1000000):
        array.append(random.random())  

    start = time.time()

    maxSumNormal(array, len(array))

    end_time = time.time()
    print('tempo de execução da soma normal: {}'.format(end_time - start))

    start = time.time()
    number_processes = mp.cpu_count()
    print("\n\nNumber processes: ", number_processes)
    number_split = 5

    pool = ThreadPool(processes=number_processes)

    end = 0
    init = 0
    end = int(len(array)/number_split)
    end_aux = end

    for number in range(0, number_split+1):
        async_result = pool.apply_async(maxSum, (array, init, end))
        threads.append(async_result)

        print('init: ', init, 'end: ', end)  

        init = end
        end+=end_aux

        if(end>len(array)):
            end = len(array)
            
    values = [result.get() for result in threads]

    sum_end = 0
    for i in range(0, len(vector_sum)):
        sum_end+=vector_sum[i]

    print("Total: ", sum_end, "\n")

    end_time = time.time()
    print('tempo de execução da soma paralela: {}'.format(end_time - start))