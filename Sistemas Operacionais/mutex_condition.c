/*

    UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
    ALUNO: DEIVIDI LUIZ
    SISTEMAS OPERACIONAIS
    TRABALHO: Cada thread incrementa a variável global uma única vez em linguagem C, no Linux.

    Parte do código já estava implementado. 

    Como compilar: gcc -o test mutex_condition.c -lpthread

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t count_mutex         = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var       = PTHREAD_COND_INITIALIZER;
pthread_cond_t  condition_var2      = PTHREAD_COND_INITIALIZER;

void *functionCount1();
void *functionCount2();
void *functionCount3();

int  count = 0;
int  controller = 0;        //CONTROLADOR DE THREADS

#define COUNT_MAX    6      //CONTADOR MÁXIMO
#define COUNT_HALT1  0      //CONTADOR DA thread1
#define COUNT_HALT2  1      //CONTADOR DA thread2
#define COUNT_HALT3  2      //CONTADOR DA thread3

void main(){
    pthread_t thread1, thread2, thread3;

    pthread_create(&thread1, NULL, &functionCount1, NULL);   
    pthread_create(&thread2, NULL, &functionCount2, NULL);
    pthread_create(&thread3, NULL, &functionCount3, NULL);

    pthread_join( thread2, NULL);

    printf("Final count: %d\n",count);
    
    exit(EXIT_SUCCESS);
}

void *functionCount1(){
   for(;;){
        pthread_mutex_lock( &count_mutex );                 //BLOQUEIA O MUTEX
        pthread_cond_wait( &condition_var, &count_mutex );  //AGUARDA A CONDIÇÃO 1 SER SINALIZADA
        count++;                                            //CONTADOR SOMA 1
        controller++;                                       //CONTROLADOR SOMA 1
        printf("Counter value functionCount1: %d\n",count); //IMPRIME DA THREAD 1

        pthread_mutex_unlock( &count_mutex );               //DESBLOQUEIA O MUTEX
        
        if(count >= COUNT_MAX){break;}                       //TERMINA A REPETIÇÃO SE FOR IGUAL AO MÁXIMO
    }

    pthread_exit(NULL);
}

void *functionCount2(){
    unsigned long i;
    for(;;){
        pthread_mutex_lock( &count_mutex );                 //BLOQUEIA O MUTEX

        if(controller == COUNT_HALT1){                      //SE O CONTROLADOR FOR 1
            pthread_mutex_unlock( &count_mutex );           //DESBLOQUEIA O MUTEX
            pthread_cond_signal( &condition_var );          //SINALIZA A CONDIÇÃO 1
        }else if(controller == COUNT_HALT2){                //SE O CONTROLADOR FOR 2
            count++;                                        //CONTADOR SOMA 1
            controller++;                                   //VARIÁVEL DE CONTROLER SOMA 1
            printf("Counter value functionCount2: %d\n",count); //IMPRIME DA THREAD 2
            pthread_mutex_unlock( &count_mutex );           //DESBLOQUEIA O MUTEX
        }else{                                              //SENÃO O CONTROLADOR NÃO FOR 1 OU 2
            pthread_mutex_unlock( &count_mutex );           //DESBLOQUEIA O MUTEX
            pthread_cond_signal( &condition_var2 );         //SINALIZA A CONDIÇÃO 2
        }   
        
       if(count >= COUNT_MAX){break;}                       //TERMINA A REPETIÇÃO SE FOR IGUAL AO MÁXIMO
    }

    pthread_exit(NULL);
}

void *functionCount3(){
    unsigned long i;
    for(;;){
        pthread_mutex_lock( &count_mutex );                 //BLOQUEIA O MUTEX
        pthread_cond_wait( &condition_var2, &count_mutex ); //AGUARDA A CONDIÇÃO 2 SER SINALIZADA

        if(controller == COUNT_HALT1){                      //SE O CONTROLADOR FOR 1
            pthread_mutex_unlock( &count_mutex );           //DESBLOQUEIA O MUTEX
            pthread_cond_signal( &condition_var );          //SINALIZA A CONDIÇÃO 1
        }else if(controller == COUNT_HALT3){                //SE O CONTROLADOR FOR 3
            count++;                                        //CONTADOR SOMA 1
            controller = 0;                                 //CONTROLADOR SOMA 1
            printf("Counter value functionCount3: %d\n",count); //IMPRIME DA THREAD 3
            pthread_mutex_unlock( &count_mutex );           //DESBLOQUEIA O MUTEX
        }     
        
       if(count >= COUNT_MAX){break;}                       //TERMINA A REPETIÇÃO SE FOR IGUAL AO MÁXIMO
    }

    pthread_exit(NULL);
}
