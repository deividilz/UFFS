from pickle import GLOBAL
from random import randint
from time import time
import _thread

vector_sum = []
num_thread = 4

def MaxNonEmpSubSeq(array, init, end):
    global num_thread
    sum = 0
    maxm = max(array)
    init_print = init
 
    if (maxm <= 0):
        return maxm
 
    while(init<end):
        if (array[init] > 0):     
            sum += array[init]

        init+=1

    print('sum: ', sum, 'init: ', init_print, 'end: ', end)             
    vector_sum.append(sum)   
    num_thread-=1     
    #return sum
 
if __name__ == '__main__':
    array = []
    threads = []

    start = time()  

    #for i in range(0, 100000000):
        #arr.append(random.random())

    for i in range(0, 10000):
        array.append(i)

    number_split = 2

    init = 0
    end = int(len(array)/number_split)
    end_aux = end

    for number in range(0, number_split):
        _thread.start_new_thread(MaxNonEmpSubSeq, (array, init, end))

        init = end
        end+=end_aux

        if(end>len(array)):
            end = len(array)
   
while num_thread > 0:
    pass
      
print("Total: ", vector_sum, "\n\n")

end = time()
print('tempo de execução da soma: {}'.format(end - start))