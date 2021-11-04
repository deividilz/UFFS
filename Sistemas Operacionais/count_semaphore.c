/*

    UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
    ALUNO: DEIVIDI LUIZ
    SISTEMAS OPERACIONAIS
    TRABALHO: Cada thread incrementa a variável global uma única vez em linguagem C, no Linux.
                - Usar semáfaros

    Parte do código já estava implementado.

    Como compilar: gcc -o test count_semaphore.c -lpthread

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>        //BIBLIOTECA THREADS
#include <unistd.h>         //BIBLIOTECA PARA O SLEEP
#include <semaphore.h>      //BIBLIOTECA PARA OS SEMÁFAROS

void *mythread(void *data);

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

#define NUM_THREADS 3       //NÚMERO DE THREADS
#define COUNT_DONE  10      //VALOR MÁXIMO PARA CONCLUIR CONTAGEM

sem_t semaphore;            //DECLARAÇÃO SEMÁFARO

int count = 0;              //CONTADOR GLOBAL

int main(void) {
	pthread_t tids[NUM_THREADS];        //DECLARAÇÃO DAS THREADS
	int i=0;

	for(i=0; i<NUM_THREADS; i++) {
		int *j = malloc(sizeof(int));                           //ALOCA ESPAÇO DO TIPO INT E INSERE EM J
		*j = i;                                                 //J RECEBE O CONTEÚDO DE I
		pthread_create(&tids[i], NULL, mythread, (void *)j);    //CRIA AS THREADS
	}

    sem_init(&semaphore, 0, 1);         //INCICIALIZA O SEMÁFARO COM VALOR 1

	for(i=0; i<NUM_THREADS; i++) {      //SINCRONIZA AS THREADS
		pthread_join(tids[i], NULL);
	}

    pthread_exit (NULL);                //SAI DA THREAD

    return 0; 
}

void *mythread(void *data) {
	int id;                             //IDENTIFICADOR DA THREAD
   	id = *((int *) data);               //ID RECEBE A CONVERSÃO DE DATA
	free((int *)data);                  //LIBERA DATA

	while(count < COUNT_DONE){          //ENQUANTO O CONTADOR NÃO CHEGAR AO MÁXIMO
        if (id == count%NUM_THREADS){   //SE O IDENTIFICADOR DA THREAD FOR IGUAL AO RESTO DA DIVISÃO
            sem_wait(&semaphore);       //OPERAÇÃO DOWN
            count++;                    //CONTADOR SOMA 1
            printf("Thread%d: %d\n",id, count);
            sem_post(&semaphore);       //OPERAÇÃO UP
            sleep(1);                   //AGUARDA 1 SEGUNDO
        }
	}
	pthread_exit(NULL);
}

