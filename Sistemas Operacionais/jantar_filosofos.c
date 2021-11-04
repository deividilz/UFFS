/*

    UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
    ALUNO: DEIVIDI LUIZ
    SISTEMAS OPERACIONAIS
    TRABALHO: Implementar/resolver o problema do "Jantar dos Filósofos" em linguagem C, no Linux.

    Parte do código já estava implementado. 

    Como compilar: gcc -o test jantar_filosofos.c -lpthread

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <pthread.h>                    //biblioteca para as threads
#include <semaphore.h>                  //biblioteca para os semáforos
#include <unistd.h>                     //biblioteca para o sleep (aguardar)

#define N               5               //número de filosofos
#define LEFT            (i+N-1)%N       //número de vizinhos à esquerda de i
#define RIGHT           (i+1)%N         //número de vizinhos à direita de i
#define THINKING        0               //o filósofo está pensando
#define HUNGRY          1               //o filósofo está tentando pegar garfos (faminto)
#define EATING          2               //o filósofo está comendo

int state[N];                           //arranjo para controlar o estado de cada um
int num_phi[N];                         //vetor com os indices dos filósofos

sem_t s[N];                             //um semáforo por filósofo
sem_t mutex;                            //semáforo mutex para controle

pthread_t philosphers[N];               //threads vetor com todos os filósofos

void test(int i){                       //i: o número do filósofo, de 0 à N-1
    if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){
        state[i] = EATING;              //define o status para COMENDO
        sleep(2);                       //aguarda 2s
        sem_post(&s[i]);                //desbloqueia o semáforo no índice i
        printf("\n> Filosofo %d esta comendo | GE: %d | GD: %d |\n\n", i+1, LEFT+1, RIGHT+1);
    }
}

void think(int i){                      //função pensando
    printf("> %d pensando\n", i+1);     //imprime na tela
}

void eat(int i){                        //função comendo
    printf("> %d comendo\n", i+1);      //imprime na tela
}

void take_forks(int i){                 //i: o número do filósofo, de 0 à N-1
    sem_wait(&mutex);                   //entra na região crítica, bloqueia mutex
    state[i] = HUNGRY;                  //registra que o filósofo está faminto
    printf("> %d com fome\n", i+1);     //imprime na tela
    test(i);                            //tenta pegar dois garfos
    sem_post(&mutex);                   //sai da região crítica, desbloqueia mutex
    sem_wait(&s[i]);                    //bloqueia se os garfos não foram pegos
    sleep(2);                           //aguarda 2s
}

void put_forks(int i){                  //i: o número do filósofo, de 0 à N-1
    sem_wait(&mutex);                   //entra na região crítica, bloqueia mutex
    state[i] = THINKING;                //o filósofo acabou de comer
    printf("> %d pensando\n", i+1);     //imprime na tela
    test(LEFT);                         //vê se o vizinho da esquerda pode comer agora
    test(RIGHT);                        //vê se o vizinho da direita pode comer agora
    sem_post(&mutex);                   //sai da região crítica, libera mutex
}

void *philospher(void *p){              //i: o número do filósofo, de 0 a N-1
    while(true){                        //repete para sempre
        int *i = p;                     //ponteiro i para pegar o índice do filósofo

        think(*i);                      //o filósofo está pensando
        sleep(1);                       //aguarda 1s
        take_forks(*i);                 //pega dois garfos ou bloqueia
        eat(*i);                        //o filósofo está comendo
        printf("> %d colocou os garfos na mesa\n", *i+1);
        sleep(1);                       //aguarda 1s
        put_forks(*i);                  //devolve os dois garfos à mesa
    }
}

int main(void) {                        //função principal
    for(int i = 0; i<N; i++)            //adiciona o índice de cada filósofo no vetor
        num_phi[i] = i;                 //filósofo i recebe seu índice

    sem_init(&mutex, 0, 1);             //inicializa o semáforo mutex

    for(int i = 0; i < N; i++)          //inicializa todos os semáforos
        sem_init(&s[i], 0, 0);          //inicializa o semáforo i


    for(int i=0; i<N; i++)              //cria a thread para cada filósofo
      pthread_create(&philosphers[i], NULL, philospher, &num_phi[i]); 

    for(int i=0; i<N; i++) {            //caso terminar de executar, retorna as threads
      pthread_join(philosphers[i], NULL);
      printf("Thread id %ld returned\n", philosphers[i]);
    }

    pthread_exit(NULL);                 //termina a chamada da thread

    return(1);
}
