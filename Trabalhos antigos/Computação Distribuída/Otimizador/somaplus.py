# Python3 program to implement
# the above approach
import time
import random
start_time = time.time()
import threading as th
threads = list()            #lista de threads para criar os timers

sum_list = []
# Function to print the maximum
# non-emepty subsequence sum
def MaxNonEmpSubSeq(a, pass_init, passage):
    sum = 0

    pass_init = int(pass_init)

    # Stores the maximum non-emepty
    # subsequence sum in an array
    #sum = 0
 
    # Stores the largest element
    # in the array
    maxm = max(a)
 
    if (maxm <= 0):
        return maxm
 
    # Traverse the array
    while (pass_init<=passage):
    #for i in range(passage):
        
        if (pass_init+1>len(a)): break

        # If a[i] is greater than 0
        if (a[pass_init] > 0):             
            # Update sum
            sum += a[pass_init]

        pass_init +=1
    
    sum_list.append(sum)
        
    
    #print("Soma: ", sum)
    #print(pass_init, passage)
    return sum
 
# Driver Code
if __name__ == '__main__':
    arr = []
    x = []

    amout_threads = 2
    max_value = 100000000

    for i in range(0, max_value):
        arr.append(i)

    #for i in range(0, max_value):
       # arr.append(random.random())

    N = len(arr)

    div = int(max_value/amout_threads)       #1º vez - vai de 0 a 5000
                                        #2º vez - vai de 5001 a 10000


                                    #10000/3 = 3333 - 0 a 3333 - 3334 a 6666 - 6667 a 10000
                                    #3333 - 3333*3 = 
    passage = div
    pass_init = 0

    for i in range(0, amout_threads):

        x = th.Thread(target=MaxNonEmpSubSeq, args=(arr, pass_init, passage))   
        threads.append(x)                                           
        x.start()
       # x.join()
        
        print("pass_init: ", pass_init, 'passage: ', passage)

        pass_init += div+1
        passage += pass_init

        if(passage>len(arr)):
            passage = len(arr)

    print(sum_list)
    
    sum_var = 0
    for i in range(0, len(sum_list)):
        sum_var += sum_list[i]
    #print(MaxNonEmpSubSeq(arr, N))

    print("Soma certa: ", sum_var)
    print("--- %.10f seconds ---" % (time.time() - start_time))

# This code is contributed by mohit kumar 29