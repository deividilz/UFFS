/**  UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
    ALUNO: DEIVIDI LUIZ
    SISTEMAS OPERACIONAIS

    TRABALHO: encontrar uma forma de consertar o deadlock causado pelas threads 

    Como compilar: gcc -o test deadlock_trylockv3.c -lpthread*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* thread1(void *data);
void* thread2(void *data);

int th1 = 0;        //VARIÁVEL GLOBAL PARA CONTROLAR SE A THREAD1 ESTÁ OK
int th2 = 0;        //VARIÁVEL GLOBAL PARA CONTROLAR SE A THREAD2 ESTÁ OK

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER, mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_t Thread1, Thread2;

int main(void) {       
    pthread_create(&Thread1, NULL, thread1, NULL);      
    pthread_create(&Thread2, NULL, thread2, NULL);
   
    for(;;){                    //EXECUTA PRA SEMPRE OU ATÉ ENCONTRAR O BREAK
        pthread_join(Thread1, NULL);
        pthread_join(Thread2, NULL);

        if(th1 == 0)            //SE A THREAD1 AINDA NÃO ESTIVER OK
            thread1(&Thread1);  //EXECUTA A FUNÇÃO NOVAMENTE
        
        if(th2 == 0)            //SE A THREAD2 AINDA NÃO ESTIVER OK
            thread2(&Thread2);  //EXECUTA A FUNÇÃO NOVAMENTE
        
        if(th1 == 1 && th2 == 1)//SE AMBAS THREADS ESTIVEREM SATISFEITAS
            break;              //INTERROMPE AS REPETIÇÕES
    }

    //now join them    
    printf("Thread id %ld returned\n", Thread1);
    printf("Thread id %ld returned\n", Thread2);

    pthread_exit(NULL);

    return 1;
}

void* thread1(void *data){
    unsigned long i,j;
    //FUNÇÃO MODIFICADA                                   
    if(pthread_mutex_trylock(&mutex1)==0){          //COMPARA PARA SABER SE O MUTEX1 ESTÁ SENDO USADO
         printf("Thread ID%ld got mutex1.\n", pthread_self());

        for(i=0; i< rand()%100000*1000; ++i);    //PERDE UM TEMPO RANDON

        if(pthread_mutex_trylock(&mutex2)==0){      //COMPARA PARA SABER SE O MUTEX2 ESTÁ SENDO USADO
            printf("Thread ID%ld got mutex2.\n", pthread_self());          
            
            for(i=0; i< rand()%1000000000*1000; ++i);     //PERDE UM TEMPO RANDON
            
            pthread_mutex_unlock(&mutex1);          //LIBERA MUTEX1
            pthread_mutex_unlock(&mutex2);          //LIBERA MUTEX2             

            printf("\nThread1 successful! Unlock mutex1 and mutex2.\n\n");

            th1 = 1;                                //RECEBE 1 INDICANDO QUE ESTÁ OK
            return NULL;                            //FINALIZA A REPETIÇÃO
        }else{
            printf("\nThread ID%ld did not get mutex2.\n", pthread_self()); 
        }
        printf("Unlock mutex1.\n\n");
        pthread_mutex_unlock(&mutex1);              //LIBERA MUTEX1
    }else{ 
        printf("\nThread ID%ld did not get mutex1.\n", pthread_self());
    }
}

void* thread2(void *data){
    unsigned long i,j;
    //FUNÇÃO MODIFICADA
    if(pthread_mutex_trylock(&mutex2)==0){           //COMPARA PARA SABER SE O MUTEX2 ESTÁ SENDO USADO
        printf("Thread ID%ld got mutex2.\n", pthread_self());
        
        for(i=0; i< rand()%100000*1000; ++i);        //PERDE UM TEMPO RANDON
        
        if(pthread_mutex_trylock(&mutex1)==0){       //COMPARA PARA SABER SE O MUTEX1 ESTÁ SENDO USADO
            printf("Thread ID%ld got mutex1.\n", pthread_self());          
            
            for(i=0; i< rand()%1000000*1000; ++i); //PERDE UM TEMPO RANDON
            
            pthread_mutex_unlock(&mutex2);          //LIBERA MUTEX2 
            pthread_mutex_unlock(&mutex1);          //LIBERA MUTEX1             

            printf("\nThread2 successful! Unlock mutex2 and mutex1.\n\n");

            th2 = 1;                                //RECEBE 1 INDICANDO QUE ESTÁ OK
            return NULL;                            //FINALIZA A REPETIÇÃO
        }else{ 
            printf("\nThread ID%ld did not get mutex1.\n", pthread_self()); 
        }
        printf("Unlock mutex2.\n\n");
        pthread_mutex_unlock(&mutex2);               //LIBERA MUTEX2 
    }else{ 
        printf("\nThread ID%ld did not get mutex2.\n", pthread_self()); 
    }
}