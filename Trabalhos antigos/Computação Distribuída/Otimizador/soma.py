import time

def maxSum(a, n):
    sum = 0
    maxm = max(a) 

    if (maxm <= 0):
        return maxm
 
    for i in range(n):
        if (a[i] > 0):    
            sum += a[i] 
            time.sleep(1)      

    return sum

if __name__ == '__main__':
    arr = []

    start = time.time()  

    for i in range(0, 100):
        arr.append(i)

    N = len(arr)
 
    print('Total: ', maxSum(arr, N))

    end = time.time()
    print('tempo de execução da soma: {}'.format(end - start))