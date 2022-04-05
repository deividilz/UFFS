from random import randint
from time import sleep, time
from multiprocessing.pool import ThreadPool


def print_names(name):
    sleep(randint(1, 3))
    print('Meu nome é: {}'.format(name))


runtime = []
threads = []
names = ['Adson', 'Gabriel', 'Siqueira', 'Ronaldo', 'Gleilson',
         'Emerson', 'Joselito', 'Piloto', 'Kleber', 'Mauricio']

pool = ThreadPool(processes=8)
start = time()

for name in names:
    async_result = pool.map_async(print_names, (name,))
    threads.append(async_result)

letters_list = [result.get() for result in threads]

end = time()
print('tempo de execução da tradução: {}'.format(end - start))