from random import randint
from time import time
from multiprocessing.pool import ThreadPool
import multiprocessing as mp

start = time() 
threads = []
max_list = []

def algoritmo_linear(a, init, end):
    max_terminando_aqui = max_ate_agora = a[init]
    for x in a[init+1:end]:
        #print(max_terminando_aqui, x)
        max_terminando_aqui = max(x, max_terminando_aqui + x)
        max_ate_agora = max(max_ate_agora, max_terminando_aqui)
        print(max_terminando_aqui,max_ate_agora)   
    
    max_list.append(max_ate_agora)
    #print(max_ate_agora)
    #return max_ate_agora

array = [2, -4, 1, 6, -1, -4, 8, -5, -2]
#for i in range(0, 10):
        #arr.append(i)

number_processes = mp.cpu_count()
pool = ThreadPool(processes=number_processes)
    
split = 2

init = 0
end = int(len(array)/split) 
end_aux = end


print('end: ', end)

for number in range(0, split+1):
    async_result = pool.apply_async(algoritmo_linear, (array, init, end))
    threads.append(async_result)
    
    print('init: ', init, 'end: ', end)

    init = end
    end+=end_aux

    if(end>len(array)):
        end = len(array)
 
    letters_list = [result.get() for result in threads]

print("Total1: ", max_list, "\n\n")
bkp = max_list
max_list = []
algoritmo_linear(bkp, 0, len(bkp))

print("Total2: ", bkp, "\n\n")


end = time()
print('tempo de execução da soma: {}'.format(end - start))
