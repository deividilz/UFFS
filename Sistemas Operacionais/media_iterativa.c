/*

    UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
    ALUNO: DEIVIDI LUIZ
    SISTEMAS OPERACIONAIS
    TRABALHO: Média iterativa uni-dimensional (One-Dimensional Iterative Averaging)
        - Esse problema consiste em resolver a recorrência da média iterativa em um vetor unidimensional.
        - Vamos assumir índices em vetores V com N elementos no intervalo de 0 a N - 1 

    Parte do código já estava implementado.

    Como compilar: gcc -o teste media_iterativa.c -lpthread -lm

*/

#include <pthread.h>    //BIBLIOTECA DAS THREADS
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,*/
#include <unistd.h>     /* Prototypes for many system calls */
#include <errno.h>      /* Declares errno and defines error constants */
#include <string.h>     //BIBLIOTECA PARA STRINGS  
#include <math.h>       //BIBLIOTECA MATEMÁTICA
#include <time.h>       //BIBLIOTECA PARA O TEMPO DE EXECUÇÃO

static pthread_barrier_t    barrier1, barrier2; //DECLARAÇÃO DAS BARREIRAS

static int numBarriers;     //VALOR ESTÁTICO PARA A QUANTIDADE DE BARREIRAS
static int numThreads;      //VALOR ESTÁTICO PARA A QUANTIDADE DE THREADS

#define N 10000             //DEFINE O TAMANHO DO VERTOR

float velho[N+2];           //CRIA UM VETOR DE FLOAT COM TAMANHO N+2 (VETOR VELHO)
float novo[N+2];            //CRIA UM VETOR DE FLOAT COM TAMANHO N+2 (VETOR NOVO)
float temp[N+2];            //CRIA UM VETOR DE FLOAT COM TAMANHO N+2 (VETOR TEMPORÁRIO, PARA SWAP)
int fatia = 0;

static void *threadFunc(void *arg){                 //FUNÇÃO PARA EXECUÇÃO NO VETOR                
    int s;                                          //IRÁ RECEBER OS RETORNOS DAS BARREIRAS
    int nsecs;                                      //VARIÁVEL QUE VAI RECEBER O TEMPO ALEATÓRIO
    long threadNum = (long) arg;                    //CONVERTE PARA SABER O ID DA THREAD

    printf("Thread %ld started\n", threadNum);      //IMPRIME A THREAD INICIADA

    int inicio = threadNum*fatia + 1;               //CALCULA A FATIA DE INÍCIO QUE A THREAD IRÁ SELECIONAR
    int fim = inicio + fatia - 1;                   //CALCULA A FATIA DE FIM QUE A THREAD IRÁ SELECIONAR

    if (inicio > N)                                 //SE O INICIO FOR MAIOR QUE O TAMANHO DO VETOR
        inicio = N;                                 //RECEBE O TAMANHO MÁXIMO DO VETOR

    if (fim>(N) && numThreads > 1)                  //SE O FIM FOR MAIOR QUE O TAMANHO DO VETOR
        fim = N;                                    //RECEBE O TAMANHO MÁXIMO DO VETOR
    
    for (int j = 0; j < numBarriers; j++) {         //REPETE A EXECUÇÃO ATÉ QUE PASSE POR TODAS AS BARREIRAS DEFINIDAS
        nsecs = random() % 2 + 1;                   //VALOR ALEATÓRIO DE 1 A 3 SEGUNDOS
        sleep(nsecs);                               //FAZ A PAUSA DE N SEGUNDOS

        s = pthread_barrier_wait(&barrier1);        //S RECEBE O RETORNO DA BARREIRA APÓS AGUARDAR TODAS AS THREADS

        for(int i = inicio; i<=fim; i++){           //VAI PERCORRER NA FATIA DEFINIDA PELA THREAD
            novo[i] = (velho[i-1] + velho[i+1])/2;  //FAZ A CONTA DA MÉDIA PEGANDO O VALOR ANTERIOR E O VALOR POSTERIOR
        }

        for(int i = 0; i<=N+1; i++){                //PERCORRE TODO O VETOR PARA IMPRIMIR AS MUDANÇAS
            if(threadNum+1 == numThreads)           //IMPRIME SOMENTE QUANDO CHEGAR NA ÚLTIMA THREAD (EVITA QUE IMPRIMA DADOS NA ORDEM ERRADA)
                printf(" %f |", novo[i]);           //IMPRIME NA TELA
        }

        s = pthread_barrier_wait(&barrier2);        //S RECEBE O RETORNO DA BARREIRA APÓS AGUARDAR TODAS AS THREADS  

        if (s == 0) {                                               //VAI COMPARAR O RETORNO DE S
            printf("Thread %ld passed barrier %d\n", threadNum, j); //IMPRIME NA TELA
        }else if(s == PTHREAD_BARRIER_SERIAL_THREAD){               //CASO O RETORNO FOR ESPECIAL, SERÁ FEITO O SWAP
            for(int i=1; i<N+1; i++)    //PERCORRE O VETOR
                temp[i] = novo[i];      //TEMP IRÁ RECEBER OS VALORES NOVOS

            for(int i=1; i<N+1; i++)    //PERCORRE O VETOR
                novo[i] = velho[i];     //NOVO IRÁ RECEBER O NOVO NOVO

            for(int i=1; i<N+1; i++)    //PERCORRE O VETOR
                velho[i] = temp[i];     //VELHO IRÁ RECEBER OS VALORES NOVOS, QUE PASSAM A SER ANTIGOS

            printf("\n");
        }  
    }
    
    printf("Thread %ld terminating\n", threadNum);  //IMPRIME NA TELA
    return NULL;
}

int main(int argc, char *argv[]){                   //FUNÇÃO PRINCIPAL
    int s;                                          //IRÁ RECEBER OS RETORNOS DAS BARREIRAS
    pthread_t *tid;                                 
    
    for(int i = 0; i<N; i++){                       //PERCORRE TODO O VETOR PREENCHENDO COM ZEROS
        velho[i] = 0.0;                             //DEFINE TODOS OS CAMPOS DE VELHO PARA ZERO
        novo[i] = 0.0;                              //DEFINE TODOS OS CAMPOS DE NOVO PARA ZERO
        temp[i] = 0.0;                              //DEFINE TODOS OS CAMPOS DE TEMP PARA ZERO
    }

    velho[N+1] = 1.0;                               //ÚLTIMA POSIÇÃO DO VETOR DEFINIDA COMO 1
    novo[N+1] = 1.0;                                //ÚLTIMA POSIÇÃO DO VETOR DEFINIDA COMO 1
    temp[N+1] = 1.0;                                //ÚLTIMA POSIÇÃO DO VETOR DEFINIDA COMO 1

    if (argc != 3 || strcmp(argv[1], "--help") == 0)//CASO A FORMA DE EXECUÇÃO ESTIVER ERRADA
        printf("Usage: %s num-barriers num-threads\n", argv[0]);

    numThreads = atoi(argv[1]);                     //CONVERTE OS VALORES DE STRING PARA INT
    numBarriers = atoi(argv[2]);                    //CONVERTE OS VALORES DE STRING PARA INT

    tid = calloc(sizeof(pthread_t), numThreads);    //ALOCA ESPAÇO PARA A QUANTIDADE DE THREADS NECESSÁRIAS
    if (tid == NULL)                                //SE FOR NULL, NÃO DEU CERTO
        printf("calloc error");

    s = pthread_barrier_init(&barrier1, NULL, numThreads);  //INICIA A BARREIRA 1
    if (s != 0)                                             //SE NÃO FOR ZERO, DEU ERRO
        printf(" Error on pthread_barrier_init");
    s = pthread_barrier_init(&barrier2, NULL, numThreads);  //INICIA A BARREIRA 2
    if (s != 0)                                             //SE NÃO FOR ZERO, DEU ERRO                        
        printf(" Error on pthread_barrier_init");   
        
    float n = N;
    fatia = ceil(n/numThreads);                             //FAZ O CÁLCULO DA FATIA
    
    clock_t t;                                              //VARIÁVEL PARA ARMAZENAR O TEMPO
    t = clock();                                            //ARMAZENA O TEMPO

    for (long threadNum = 0; threadNum < numThreads; threadNum++) {                     //VAI PERCORRER TODAS AS THREADS, CHAMANDO A FUNÇÃO PARA CADA
        s = pthread_create(&tid[threadNum], NULL, threadFunc, (void *) threadNum);      //CRIA A THREAD E CHAMA A FUNÇÃO
        if (s != 0)                                                                     //SE NÃO FOR ZERO, DEU ERRO
            printf("Error pthread_create");
    }

    usleep(100000);                             //DORME PELO TEMPO DEFINIDO
    printf("\n");

    for(int i = 0; i<=N+1; i++){                //PERCORRE TODO O VETOR PARA IMPRIMIR AS MUDANÇAS
        printf(" %f |", novo[i]);               //IMPRIME NA TELA
    }

    printf("\n");
    
    for (int threadNum = 0; threadNum < numThreads; threadNum++) {  //PERCORRE TODAS AS THREADS PARA INICIÁ-LAS JUNTAS
        s = pthread_join(tid[threadNum], NULL);                     //SINCROZINA CADA THREAD
        if (s != 0)                                                 //SE NÃO FOR ZERO, DEU ERRO
            printf("Error pthread_join");
    }

    t = clock() - t;                                                //TEMPO FINAL - TEMPO INICIAL
    printf("\nTempo de execucao: %lf\n\n", ((double)t)/((CLOCKS_PER_SEC/1000))); //CONVERTE PARA DOUBLE E IMPRIME NA TELA

    pthread_exit (NULL);                                            //SAI DA THREAD

    exit(0);
}
