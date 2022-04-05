from random import randint
from time import time
from multiprocessing.pool import ThreadPool

vector_sum = []

def MaxNonEmpSubSeq(array, init, end):
    sum = 0
    maxm = max(array)
    init_print = init
 
    if (maxm <= 0):
        return maxm
 
    for i in range(init, end):
        if (array[init] > 0):     
            sum += array[init]

        init+=1

    print('sum: ', sum, 'init: ', init_print, 'end: ', end)             
    vector_sum.append(sum)        
    return sum
 
# Driver Code
if __name__ == '__main__':
    array = []
    threads = []

    start = time()  

    #for i in range(0, 100000000):
        #arr.append(random.random())

    for i in range(0, 100000000):
        array.append(i)

    number_processes = 4
    number_split = 4

    pool = ThreadPool(processes=number_processes)

    init = 0
    end = int(len(array)/number_split) # 10000 / 4 = 2500
    end_aux = end

    for number in range(0, number_split):
        async_result = pool.map_async(MaxNonEmpSubSeq, (array, init, end))
        threads.append(async_result)

        init = end
        end+=end_aux

        if(end>len(array)):
            end = len(array)
 
    letters_list = [result.get() for result in threads]

    #sum_end = 0
    #for i in range(0, len(vector_sum)):
        #sum_end+=vector_sum[i]

    print("Total: ", letters_list, "\n\n")

    end = time()
    print('tempo de execução da soma: {}'.format(end - start))