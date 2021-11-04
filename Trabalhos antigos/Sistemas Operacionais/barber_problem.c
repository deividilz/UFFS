/*

    UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
    ALUNO: DEIVIDI LUIZ
    SISTEMAS OPERACIONAIS
    TRABALHO: problema do Sleeping Barber Problem (Problema do Barbeiro Adormecido)


    Como compilar: gcc -o test barber_problem.c -lpthread

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>        //BIBLIOTECA THREADS
#include <unistd.h>         //BIBLIOTECA PARA SLEEP
#include <stdbool.h>        //BIBLIOTECA PARA TRUE/FALSE
#include <semaphore.h>      //BIBLIOTECA PARA OS SEMÁFAROS

#define q_thread        9   //QUANTIDADE DE PESSOAS/THREADS
#define chairs          5   //QUANTIDADE DE CADEIRAS

int seated = 0;             //QUANTIDADE PESSOAS SENTADAS
bool occupied = false;      //SABER QUANDO ESTÁ OCUPADO = TRUE
bool wakeup = true;        //SABER QUANDO ESTÁ ACORDADO = TRUE

int attended[q_thread];     //VETOR DOS ATENDIDOS, USADO (1- ATENDIDO, 2- AGUARDANDO, 3- FOI DAR UMA VOLTA, 0- NÃO ESTÁ NA BARBEARIA AINDA)
int chair_thread[q_thread]; //VETOR DA CADEIRA PARA SABER QUEM TÁ SENTADO

sem_t semaphore;            //DECLARAÇÃO SEMÁFARO

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER, //DECLARAÇÃO DOS MUTEXs
                mutex2 = PTHREAD_MUTEX_INITIALIZER,
                mutex3 = PTHREAD_MUTEX_INITIALIZER,
                mutex4 = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t  condition_var       = PTHREAD_COND_INITIALIZER;
pthread_cond_t  condition_var2      = PTHREAD_COND_INITIALIZER;

void *barbearia(void *data);
void sleepWakeUP();

void imprimeCadeiras(){                                     //FUNÇÃO PARA IMPRIMIR AS CADEIRAS
    int sobrando = 0;                                       //PARA SABER QUANTAS CADEIRAS TEM SOBRANDO

    printf("Chair: \t\t|");                                 //IMPRIME NA TELA
    for(int i = 0; i<q_thread; i++){                        //PERCORRE TODAS AS THREADS PARA SABER QUEM ESTÁ SENTADO
        if(attended[i] == 2){                               //SE FOR = 2 É PORQUE ESTÁ AGUARDANDO SENTADO
            printf(" T%d |", chair_thread[i]);              //IMPRIME NA TELA A THREAD CORRESPONDENTE
            sobrando++;                                     //SOBRANDO SOMA 1
        }
    }
    
    if(sobrando < chairs)                                   //SE O SOBRANDO FOR MENOR QUE AS CADEIRAS, OU SEJA, NÃO OCUPOU TODAS
        for(int i=0; i<chairs-sobrando; i++){               //PERCORRE A QUANTIDADE DE CADEIRAS - SOBRARAM
            printf("    |");                                //IMPRIME PARA PODER DEIXAR SEMPRE COM TAMANHO IGUAL
        }
    
    printf("\n");
}

void *cortarCabelo(void *data){                             //FUNÇÃO PARA CORTAR O CABELO
    long threadNum = (long) data;                           //CONVERTE O ID DA THREAD
    int nsecs = 0;                                          //USADO PARA OS SEGUNDOS DE ESPERA

    sem_wait(&semaphore);                                   //OPERAÇÃO DOWN
    seated--;                                               //SENTADOS SUBTRAI, SAIU DA CADEIRA E FOI ATENDIDO
    occupied = true;                                        //BARBEIRO OCUPADO

    sleepWakeUP();                                          //CHAMA FUNÇÃO PARA VERIFICAR SE O BARBEIRO ESTÁ DORMINDO OU ACORDADO
    
    printf("\nAttended: \t| T%ld |\n", threadNum);          //IMPRIME A PESSOA DA VEZ QUE ESTÁ SENDO ATENDINDO

    sem_post(&semaphore);                                   //OPERAÇÃO UP

    nsecs = random() % 5 + 1;                               //PEGA UM TEMPO RANDOMICO
    sleep(nsecs);                                           //AGUARDA POR X SEGUNDOS

    imprimeCadeiras();                                      //IMPRIME TODAS AS CADEIRAS
    
    chair_thread[threadNum] = 0;                            //CADEIRA DA THREAD FICA ZERO

    occupied = false;                                       //OCUPADO VOLTA A SER FALSO
    pthread_mutex_unlock(&mutex1);                          //MUTEX1 É LIVRE
    return NULL;                                            //RETORNA NULL
}

void *darVolta(void *data){                                 //FUNÇÃO DAR VOLTA, QUANDO ESTÁ SEM CADEIRAS DISPONÍVEIS
    long threadNum = (long) data;                           //CONVERTE O ID DA THREAD
    int nsecs = 0;                                          //USADO PARA OS SEGUNDOS DE ESPERA

    nsecs = random() % 5 + 1;                               //PEGA UM TEMPO RANDOMICO
    sleep(nsecs);                                           //AGUARDA POR X SEGUNDOS

    attended[threadNum] = 3;                                //ATENDIDO RECEBE 1

    printf("\nThread %ld went for a walk for %d seconds..\n\n", threadNum, nsecs); 
    return NULL;                                            //RETORNA NULL
}

void *aguardaVez(void *data){                               //FUNÇÃO AGUARDA VEZ PARA SER ATENDIDO
    long threadNum = (long) data;                           //CONVERTE O ID DA THREAD
    int nsecs = 0;                                          //USADO PARA OS SEGUNDOS DE ESPERA

    while(seated > 0){                                      //ENQUANTO TIVER CADEIRAS DISPONÍVEIS
        if(attended[threadNum] != 1){                       //CASO FOR DIFERENTE DE ATENDIDO
            if(pthread_mutex_trylock(&mutex1)==0){          //BLOQUEIA O MUTEX1
                attended[threadNum] = 1;                    //ATENDIDO RECEBE 1
                cortarCabelo(data);                         //CHAMA FUNÇÃO PARA CORTAR O CABELO
            }else{                                          //SE O BARBEIRO ESTIVER OCUPADO = MUTEX BLOQUEADO
                nsecs = random() % 5 + 1;                   //PEGA UM TEMPO RANDOMICO
                sleep(nsecs);                               //AGUARDA POR X SEGUNDOS

                chair_thread[threadNum] = threadNum;        //SALVA O ID DA THREAD NO SEU PRÓPRIO ID
            }
        }
    }

    return NULL;                                            //RETORNA NULL
}

void sleepWakeUP(){                                                 //FUNÇÃO PARA SABER SE O BARBEIRO ESTÁ DORMINDO OU ACORDADO
    if(pthread_mutex_trylock(&mutex4)==0){                          //BLOQUEIA O MUTEX4
        if(seated==0 && occupied == false && wakeup == true){       //SE ELE NÃO ESTIVER ATENDENDO
            printf("\nBarber is sleeping because he is without clients!\n");
            imprimeCadeiras();
            wakeup = false;
        }

        if(seated == 1 && wakeup == false && occupied == true){     //SE ELE ESTIVER ATENDENDO
            wakeup = true;                                          //ACORDADO RECEBE VERDADEIRO
            printf("\nThe barber was awake, to work!\n");           //IMPRIME NA TELA QUE ELE ACORDOU
        }

        pthread_mutex_unlock( &mutex4 );                            //DESBLOQUEIA MUTEX4
    }
}

void* barbearia(void *data){                                //FUNÇÃO PRINCIPAL DA BARBEARIA
    long threadNum = (long) data;                           //CONVERTE O ID DA THREAD
    int nsecs = 0;                                          //USADO PARA OS SEGUNDOS DE ESPERA

    nsecs = random() % 5 + 1;                               //PEGA UM TEMPO RANDOMICO
    sleep(nsecs);                                           //AGUARDA POR X SEGUNDOS

    sleepWakeUP();                                          //CHAMA FUNÇÃO PARA VERIFICAR SE O BARBEIRO ESTÁ DORMINDO OU ACORDADO

    while(attended[threadNum] != 1){
        if(pthread_mutex_trylock(&mutex2)==0){                  //BLOQUEIA O MUTEX2 SE ESTIVER DISPONÍVEL
            if(seated < chairs){                                //VERIFICA SE TEM CADEIRA DISPONÍVEL
                seated++;                                       //SENTADO SOMA 1

                if(attended[threadNum] == 3)                    //CASO FOR 3 ELE JÁ FOI DAR UMA VOLTA
                    printf("\nThread %ld returned to the barber shop..\n", threadNum); 

                attended[threadNum] = 2;                        //ATENDIDO MUDA STATUS PARA AGUARDANDO
                chair_thread[threadNum] = threadNum;            //SALVA O ID DA THREAD NO SEU PRÓPRIO ID
                pthread_mutex_unlock( &mutex2 );                //DESBLOQUEIA O MUTEX2

                aguardaVez(data);                               //SE TEM CADEIRA DISPONÍVEL, VAI AGUARDAR A VEZ
            }else{
                if(attended[threadNum] == 3)
                    printf("\nThread %ld returned but there are still no chairs available!\n\n", threadNum); 

                pthread_mutex_unlock( &mutex2 );                //DESBLOQUEIA O MUTEX2
                darVolta(data);                
            }
        }
    }

    sleepWakeUP();                                          //CHAMA FUNÇÃO PARA VERIFICAR SE O BARBEIRO ESTÁ DORMINDO OU ACORDADO
    return NULL;
}

int main(){                                                 //MAIN PRINCIPAL
    pthread_t thread[q_thread];                             //DECLARAÇÃO DA QUANTIDADE DE THREADS

    sem_init(&semaphore, 0, 1);                             //INCICIALIZA O SEMÁFARO COM VALOR 1

    for(long i=0; i<q_thread; i++){                         //PERCORRE TODAS AS THREADS
        pthread_create(&thread[i], NULL, barbearia, (void *) i);  //CRIA O THREAD i
        attended[i] = 0;                                    //ATENDIDO RECEBE 0
        chair_thread[i] = 0;                                //CADEIRA RECEBE O ID ZERO
    }
    
    for(int i=0; i<q_thread; i++){                          //PERCORRE PARA SINCRONIZAR TODAS AS THREADS
         pthread_join(thread[i], NULL);                     //JOIN EM TODAS AS THREADS         
    }

    printf("\n\nAttended: \t|    |\n");                     //APENAS IMPRIME VAZIO
    imprimeCadeiras();                                      //IMPRIME AS CADEIRAS NOVAMENTE
    printf("\nEmpty barber shop!\n\n");                       //IMPRIME NA TELA

    sleepWakeUP();                                          //CHAMA FUNÇÃO PARA VERIFICAR SE O BARBEIRO ESTÁ DORMINDO OU ACORDADO
    
    pthread_mutex_destroy(&mutex1);                         //DESTRÓI MUTEX
    pthread_mutex_destroy(&mutex2);                         //DESTRÓI MUTEX
    pthread_mutex_destroy(&mutex3);                         //DESTRÓI MUTEX
    pthread_mutex_destroy(&mutex4);                         //DESTRÓI MUTEX
    pthread_exit (NULL);                                    //SAI DA THREAD
    
    return 0; 
}