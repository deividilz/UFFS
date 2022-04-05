# Python3 program to implement
# the above approach
import time
import random
import threading as th

# Function to print the maximum
# non-emepty subsequence sum
def MaxNonEmpSubSeq(a, n):
     
    # Stores the maximum non-emepty
    # subsequence sum in an array
    sum = 0
 
    # Stores the largest element
    # in the array
    maxm = max(a)
 
    if (maxm <= 0):
        return maxm
 
    # Traverse the array
    for i in range(n):
         
        # If a[i] is greater than 0
        if (a[i] > 0):             
            # Update sum
            sum += a[i]       
    return sum
 
# Driver Code
if __name__ == '__main__':
    arr = []
    start_time = time.time()

    #for i in range(0, 100000000):
        #arr.append(random.random())

    for i in range(0, 100000000):
        arr.append(i)

    N = len(arr)
 
    print(MaxNonEmpSubSeq(arr, N))

    print("--- %.10f seconds ---" % (time.time() - start_time))
 
# This code is contributed by mohit kumar 29