import time
from multiprocessing.pool import ThreadPool
import multiprocessing as mp

vector_sum = []

def MaxNonEmpSubSeq(array, init, end):
    sum = 0
    maxm = max(array)
 
    if (maxm <= 0):
        return maxm
 
    while(init<end):
        if (array[init] > 0):     
            sum += array[init]
            time.sleep(1)
        init+=1
            
    vector_sum.append(sum) 
           
    return sum

if __name__ == '__main__':
    array = []
    threads = []

    start = time.time()  

    for i in range(0, 100):
        array.append(i)

    number_processes = mp.cpu_count()
    print("Number processes: ", number_processes)
    number_split = 100

    pool = ThreadPool(processes=number_processes)

    init = 0
    end = int(len(array)/number_split)
    end_aux = end

    for number in range(0, number_split+1):
        async_result = pool.apply_async(MaxNonEmpSubSeq, (array, init, end))
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

    print("Total: ", sum_end, "\n\n")

    end = time.time()
    print('tempo de execução da soma: {}'.format(end - start))