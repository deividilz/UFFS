from random import randint
from time import time
from multiprocessing.pool import ThreadPool
import multiprocessing as mp

start = time() 
threads = []

def algoritmo_linear(a, init, end):
    max_terminando_aqui = max_ate_agora = a[0]
    for x in a[init:end]:
        #print(max_terminando_aqui, x)
        max_terminando_aqui = max(x, max_terminando_aqui + x)
        max_ate_agora = max(max_ate_agora, max_terminando_aqui)
        #print(max_terminando_aqui,max_ate_agora)   
    
    print(max_ate_agora)
    return max_ate_agora

array = [5, -3, 4, 10, 0, -2, 2, -5, -2]
#for i in range(0, 10):
        #arr.append(i)

number_processes = mp.cpu_count()
pool = ThreadPool(processes=number_processes)
    
init = 0
end = int(len(array)/1) 
end_aux = end

print('end: ', end)

for number in range(0, 1):
    async_result = pool.apply_async(algoritmo_linear, (array, init, end))
    threads.append(async_result)

    init = end
    end+=end_aux

    if(end>len(array)):
        end = len(array)
 
    print('init: ', init, 'end: ', end)
    letters_list = [result.get() for result in threads]

    #sum_end = 0
    #for i in range(0, len(vector_sum)):
        #sum_end+=vector_sum[i]

print("Total: ", letters_list, "\n\n")

end = time()
print('tempo de execução da soma: {}'.format(end - start))
