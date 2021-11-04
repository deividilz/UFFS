/**  UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
    ALUNO: DEIVIDI LUIZ
    SISTEMAS OPERACIONAIS

    TRABALHO: encontrar uma forma de consertar o deadlock causado pelas threads 

    Como compilar: gcc -o test deadlock_trylock.c -lpthread*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void *thread1(void *data);
void *thread2(void *data);

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER, mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_t Thread1, Thread2;

int main(void) {                                
    pthread_create(&Thread1, NULL, thread1, NULL);      
    pthread_create(&Thread2, NULL, thread2, NULL);

    //now join them
    pthread_join(Thread1, NULL);
    printf("\nThread id %ld returned\n", Thread1);
    pthread_join(Thread2, NULL);
    printf("Thread id %ld returned\n", Thread2);

    return 1;
}

void *thread1(void *data){
     unsigned long i,j;
     //FUNÇÃO MODIFICADA
     while(true){                                       //EXECUTA PARA SEMPRE OU ATÉ ENCONTRAR O BREAK
        if(pthread_mutex_trylock(&mutex1)==0){          //COMPARA PARA SABER SE O MUTEX1 ESTÁ SENDO USADO
             printf("Thread ID%ld got mutex1.\n", pthread_self());

            for(i=0; i< rand()%100000000*1000; ++i);    //PERDE UM TEMPO RANDON

            if(pthread_mutex_trylock(&mutex2)==0){      //COMPARA PARA SABER SE O MUTEX2 ESTÁ SENDO USADO
                printf("Thread ID%ld got mutex2.\n", pthread_self());          
                
                for(i=0; i< rand()%1000*1000; ++i);     //PERDE UM TEMPO RANDON
                
                pthread_mutex_unlock(&mutex2);          //LIBERA MUTEX2  
                break;                                  //FINALIZA A REPETIÇÃO
            }else{
                printf("\nThread ID%ld did not get mutex2.\n\n", pthread_self()); 
                pthread_mutex_unlock(&mutex2);          //LIBERA MUTEX2 
            }
            pthread_mutex_unlock(&mutex1);              //LIBERA MUTEX1
        }else{ 
            printf("\nThread ID%ld did not get mutex1.\n", pthread_self());
        }
    }
     
    pthread_exit(NULL);
}

void *thread2(void *data){
    unsigned long i,j;
    //FUNÇÃO MODIFICADA
    while(true){                                         //EXECUTA PARA SEMPRE OU ATÉ ENCONTRAR O BREAK
        if(pthread_mutex_trylock(&mutex2)==0){           //COMPARA PARA SABER SE O MUTEX2 ESTÁ SENDO USADO
            printf("Thread ID%ld got mutex2.\n", pthread_self());
            
            for(i=0; i< rand()%100000*1000; ++i);        //PERDE UM TEMPO RANDON
            
            if(pthread_mutex_trylock(&mutex1)==0){       //COMPARA PARA SABER SE O MUTEX1 ESTÁ SENDO USADO
                printf("Thread ID%ld got mutex1.\n", pthread_self());          
                
                for(i=0; i< rand()%100000000*1000; ++i); //PERDE UM TEMPO RANDON
                
                pthread_mutex_unlock(&mutex1);           //LIBERA MUTEX1   
                break;                                   //FINALIZA A REPETIÇÃO
            }else{ 
                printf("\nThread ID%ld did not get mutex1.\n\n", pthread_self()); 
                pthread_mutex_unlock(&mutex1);           //LIBERA MUTEX1 
            }
            pthread_mutex_unlock(&mutex2);               //LIBERA MUTEX2 
        }else{ 
            printf("\nThread ID%ld did not get mutex2.\n", pthread_self()); 
        }
    }
     
    pthread_exit(NULL);
}






