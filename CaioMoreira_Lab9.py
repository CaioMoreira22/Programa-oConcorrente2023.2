#Programação Concorrente
#Nome: Caio Moraes Santos Moreira
#DRE: 121050809
#LAB 9 - Codigo

from multiprocessing.pool import Pool
import time
import math

def task(x):
    if(x<=1):
        return 0
    if(x==2):
        return 1
    if(x%2==0):
        return 0
    for i in range(3,int(math.sqrt(x))+1,2):
        if(x%i==0):
            return 0
    return 1

if __name__ == '__main__':
    n = 10000000 #até onde vai o intervalo desejado
    pool = Pool() 

    numbers = list(range(n))
    results = pool.map(task, numbers)
    soma = 0
    for i in results:
        soma = soma + numbers[i] 
    print("O total de primos no intervalo é ",soma)
